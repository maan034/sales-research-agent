#include "sra/research/OpenAIResearchProvider.h"
#include <drogon/HttpClient.h>
#include <json/json.h>
#include <iostream>
#include <sstream>
#include <future>
#include <chrono>

namespace sra::research {

OpenAIResearchProvider::OpenAIResearchProvider(std::string apiKey, std::string apiEndpoint)
    : apiKey_(std::move(apiKey)), apiEndpoint_(std::move(apiEndpoint)) {}

sra::domain::ResearchResult OpenAIResearchProvider::research(const std::string& companyName) const {
    std::string defaultPrompt = "Perform sales research on company '" + companyName + "'. "
                                "Respond ONLY in JSON format with keys: "
                                "\"industry\" (string), "
                                "\"description\" (string), "
                                "\"icpScore\" (integer 0-100), "
                                "\"recommendation\" (string).";
    return researchWithPrompt(companyName, defaultPrompt);
}

sra::domain::ResearchResult OpenAIResearchProvider::researchWithPrompt(
    const std::string& companyName, 
    const std::string& customPrompt) const 
{
    std::cout << "Starting research for " << companyName << "...\n";
    std::cout << " -> Connecting to local Ollama endpoint: " << apiEndpoint_ << std::endl;

    auto client = drogon::HttpClient::newHttpClient(apiEndpoint_);

    // Construct OpenAI-compatible payload for Ollama
    Json::Value payload;
    payload["model"] = "llama3.2";
    payload["temperature"] = 0.2;

    Json::Value messages(Json::arrayValue);
    
    Json::Value systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = "You are a sales research assistant. Always output valid raw JSON matching the requested fields.";
    messages.append(systemMessage);

    Json::Value userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = customPrompt;
    messages.append(userMessage);

    payload["messages"] = messages;

    auto req = drogon::HttpRequest::newHttpRequest();
    req->setMethod(drogon::Post);
    req->setPath("/v1/chat/completions");
    req->setContentTypeCode(drogon::CT_APPLICATION_JSON);

    Json::StreamWriterBuilder writerBuilder;
    req->setBody(Json::writeString(writerBuilder, payload));

    // Async to Sync bridging using std::promise
    auto promisePtr = std::make_shared<std::promise<std::shared_ptr<drogon::HttpResponse>>>();
    auto future = promisePtr->get_future();

    const int maxRetries = 3;
    int currentAttempt = 0;
    bool requestSucceeded = false;
    std::shared_ptr<drogon::HttpResponse> responsePtr = nullptr;

    while (currentAttempt < maxRetries && !requestSucceeded) {
        currentAttempt++;
        std::cout << " -> [Attempt " << currentAttempt << "/" << maxRetries 
                  << "] Requesting Ollama inference (Timeout: 180s)..." << std::endl;

        promisePtr = std::make_shared<std::promise<std::shared_ptr<drogon::HttpResponse>>>();
        future = promisePtr->get_future();

        client->sendRequest(
            req,
            [promisePtr](drogon::ReqResult result, const drogon::HttpResponsePtr& response) {
                if (result == drogon::ReqResult::Ok && response) {
                    promisePtr->set_value(response);
                } else {
                    promisePtr->set_value(nullptr);
                }
            },
            180.0
        );

        if (future.wait_for(std::chrono::seconds(185)) == std::future_status::ready) {
            responsePtr = future.get();
            if (responsePtr && responsePtr->getStatusCode() == drogon::k200OK) {
                requestSucceeded = true;
                std::cout << " -> Response successfully received from Ollama on attempt " 
                          << currentAttempt << "!\n";
            } else {
                std::cerr << " -> [Attempt " << currentAttempt << " Failed] HTTP Status: " 
                          << (responsePtr ? std::to_string(responsePtr->getStatusCode()) : "No Response / Network Error") 
                          << std::endl;
            }
        } else {
            std::cerr << " -> [Attempt " << currentAttempt << " Failed] Request timed out on client wait.\n";
        }
    }

    sra::domain::ResearchResult result;
    result.company.name = companyName;

    if (!requestSucceeded || !responsePtr) {
        std::cerr << " -> [Error] All retry attempts to local Ollama failed.\n";
        result.icpScore = 50;
        result.recommendation = "Inference failed due to Ollama endpoint issue.";
        return result;
    }

    // Parse Response
    Json::CharReaderBuilder readerBuilder;
    Json::Value rootResponse;
    std::string errs;
    std::string bodyStr(std::string(responsePtr->getBody()));
    std::stringstream ss(bodyStr);

    if (Json::parseFromStream(readerBuilder, ss, &rootResponse, &errs)) {
        if (rootResponse.isMember("choices") && !rootResponse["choices"].empty()) {
            std::string rawContent = rootResponse["choices"][0]["message"]["content"].asString();

            std::cout << "\n================ OLLAMA RESPONSE ================\n" 
                      << rawContent 
                      << "\n=================================================\n" << std::endl;

            // Extract valid JSON object by isolating substring between first '{' and last '}'
            size_t startPos = rawContent.find('{');
            size_t endPos = rawContent.rfind('}');

            std::string jsonPayload = "";
            if (startPos != std::string::npos && endPos != std::string::npos && endPos > startPos) {
                jsonPayload = rawContent.substr(startPos, (endPos - startPos) + 1);
            } else {
                jsonPayload = rawContent;
            }

            Json::Value contentJson;
            std::stringstream contentSs(jsonPayload);
            if (Json::parseFromStream(readerBuilder, contentSs, &contentJson, &errs)) {
                if (contentJson.isMember("industry") && contentJson["industry"].isString()) {
                    result.company.industry = contentJson["industry"].asString();
                }
                if (contentJson.isMember("description") && contentJson["description"].isString()) {
                    result.company.description = contentJson["description"].asString();
                }
                if (contentJson.isMember("icpScore")) {
                    if (contentJson["icpScore"].isInt()) {
                        result.icpScore = contentJson["icpScore"].asInt();
                    } else if (contentJson["icpScore"].isString()) {
                        try {
                            result.icpScore = std::stoi(contentJson["icpScore"].asString());
                        } catch (...) {
                            result.icpScore = 75;
                        }
                    }
                } else {
                    result.icpScore = 75;
                }
                if (contentJson.isMember("recommendation") && contentJson["recommendation"].isString()) {
                    result.recommendation = contentJson["recommendation"].asString();
                }
            } else {
                std::cerr << " -> [Warn] Failed to parse inner JSON object: " << errs << std::endl;
                result.company.description = rawContent;
                result.icpScore = 75;
                result.recommendation = "Extracted from raw response text.";
            }
        }
    } else {
        result.icpScore = 70;
        result.recommendation = "Default prospect (Root JSON parse failure)";
    }

    return result;
}

} // namespace sra::research
#include "sra/api/ResearchController.h"
#include "sra/research/OpenAIResearchProvider.h"
#include "sra/research/ResearchService.h"
#include <drogon/HttpController.h>
#include <json/json.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include <future>

namespace sra::api {

void ResearchController::researchCompany(
    const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) 
{
    // 1. Validate HTTP Content-Type header
    if (req->contentType() != drogon::CT_APPLICATION_JSON) {
        Json::Value errJson;
        errJson["error"] = "Invalid Content-Type. Expected 'application/json'.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    // 2. Parse JSON body
    auto jsonPtr = req->getJsonObject();
    if (!jsonPtr) {
        Json::Value errJson;
        errJson["error"] = "Malformed JSON payload.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    // 3. Validate 'company' parameter presence
    if (!jsonPtr->isMember("company") || !(*jsonPtr)["company"].isString()) {
        Json::Value errJson;
        errJson["error"] = "Missing or invalid 'company' string parameter.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string companyName = (*jsonPtr)["company"].asString();

    // 4. Sanitize input (trim whitespace)
    companyName.erase(0, companyName.find_first_not_of(" \t\n\r"));
    companyName.erase(companyName.find_last_not_of(" \t\n\r") + 1);

    if (companyName.empty()) {
        Json::Value errJson;
        errJson["error"] = "'company' parameter cannot be empty.";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    // 5. Run research asynchronously off the main I/O thread
    std::async(std::launch::async, [companyName, callback = std::move(callback)]() {
        try {
            // Match constructor: std::make_unique<OpenAIResearchProvider>
            auto provider = std::make_unique<sra::research::OpenAIResearchProvider>(
                "ollama", "http://127.0.0.1:11434"
            );
            sra::research::ResearchService service(std::move(provider));

            auto result = service.researchCompany(companyName);

            // Build Success JSON Response
            Json::Value responseJson;
            responseJson["status"] = "success";
            responseJson["company"]["name"] = result.company.name;
            responseJson["company"]["industry"] = result.company.industry;
            responseJson["company"]["description"] = result.company.description;
            responseJson["icp_score"] = result.icpScore;
            responseJson["recommendation"] = result.recommendation;

            auto resp = drogon::HttpResponse::newHttpJsonResponse(responseJson);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);

        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Research pipeline failed: " << e.what() << std::endl;

            Json::Value errJson;
            errJson["error"] = "Internal research processing error.";
            errJson["details"] = e.what();

            auto resp = drogon::HttpResponse::newHttpJsonResponse(errJson);
            resp->setStatusCode(drogon::k500InternalServerError);
            callback(resp);
        }
    });
}

} // namespace sra::api
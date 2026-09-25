#include "sra/app/Application.h"
#include "sra/research/DuckDuckGoSearchClient.h"
#include "sra/research/OpenAIResearchProvider.h"
#include "sra/research/WebResearchProvider.h"
#include "sra/research/ResearchService.h"
#include <iostream>
#include <memory>

namespace sra::app {

Application::Application() = default;

std::string Application::name() const {
    return "Sales Research Agent";
}

std::string Application::version() const {
    return "0.1.0";
}

int Application::run() {
    std::cout << "Starting " << name() << " v" << version() << " with Web Research Pipeline..." << std::endl;

    const char* envKey = std::getenv("GROQ_API_KEY");
    std::string apiKey = envKey ? envKey : "";
    std::string apiEndpoint = "https://api.groq.com";

    // 1. Build Search Client & LLM Provider as shared_ptr
    auto searchClient = std::make_shared<sra::research::DuckDuckGoSearchClient>();
    auto llmProvider = std::make_shared<sra::research::OpenAIResearchProvider>(apiKey, apiEndpoint);

    // 2. Build WebResearchProvider as unique_ptr matching ResearchProvider interface
    auto webProvider = std::make_unique<sra::research::WebResearchProvider>(searchClient, llmProvider);

    // 3. Move unique_ptr into ResearchService
    sra::research::ResearchService researchService(std::move(webProvider));

    try {
        std::string companyToResearch = "Acme Corp";
        auto result = researchService.researchCompany(companyToResearch);

        std::cout << "\n================ RESEARCH SUMMARY ================" << std::endl;
        std::cout << "Company: " << result.company.name << std::endl;
        std::cout << "ICP Score: " << result.icpScore << "/100" << std::endl;

        std::cout << "\n[Technology Signals & Evidence]:" << std::endl;
        for (const auto& signal : result.technologySignals) {
            std::cout << " - Category: " << signal.category << std::endl;
            std::cout << "   Description: " << signal.description << std::endl;
            std::cout << "   Source: " << signal.source << " (Confidence: " << signal.confidence << ")" << std::endl;
            
            for (const auto& ev : signal.evidenceList) {
                std::cout << "    * Claim: " << ev.claim << std::endl;
                std::cout << "      URL: " << ev.sourceUrl << std::endl;
                std::cout << "      Snippet: " << ev.extractedSnippet << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Research failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

} // namespace sra::app
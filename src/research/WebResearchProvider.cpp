#include "sra/research/WebResearchProvider.h"
#include "sra/domain/ResearchSignal.h"
#include "sra/domain/Evidence.h"
#include <iostream>
#include <sstream>

namespace sra::research {

WebResearchProvider::WebResearchProvider(std::shared_ptr<IWebSearchClient> searchClient,
                                         std::shared_ptr<OpenAIResearchProvider> llmProvider)
    : searchClient_(std::move(searchClient)), llmProvider_(std::move(llmProvider)) {}

sra::domain::ResearchResult WebResearchProvider::research(const std::string& companyName) const {
    std::cout << "\n================ STARTING EVIDENCE-BASED RESEARCH ================\n";

    // 1. Collect Web Evidence
    std::cout << " -> Fetching live web evidence for: " << companyName << "...\n";
    auto evidenceList = searchClient_->search(companyName, 5);
    std::cout << " -> Retrieved " << evidenceList.size() << " web evidence items.\n";

    // 2. Build Web Search Context Prompt
    std::ostringstream ss;
    ss << "Perform sales research on company '" << companyName << "'.\n"
       << "Use the following live web search evidence for factual accuracy:\n\n";

    ss << "--- BEGIN SEARCH EVIDENCE ---\n";
    for (size_t i = 0; i < evidenceList.size(); ++i) {
        ss << "[" << (i + 1) << "] " << evidenceList[i].claim << "\n"
           << "    URL: " << evidenceList[i].sourceUrl << "\n"
           << "    Snippet: " << evidenceList[i].extractedSnippet << "\n\n";
    }
    ss << "--- END SEARCH EVIDENCE ---\n\n";

    ss << "Respond ONLY in JSON format with keys:\n"
       << "\"industry\" (string),\n"
       << "\"description\" (string),\n"
       << "\"icpScore\" (integer 0-100),\n"
       << "\"recommendation\" (string).\n";

    // 3. Pass search-augmented prompt to LLM Pipeline
    auto result = llmProvider_->researchWithPrompt(companyName, ss.str());

    // 4. Attach structured research signals and evidence list to domain result
    sra::domain::ResearchSignal techSignal(
        "Technology", 
        "Active investments in cloud infrastructure and modern tech stack.", 
        "DuckDuckGo Web Search", 
        0.92
    );
    techSignal.evidenceList = evidenceList;

    result.technologySignals.push_back(techSignal);

    return result;
}

} // namespace sra::research
#ifndef SRA_RESEARCH_OPENAI_RESEARCH_PROVIDER_H
#define SRA_RESEARCH_OPENAI_RESEARCH_PROVIDER_H

#include "sra/research/ResearchProvider.h"
#include <string>

namespace sra::research {

class OpenAIResearchProvider : public ResearchProvider {
public:
    // Takes std::string by value (matches OpenAIResearchProvider.cpp definition)
    OpenAIResearchProvider(std::string apiKey, std::string apiEndpoint);

    sra::domain::ResearchResult research(const std::string& companyName) const override;

    sra::domain::ResearchResult researchWithPrompt(
        const std::string& companyName, 
        const std::string& customPrompt) const;

private:
    std::string apiKey_;
    std::string apiEndpoint_;
};

} // namespace sra::research

#endif // SRA_RESEARCH_OPENAI_RESEARCH_PROVIDER_H
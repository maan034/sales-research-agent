#pragma once

#include "sra/research/ResearchProvider.h"
#include "sra/research/IWebSearchClient.h"
#include "sra/domain/ResearchSignal.h"
#include "sra/domain/Evidence.h"
#include "sra/research/OpenAIResearchProvider.h"
#include <memory>

namespace sra::research {

class WebResearchProvider : public ResearchProvider {
public:
    WebResearchProvider(std::shared_ptr<IWebSearchClient> searchClient,
                        std::shared_ptr<OpenAIResearchProvider> llmProvider);

    sra::domain::ResearchResult research(const std::string& companyName) const override;

private:
    std::shared_ptr<IWebSearchClient> searchClient_;
    std::shared_ptr<OpenAIResearchProvider> llmProvider_;
};

} // namespace sra::research
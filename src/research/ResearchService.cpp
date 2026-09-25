#include "sra/research/ResearchService.h"
#include <stdexcept>
#include <utility>

namespace sra::research {

ResearchService::ResearchService(std::unique_ptr<ResearchProvider> provider)
    : provider_(std::move(provider))
{
    if (!provider_) {
        throw std::invalid_argument("ResearchProvider cannot be null");
    }
}

sra::domain::ResearchResult ResearchService::researchCompany(const std::string& companyName) const {
    if (companyName.empty()) {
        throw std::invalid_argument("Company name cannot be empty");
    }
    return provider_->research(companyName);
}

} // namespace sra::research
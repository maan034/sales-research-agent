#pragma once

#include "sra/research/ResearchProvider.h"
#include "sra/domain/ResearchResult.h"
#include <memory>
#include <string>

namespace sra::research {

class ResearchService {
public:
    explicit ResearchService(std::unique_ptr<ResearchProvider> provider);
    
    sra::domain::ResearchResult researchCompany(const std::string& companyName) const;

private:
    std::unique_ptr<ResearchProvider> provider_;
};

} // namespace sra::research
#pragma once

#include "sra/research/ResearchProvider.h"

namespace sra::research {

class DemoResearchProvider : public ResearchProvider {
public:
    sra::domain::ResearchResult research(const std::string& companyName) const override;
};

} // namespace sra::research
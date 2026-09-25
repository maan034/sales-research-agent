#pragma once

#include "sra/domain/ResearchResult.h"
#include <string>

namespace sra::research {

class ResearchProvider {
public:
    virtual ~ResearchProvider() = default;
    virtual sra::domain::ResearchResult research(const std::string& companyName) const = 0;
};

} // namespace sra::research
#pragma once

#include "sra/domain/Company.h"
#include "sra/domain/ResearchSignal.h"
#include <string>
#include <vector>

namespace sra::domain {

struct ResearchResult {
    Company company;
    std::vector<ResearchSignal> technologySignals; // Must match ResearchSignal
    std::vector<ResearchSignal> businessSignals;   // Must match ResearchSignal
    int icpScore{0};
    std::string recommendation;
};

} // namespace sra::domain
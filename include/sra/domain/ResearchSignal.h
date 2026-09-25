#pragma once

#include "sra/domain/Evidence.h"
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace sra::domain {

struct ResearchSignal {
    std::string category;
    std::string description;
    std::string source;
    double confidence{0.0};
    std::vector<Evidence> evidenceList; // Attached evidence records

    ResearchSignal() = default;

    ResearchSignal(
        std::string categoryValue,
        std::string descriptionValue,
        std::string sourceValue,
        double confidenceValue)
        : category(std::move(categoryValue)),
          description(std::move(descriptionValue)),
          source(std::move(sourceValue)),
          confidence(confidenceValue)
    {
        if (confidence < 0.0 || confidence > 1.0) {
            throw std::invalid_argument(
                "ResearchSignal confidence must be between 0.0 and 1.0");
        }
    }
};

} // namespace sra::domain
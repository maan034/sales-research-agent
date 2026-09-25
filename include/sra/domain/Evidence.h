#pragma once

#include <string>

namespace sra::domain {

struct Evidence {
    std::string sourceUrl;
    std::string title;
    std::string extractedSnippet;
    std::string timestamp;
    double confidence{1.0}; // 0.0 - 1.0
    std::string claim;
};

} // namespace sra::domain
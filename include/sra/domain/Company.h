#pragma once

#include <string>

namespace sra::domain {

struct Company {
    std::string name;
    std::string industry;
    std::string website;
    std::string description;
};

} // namespace sra::domain
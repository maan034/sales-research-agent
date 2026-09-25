#pragma once

#include "sra/config/Config.h"

namespace sra::config {

class ConfigLoader {
public:
    [[nodiscard]]
    static Config load();
};

} // namespace sra::config
#pragma once

#include <string>

namespace sra::config {

struct Config {
    std::string databaseUrl;
    std::string redisUrl;
    std::string llmApiKey;
    int apiPort{8080};
};

} // namespace sra::config
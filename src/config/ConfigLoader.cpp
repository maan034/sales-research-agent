#include "sra/config/ConfigLoader.h"

#include <cstdlib>
#include <string>

namespace sra::config {

namespace {

std::string getEnvironmentVariable(const char* name)
{
#ifdef _WIN32
    char* value = nullptr;
    std::size_t size = 0;

    if (_dupenv_s(&value, &size, name) != 0 || value == nullptr) {
        return {};
    }

    std::string result(value);
    std::free(value);

    return result;
#else
    const char* value = std::getenv(name);

    if (value == nullptr) {
        return {};
    }

    return value;
#endif
}

int getPort()
{
    const std::string value =
        getEnvironmentVariable("SRA_API_PORT");

    if (value.empty()) {
        return 8080;
    }

    try {
        return std::stoi(value);
    }
    catch (...) {
        return 8080;
    }
}

} // namespace

Config ConfigLoader::load()
{
    Config config;

    config.databaseUrl =
        getEnvironmentVariable("SRA_DATABASE_URL");

    config.redisUrl =
        getEnvironmentVariable("SRA_REDIS_URL");

    config.llmApiKey =
        getEnvironmentVariable("SRA_LLM_API_KEY");

    config.apiPort = getPort();

    return config;
}

} // namespace sra::config
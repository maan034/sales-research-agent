#pragma once

#include <string>

namespace sra::app {

class Application {
public:
    Application();
    std::string name() const;
    std::string version() const;
    int run();
};

} // namespace sra::app
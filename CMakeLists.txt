cmake_minimum_required(VERSION 3.25)

project(
    sales_research_agent
    VERSION 0.1.0
    DESCRIPTION "Production-style AI Sales Research Agent"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find packages installed via vcpkg
find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# Application executable
add_executable(sales_research_agent
    src/main.cpp
    src/app/Application.cpp
    src/config/ConfigLoader.cpp
    src/research/ResearchService.cpp
    src/research/DemoResearchProvider.cpp
    src/research/DuckDuckGoSearchClient.cpp
    src/research/WebResearchProvider.cpp
    src/research/OpenAIResearchProvider.cpp
    src/api/ResearchController.cpp
)

target_include_directories(sales_research_agent
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sales_research_agent
    PRIVATE
        Drogon::Drogon
        JsonCpp::JsonCpp
)

# Enable testing
enable_testing()
add_subdirectory(tests)
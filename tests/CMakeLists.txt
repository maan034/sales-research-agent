include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

# For Windows MSVC runtime consistency
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(research_service_test 
    ResearchServiceTest.cpp
    ../src/research/ResearchService.cpp
)

target_include_directories(research_service_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(research_service_test
    PRIVATE
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(research_service_test)
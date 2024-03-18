set(target_name tests)

add_executable(
    ${target_name}
    vector.cpp
)

set_target_properties(
    ${target_name}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    ${target_name}
    PRIVATE
    GTest::gtest_main
)

target_include_directories(
    ${target_name}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
)

include(GoogleTest)
gtest_discover_tests(${target_name})

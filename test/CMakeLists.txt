# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(SelfBalancingTreesVisualizerTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(SelfBalancingTreesVisualizer_test source/SelfBalancingTreesVisualizer_test.cpp)
target_link_libraries(
    SelfBalancingTreesVisualizer_test PRIVATE
    SelfBalancingTreesVisualizer_lib
    Catch2::Catch2WithMain
)
target_compile_features(SelfBalancingTreesVisualizer_test PRIVATE cxx_std_17)

catch_discover_tests(SelfBalancingTreesVisualizer_test)

# ---- End-of-file commands ----

add_folders(Test)

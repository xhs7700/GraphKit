#include "graphkit.h"
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>

int main(int argc, char** argv)
{
    auto cur_path = std::filesystem::current_path();
    fmt::println("current path is {}.", cur_path.string());
    std::filesystem::path in_path("/home/xhs-wsl/workspace/Git/GraphKit/tmp/out.roadNet-CA");
    std::ifstream fin(in_path);
    gkit::SimpleUndiGraph g("roadNet-CA", in_path);
    fmt::println("The size of g is ({}, {})", g.nodeNum(), g.edgeNum());
    return 0;
}

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
    std::ifstream fin("/home/xhs-wsl/workspace/Git/GraphKit/tmp/out.roadNet-CA");
    gkit::UnweightedUndiGraph g("roadNet-CA", "/home/xhs-wsl/workspace/Git/GraphKit/tmp/out.roadNet-CA");
    fmt::println("The size of g is ({}, {})", g.nodeNum(), g.edgeNum());
    gkit::UnweightedUndiGraph gLCC = g.LCC();
    // fmt::println("The size of gLCC is ({}, {})", gLCC.nodeNum(), gLCC.edgeNum());
    return 0;
}

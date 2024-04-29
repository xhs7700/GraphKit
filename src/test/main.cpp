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
    std::filesystem::path in_path("/home/xhs-wsl/workspace/Git/GraphKit/tmp/out.soc-pokec-relationships");
    std::ifstream fin(in_path);
    gkit::WeightedDiGraph g("pokec", in_path, []() { return 1; });
    fmt::println("The size of g is ({}, {})", g.nodeNum(), g.edgeNum());
    gkit::WeightedDiGraph gLSCC = g.LSCC();
    fmt::println("The size of gLSCC is ({}, {})", gLSCC.nodeNum(), gLSCC.edgeNum());
    return 0;
}

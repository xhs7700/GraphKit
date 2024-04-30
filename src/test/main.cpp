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
    std::filesystem::path in_path("/home/xhs-wsl/workspace/Git/GraphKit/tmp/out.p2p-Gnutella30");
    std::filesystem::path out_path("/home/xhs-wsl/workspace/Git/GraphKit/tmp/gnutella30.txt");
    std::ifstream fin(in_path);
    std::ofstream fout(out_path);
    gkit::SimpleDiGraph g("gnutella30", in_path);
    fmt::println("The size of g is ({}, {})", g.nodeNum(), g.edgeNum());
    fout << g;
    return 0;
}

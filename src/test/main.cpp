#include "graphkit.h"
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

void GraphConstructTest()
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
}

void KonectTest()
{
    std::string internalName = "p2p-Gnutella30";
    auto g = gkit::LoadSimpleDiKonect(internalName, "Gnutella30");
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "Gnutella30.txt");
    fout << g;
}

void SnapTest()
{
    std::string url = "https://snap.stanford.edu/data/facebook_combined.txt.gz";
    auto g = gkit::LoadSimpleUndiSnap(url, "facebook");
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "facebook.txt");
    fout << g;
}

void SyntheticTest()
{
    auto g = gkit::LoadApollo(14);
    fmt::println("size of g is ({}, {}).", g.nodeNum(), g.edgeNum());
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / fmt::format("{}.txt", g.name));
    fout << g;
}

void SignedKonectTest(const std::string& internalName, std::ofstream& fout, int lineNum)
{
    auto path = gkit::GetKonectPath(internalName);
    std::ifstream fin(path);
    std::string desc = fmt::format("{} lines of {}:\n", lineNum, internalName);
    std::string line;
    fout << desc;
    for (int _ = 0; _ < lineNum; _++) {
        std::getline(fin, line);
        fout << line << std::endl;
    }
}

void SignedKonectTest(std::vector<std::string> internalNames, int lineNum)
{
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "SignedTest.txt");
    for (const auto& internalName : internalNames)
        SignedKonectTest(internalName, fout, lineNum);
}

int main(int argc, char** argv)
{
    return 0;
}

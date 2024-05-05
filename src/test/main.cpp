#include "graphkit.h"
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <string>

void KonectTest()
{
    std::string internalName = "p2p-Gnutella30";
    gkit::SimpleDiGraph g = gkit::LoadKonect<gkit::SimpleDiGraph>(internalName, "Gnutella30");
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "Gnutella30.txt");
    fout << g;
}

void SnapTest()
{
    std::string url = "https://snap.stanford.edu/data/facebook_combined.txt.gz";
    gkit::SimpleUndiGraph g = gkit::LoadSnap<gkit::SimpleUndiGraph>(url, "facebook");
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

void SignedTest()
{
    std::string internalName("convote");
    gkit::SignedDiGraph g = gkit::LoadKonect<gkit::SignedDiGraph>(internalName, "Congress votes");
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "convote.txt");
    fout << g;
}

int main(int argc, char** argv)
{
    return 0;
}

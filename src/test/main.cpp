#include "graphkit.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
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
    std::cout << std::format("size of g is ({}, {}).\n", g.nodeNum(), g.edgeNum());
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / std::format("{}.txt", g.name));
    fout << g;
}

void SignedTest()
{
    std::string internalName("convote");
    gkit::SignedDiGraph g = gkit::LoadKonect<gkit::SignedDiGraph>(internalName, "Congress votes");
    std::ofstream fout(std::filesystem::path(PROJECT_DIR) / "tmp" / "convote.txt");
    fout << g;
}

void ExpansionTest()
{
    auto filePath = std::filesystem::path(PROJECT_DIR) / "tmp" / "toy_raw.txt";
    gkit::SignedUndiGraph g("toy", filePath);
    std::ofstream fout1(std::filesystem::path(PROJECT_DIR) / "tmp" / "toy.txt");
    std::ofstream fout2(std::filesystem::path(PROJECT_DIR) / "tmp" / "toy_expansion.txt");
    fout1 << g;
    fout2 << g.expansion();
}

int main(int argc, char** argv)
{
    return 0;
}

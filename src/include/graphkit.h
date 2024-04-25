#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace gkit {
struct GeneralGraph {
    std::string name;
    std::unordered_set<int> nodes;
    std::unordered_map<std::pair<int, int>, int> edges;
    GeneralGraph(std::string name, std::unordered_set<int> nodes, std::unordered_map<std::pair<int, int>, int> edges);
    int NodeNum();
    int EdgeNum();
    void AddNode(int u);
    void AddEdge(int u, int v, int w);
    GeneralGraph LCC();
    void ReserveToLCC();
};

std::filesystem::path GetKonectPath(const std::string& internal_name);
}

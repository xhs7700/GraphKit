#include <cstddef>
#include <filesystem>
#include <istream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gkit {

struct UnweightedUndiGraph {
    std::string name;
    std::unordered_set<int> nodes;
    std::set<std::pair<int, int>> edges;
    UnweightedUndiGraph(std::string name, std::unordered_set<int> nodes, std::set<std::pair<int, int>> edges)
        : name(name)
        , nodes(nodes)
        , edges(edges)
    {
    }
    UnweightedUndiGraph(std::string name, std::istream& in);
    UnweightedUndiGraph(std::string name, std::filesystem::path source);
    void Init(std::istream& in);
    std::size_t nodeNum() { return nodes.size(); }
    std::size_t edgeNum() { return edges.size(); }
    void addNode(int u);
    void addEdge(int u, int v);
    UnweightedUndiGraph LCC();
};

struct UnweightedDiGraph {
    std::string name;
    std::unordered_map<int, std::vector<int>> adjs;
    std::set<std::pair<int, int>> edges;
    UnweightedDiGraph(std::string name, std::unordered_map<int, std::vector<int>> adjs, std::set<std::pair<int, int>> edges)
        : name(name)
        , adjs(adjs)
        , edges(edges)
    {
    }
    UnweightedDiGraph(std::string name, std::istream& in);
    UnweightedDiGraph(std::string name, std::filesystem::path source);
    void Init(std::istream& in);
    std::size_t nodeNum() { return adjs.size(); }
    std::size_t edgeNum() { return edges.size(); }
    void addNode(int u);
    void addEdge(int u, int v);
    UnweightedDiGraph LSCC();
};

struct WeightedUndiGraph {
    std::string name;
    std::unordered_set<int> nodes;
    std::map<std::pair<int, int>, int> edges;
    WeightedUndiGraph(std::string name, std::unordered_set<int> nodes, std::map<std::pair<int, int>, int> edges)
        : name(name)
        , nodes(nodes)
        , edges(edges)
    {
    }
    WeightedUndiGraph(std::string name, std::istream& in);
    WeightedUndiGraph(std::string name, std::filesystem::path source);
    void Init(std::istream& in);
    template <typename Func>
    WeightedUndiGraph(std::string name, std::istream& in, Func generator);
    template <typename Func>
    WeightedUndiGraph(std::string name, std::filesystem::path source, Func generator);
    template <typename Func>
    void Init(std::istream& in, Func generator);
    std::size_t nodeNum() { return nodes.size(); }
    std::size_t edgeNum() { return edges.size(); }
    void addNode(int u);
    void addEdge(int u, int v, int w);
    WeightedUndiGraph LCC();
};

struct WeightedDiGraph {
    std::string name;
    std::unordered_map<int, std::vector<std::pair<int, int>>> adjs;
    std::map<std::pair<int, int>, int> edges;
    WeightedDiGraph(std::string name, std::unordered_map<int, std::vector<std::pair<int, int>>> adjs, std::map<std::pair<int, int>, int> edges)
        : name(name)
        , adjs(adjs)
        , edges(edges)
    {
    }
    WeightedDiGraph(std::string name, std::istream& in);
    WeightedDiGraph(std::string name, std::filesystem::path source);
    template <typename Func>
    WeightedDiGraph(std::string name, std::istream& in, Func generator);
    template <typename Func>
    WeightedDiGraph(std::string name, std::filesystem::path source, Func generator);
    std::size_t nodeNum() { return adjs.size(); }
    std::size_t edgeNum() { return edges.size(); }
    void addNode(int u);
    void addEdge(int u, int v, int w);
    WeightedDiGraph LSCC();
};

struct SimpleUndiGraph {
    std::size_t n, m;
    std::string name;
    std::vector<std::vector<int>> adjs;
    SimpleUndiGraph(std::size_t n, std::size_t m, std::string name, std::vector<std::vector<int>> adjs)
        : n(n)
        , m(m)
        , name(name)
        , adjs(adjs)
    {
    }
    SimpleUndiGraph(UnweightedUndiGraph g);
    SimpleUndiGraph(std::string name, std::istream& in);
    SimpleUndiGraph(std::string name, std::filesystem::path source);
    std::size_t nodeNum() { return n; }
    std::size_t edgeNum() { return m; }
};

struct SimpleDiGraph {
    std::size_t n, m;
    std::string name;
    std::vector<std::vector<int>> adjs;
    SimpleDiGraph(std::size_t n, std::size_t m, std::string name, std::vector<std::vector<int>> adjs)
        : n(n)
        , m(m)
        , name(name)
        , adjs(adjs)
    {
    }
    SimpleDiGraph(UnweightedDiGraph g);
    SimpleDiGraph(std::string name, std::istream& in);
    SimpleDiGraph(std::string name, std::filesystem::path source);
    std::size_t nodeNum() { return n; }
    std::size_t edgeNum() { return m; }
};

std::filesystem::path GetKonectPath(const std::string& internal_name);
UnweightedUndiGraph LoadUnweightedUndiKonect(std::string internal_name, std::string name);
UnweightedUndiGraph LoadUnweightedUndiKonect(std::string internal_name);
UnweightedUndiGraph LoadUnweightedUndiSnap(std::string url, std::string name);
UnweightedUndiGraph LoadUnweightedUndiSnap(std::string url);

WeightedUndiGraph LoadWeightedUndiKonect(std::string internal_name, std::string name);
WeightedUndiGraph LoadWeightedUndiKonect(std::string internal_name);
WeightedUndiGraph LoadWeightedUndiSnap(std::string url, std::string name);
WeightedUndiGraph LoadWeightedUndiSnap(std::string url);

SimpleUndiGraph LoadSimpleUndiKonect(std::string internal_name, std::string name);
SimpleUndiGraph LoadSimpleUndiKonect(std::string internal_name);
SimpleUndiGraph LoadSimpleUndiSnap(std::string url, std::string name);
SimpleUndiGraph LoadSimpleUndiSnap(std::string url);

UnweightedDiGraph LoadUnweightedDiKonect(std::string internal_name, std::string name);
UnweightedDiGraph LoadUnweightedDiKonect(std::string internal_name);
UnweightedDiGraph LoadUnweightedDiSnap(std::string url, std::string name);
UnweightedDiGraph LoadUnweightedDiSnap(std::string url);

WeightedDiGraph LoadWeightedDiKonect(std::string internal_name, std::string name);
WeightedDiGraph LoadWeightedDiKonect(std::string internal_name);
WeightedDiGraph LoadWeightedDiSnap(std::string url, std::string name);
WeightedDiGraph LoadWeightedDiSnap(std::string url);

SimpleDiGraph LoadSimpleDiKonect(std::string internal_name, std::string name);
SimpleDiGraph LoadSimpleDiKonect(std::string internal_name);
SimpleDiGraph LoadSimpleDiSnap(std::string url, std::string name);
SimpleDiGraph LoadSimpleDiSnap(std::string url);
}

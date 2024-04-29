#include <filesystem>
#include <functional>
#include <istream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gkit {
using node_t = std::uint64_t;
using weight_t = std::int64_t;
struct UnweightedUndiGraph {
    std::string name;
    std::unordered_set<node_t> nodes;
    std::set<std::pair<node_t, node_t>> edges;
    UnweightedUndiGraph(std::string&& name, std::unordered_set<node_t>&& nodes, std::set<std::pair<node_t, node_t>>&& edges)
        : name(name)
        , nodes(nodes)
        , edges(edges)
    {
    }
    UnweightedUndiGraph(std::string&& name, std::istream& in);
    UnweightedUndiGraph(std::string&& name, std::filesystem::path& source);
    void Init(std::istream& in);
    node_t nodeNum() { return nodes.size(); }
    node_t edgeNum() { return edges.size(); }
    void addNode(node_t u);
    void addEdge(node_t u, node_t v);
    UnweightedUndiGraph LCC();
};

struct UnweightedDiGraph {
    std::string name;
    std::unordered_map<node_t, std::vector<node_t>> adjs;
    std::set<std::pair<node_t, node_t>> edges;
    UnweightedDiGraph(std::string&& name, std::unordered_map<node_t, std::vector<node_t>>&& adjs, std::set<std::pair<node_t, node_t>>&& edges)
        : name(name)
        , adjs(adjs)
        , edges(edges)
    {
    }
    UnweightedDiGraph(std::string&& name, std::istream& in);
    UnweightedDiGraph(std::string&& name, std::filesystem::path& source);
    void Init(std::istream& in);
    node_t nodeNum() { return adjs.size(); }
    node_t edgeNum() { return edges.size(); }
    void addNode(node_t u);
    void addEdge(node_t u, node_t v);
    UnweightedDiGraph LSCC();
};

struct WeightedUndiGraph {
    std::string name;
    std::unordered_set<node_t> nodes;
    std::map<std::pair<node_t, node_t>, weight_t> edges;
    WeightedUndiGraph(std::string&& name, std::unordered_set<node_t>&& nodes, std::map<std::pair<node_t, node_t>, weight_t>&& edges)
        : name(name)
        , nodes(nodes)
        , edges(edges)
    {
    }
    WeightedUndiGraph(std::string&& name, std::istream& in);
    WeightedUndiGraph(std::string&& name, std::filesystem::path& source);
    WeightedUndiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator);
    WeightedUndiGraph(std::string&& name, std::filesystem::path& source, std::function<weight_t()>&& generator);
    void Init(std::istream& in);
    void Init(std::istream& in, std::function<weight_t()>&& generator);
    node_t nodeNum() { return nodes.size(); }
    node_t edgeNum() { return edges.size(); }
    void addNode(node_t u);
    void addEdge(node_t u, node_t v, weight_t w);
    WeightedUndiGraph LCC();
};

struct WeightedDiGraph {
    std::string name;
    std::unordered_map<node_t, std::vector<std::pair<node_t, weight_t>>> adjs;
    std::map<std::pair<node_t, node_t>, weight_t> edges;
    WeightedDiGraph(std::string&& name, std::unordered_map<node_t, std::vector<std::pair<node_t, weight_t>>>&& adjs, std::map<std::pair<node_t, node_t>, weight_t>&& edges)
        : name(name)
        , adjs(adjs)
        , edges(edges)
    {
    }
    WeightedDiGraph(std::string&& name, std::istream& in);
    WeightedDiGraph(std::string&& name, std::filesystem::path& source);
    WeightedDiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator);
    WeightedDiGraph(std::string&& name, std::filesystem::path& source, std::function<weight_t()>&& generator);
    void Init(std::istream& in);
    void Init(std::istream& in, std::function<weight_t()>&& generator);
    node_t nodeNum() { return adjs.size(); }
    node_t edgeNum() { return edges.size(); }
    void addNode(node_t u);
    void addEdge(node_t u, node_t v, weight_t w);
    WeightedDiGraph LSCC();
};

struct SimpleUndiGraph {
    node_t n, m;
    std::string name;
    std::vector<std::vector<node_t>> adjs;
    SimpleUndiGraph(node_t n, node_t m, std::string&& name, std::vector<std::vector<node_t>>&& adjs)
        : n(n)
        , m(m)
        , name(name)
        , adjs(adjs)
    {
    }
    SimpleUndiGraph(UnweightedUndiGraph&& g);
    SimpleUndiGraph(std::string&& name, std::istream& in);
    SimpleUndiGraph(std::string&& name, std::filesystem::path& source);
    node_t nodeNum() { return n; }
    node_t edgeNum() { return m; }
};

struct SimpleDiGraph {
    node_t n, m;
    std::string name;
    std::vector<std::vector<node_t>> adjs;
    SimpleDiGraph(node_t n, node_t m, std::string&& name, std::vector<std::vector<node_t>>&& adjs)
        : n(n)
        , m(m)
        , name(name)
        , adjs(adjs)
    {
    }
    SimpleDiGraph(UnweightedDiGraph g);
    SimpleDiGraph(std::string&& name, std::istream& in);
    SimpleDiGraph(std::string&& name, std::filesystem::path& source);
    node_t nodeNum() { return n; }
    node_t edgeNum() { return m; }
};

std::filesystem::path GetKonectPath(const std::string& internal_name);
UnweightedUndiGraph LoadUnweightedUndiKonect(std::string& internal_name, std::string&& name);
UnweightedUndiGraph LoadUnweightedUndiKonect(std::string& internal_name);
UnweightedUndiGraph LoadUnweightedUndiSnap(std::string& url, std::string&& name);
UnweightedUndiGraph LoadUnweightedUndiSnap(std::string& url);

WeightedUndiGraph LoadWeightedUndiKonect(std::string& internal_name, std::string&& name);
WeightedUndiGraph LoadWeightedUndiKonect(std::string& internal_name);
WeightedUndiGraph LoadWeightedUndiSnap(std::string& url, std::string&& name);
WeightedUndiGraph LoadWeightedUndiSnap(std::string& url);

SimpleUndiGraph LoadSimpleUndiKonect(std::string& internal_name, std::string&& name);
SimpleUndiGraph LoadSimpleUndiKonect(std::string& internal_name);
SimpleUndiGraph LoadSimpleUndiSnap(std::string& url, std::string&& name);
SimpleUndiGraph LoadSimpleUndiSnap(std::string& url);

UnweightedDiGraph LoadUnweightedDiKonect(std::string& internal_name, std::string&& name);
UnweightedDiGraph LoadUnweightedDiKonect(std::string& internal_name);
UnweightedDiGraph LoadUnweightedDiSnap(std::string& url, std::string&& name);
UnweightedDiGraph LoadUnweightedDiSnap(std::string& url);

WeightedDiGraph LoadWeightedDiKonect(std::string& internal_name, std::string&& name);
WeightedDiGraph LoadWeightedDiKonect(std::string& internal_name);
WeightedDiGraph LoadWeightedDiSnap(std::string& url, std::string&& name);
WeightedDiGraph LoadWeightedDiSnap(std::string& url);

SimpleDiGraph LoadSimpleDiKonect(std::string& internal_name, std::string&& name);
SimpleDiGraph LoadSimpleDiKonect(std::string& internal_name);
SimpleDiGraph LoadSimpleDiSnap(std::string& url, std::string&& name);
SimpleDiGraph LoadSimpleDiSnap(std::string& url);
}

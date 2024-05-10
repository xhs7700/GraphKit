#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <filesystem>
#include <functional>
#include <istream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace gkit {
using node_t = std::uint64_t;
using weight_t = std::int64_t;

struct UnweightedUndiGraph;
struct UnweightedDiGraph;
struct WeightedUndiGraph;
struct WeightedDiGraph;
struct SimpleUndiGraph;
struct SimpleDiGraph;
struct SignedUndiGraph;
struct SignedDiGraph;

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
    UnweightedUndiGraph(const UnweightedUndiGraph& g)
        : name(g.name)
        , nodes(g.nodes)
        , edges(g.edges)
    {
    }
    UnweightedUndiGraph(std::string&& name, std::istream& in);
    UnweightedUndiGraph(std::string&& name, const std::filesystem::path& source);
    void Init(std::istream& in);
    node_t nodeNum() const { return nodes.size(); }
    node_t edgeNum() const { return edges.size(); }
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
    UnweightedDiGraph(const UnweightedDiGraph& g)
        : name(g.name)
        , adjs(g.adjs)
        , edges(g.edges)
    {
    }
    UnweightedDiGraph(std::string&& name, std::istream& in);
    UnweightedDiGraph(std::string&& name, const std::filesystem::path& source);
    void Init(std::istream& in);
    node_t nodeNum() const { return adjs.size(); }
    node_t edgeNum() const { return edges.size(); }
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
    WeightedUndiGraph(const WeightedUndiGraph& g)
        : name(g.name)
        , nodes(g.nodes)
        , edges(g.edges)
    {
    }
    WeightedUndiGraph(std::string&& name, std::istream& in);
    WeightedUndiGraph(std::string&& name, const std::filesystem::path& source);
    WeightedUndiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator);
    WeightedUndiGraph(std::string&& name, const std::filesystem::path& source, std::function<weight_t()>&& generator);
    void Init(std::istream& in);
    void Init(std::istream& in, std::function<weight_t()>&& generator);
    node_t nodeNum() const { return nodes.size(); }
    node_t edgeNum() const { return edges.size(); }
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
    WeightedDiGraph(const WeightedDiGraph& g)
        : name(g.name)
        , adjs(g.adjs)
        , edges(g.edges)
    {
    }
    WeightedDiGraph(std::string&& name, std::istream& in);
    WeightedDiGraph(std::string&& name, const std::filesystem::path& source);
    WeightedDiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator);
    WeightedDiGraph(std::string&& name, const std::filesystem::path& source, std::function<weight_t()>&& generator);
    void Init(std::istream& in);
    void Init(std::istream& in, std::function<weight_t()>&& generator);
    node_t nodeNum() const { return adjs.size(); }
    node_t edgeNum() const { return edges.size(); }
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
    SimpleUndiGraph(const SimpleUndiGraph& g)
        : n(g.n)
        , m(g.m)
        , name(g.name)
        , adjs(g.adjs)
    {
    }
    SimpleUndiGraph(UnweightedUndiGraph&& g);
    SimpleUndiGraph(SignedUndiGraph&& g);
    SimpleUndiGraph(std::string&& name, std::istream& in);
    SimpleUndiGraph(std::string&& name, const std::filesystem::path& source);
    node_t nodeNum() const { return n; }
    node_t edgeNum() const { return m; }
    Eigen::VectorXd degrVec();
    Eigen::SparseMatrix<double> degrMat();
    Eigen::SparseMatrix<double> adjMat();
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
    SimpleDiGraph(const SimpleDiGraph& g)
        : n(g.n)
        , m(g.m)
        , name(g.name)
        , adjs(g.adjs)
    {
    }
    SimpleDiGraph(UnweightedDiGraph&& g);
    SimpleDiGraph(SignedDiGraph&& g);
    SimpleDiGraph(std::string&& name, std::istream& in);
    SimpleDiGraph(std::string&& name, const std::filesystem::path& source);
    node_t nodeNum() const { return n; }
    node_t edgeNum() const { return m; }
    Eigen::VectorXd degrVec();
    Eigen::SparseMatrix<double> degrMat();
    Eigen::SparseMatrix<double> adjMat();
};

struct SignedUndiGraph {
    node_t n, m;
    std::string name;
    std::vector<std::vector<node_t>> posAdjs, negAdjs;
    SignedUndiGraph(node_t n, node_t m, std::string&& name, std::vector<std::vector<node_t>>&& posAdjs, std::vector<std::vector<node_t>>&& negAdjs)
        : n(n)
        , m(m)
        , name(name)
        , posAdjs(posAdjs)
        , negAdjs(negAdjs)
    {
    }
    SignedUndiGraph(const SignedUndiGraph& g)
        : n(g.n)
        , m(g.m)
        , name(g.name)
        , posAdjs(g.posAdjs)
        , negAdjs(g.negAdjs)
    {
    }
    SignedUndiGraph(WeightedUndiGraph&& g);
    SignedUndiGraph(std::string&& name, std::istream& in);
    SignedUndiGraph(std::string&& name, const std::filesystem::path& source);
    node_t nodeNum() const { return n; }
    node_t edgeNum() const { return m; }
    Eigen::VectorXd degrVec();
    Eigen::SparseMatrix<double> degrMat();
    Eigen::SparseMatrix<double> adjMat();
    SimpleUndiGraph expansion();
};

struct SignedDiGraph {
    node_t n, m;
    std::string name;
    std::vector<std::vector<node_t>> posAdjs, negAdjs;
    SignedDiGraph(node_t n, node_t m, std::string&& name, std::vector<std::vector<node_t>>&& posAdjs, std::vector<std::vector<node_t>>&& negAdjs)
        : n(n)
        , m(m)
        , name(name)
        , posAdjs(posAdjs)
        , negAdjs(negAdjs)
    {
    }
    SignedDiGraph(const SignedDiGraph& g)
        : n(g.n)
        , m(g.m)
        , name(g.name)
        , posAdjs(g.posAdjs)
        , negAdjs(g.negAdjs)
    {
    }
    SignedDiGraph(WeightedDiGraph&& g);
    SignedDiGraph(std::string&& name, std::istream& in);
    SignedDiGraph(std::string&& name, const std::filesystem::path& source);
    node_t nodeNum() const { return n; }
    node_t edgeNum() const { return m; }
    Eigen::VectorXd degrVec();
    Eigen::SparseMatrix<double> degrMat();
    Eigen::SparseMatrix<double> adjMat();
    SimpleDiGraph expansion();
};

std::ostream& operator<<(std::ostream& os, const UnweightedUndiGraph& g);
std::ostream& operator<<(std::ostream& os, const WeightedUndiGraph& g);
std::ostream& operator<<(std::ostream& os, const SimpleUndiGraph& g);
std::ostream& operator<<(std::ostream& os, const UnweightedDiGraph& g);
std::ostream& operator<<(std::ostream& os, const WeightedDiGraph& g);
std::ostream& operator<<(std::ostream& os, const SimpleDiGraph& g);
std::ostream& operator<<(std::ostream& os, const SignedUndiGraph& g);
std::ostream& operator<<(std::ostream& os, const SignedDiGraph& g);

std::filesystem::path GetKonectPath(const std::string& internalName);
std::filesystem::path GetSnapPath(const std::string& url);

template <typename T>
T LoadKonect(std::string& internalName, std::string&& name)
{
    const std::filesystem::path source = GetKonectPath(internalName);
    return T(std::move(name), source);
}
template <typename T>
T LoadKonect(std::string& internalName)
{
    std::string name(internalName);
    return LoadKonect<T>(internalName, std::move(name));
}
template <typename T>
T LoadSnap(std::string& url, std::string&& name)
{
    const std::filesystem::path source = GetSnapPath(url);
    return T(std::move(name), source);
}

SimpleUndiGraph LoadPseudoExt(std::uint64_t m, std::uint64_t g);
SimpleUndiGraph LoadPseudo(std::uint64_t g);
SimpleUndiGraph LoadCorona(std::uint64_t q, std::uint64_t g);
SimpleUndiGraph LoadKoch(std::uint64_t g);
SimpleUndiGraph LoadCayleyTree(std::uint64_t b, std::uint64_t g);
SimpleUndiGraph Load3CayleyTree(std::uint64_t g);
SimpleUndiGraph LoadApollo(std::uint64_t g);
}

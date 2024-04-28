#include "graphkit.h"
#include "utils.h"
#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using gkit::node_t, gkit::weight_t;
using std::uint64_t;

struct DSUEntry {
    bool isRoot;
    uint64_t val;
};

struct DSU {
    std::unordered_map<node_t, DSUEntry> parent;
    DSU() = default;
    DSU(gkit::UnweightedUndiGraph& g)
    {
        TickSpinner spinner1("LCC: Adding elements to DSU...", g.nodes.size());
        for (const node_t u : g.nodes) {
            add(u);
            spinner1.tick();
        }
        spinner1.markAsCompleted();
        TickSpinner spinner2("LCC: Performing setUnion...", g.edges.size());
        for (const auto [u, v] : g.edges) {
            setUnion(u, v);
            spinner2.tick();
        }
        spinner2.markAsCompleted();
    }
    DSU(gkit::WeightedUndiGraph& g)
    {
        TickSpinner spinner1("LCC: Adding elements to DSU...", g.nodes.size());
        for (const node_t u : g.nodes) {
            add(u);
            spinner1.tick();
        }
        spinner1.markAsCompleted();
        TickSpinner spinner2("LCC: Performing setUnion...", g.edges.size());
        for (const std::pair<std::pair<node_t, node_t>, weight_t>& p : g.edges) {
            const auto [u, v] = p.first;
            setUnion(u, v);
            spinner2.tick();
        }
        spinner2.markAsCompleted();
    }
    void add(node_t u) { parent.insert({ u, { true, 1ull } }); }
    node_t find(node_t u)
    {
        if (parent[u].isRoot)
            return u;
        node_t f = parent[u].val;
        f = find(f);
        parent[u].val = f;
        return f;
    }
    std::pair<node_t, uint64_t> findAll(node_t u)
    {
        if (parent[u].isRoot)
            return { u, parent[u].val };
        node_t f = parent[u].val;
        uint64_t fNum;
        std::tie(f, fNum) = findAll(f);
        parent[u].val = f;
        return { f, fNum };
    }
    bool setUnion(node_t u, node_t v)
    {
        u = find(u), v = find(v);
        if (u == v)
            return false;
        uint64_t uNum = parent[u].val, vNum = parent[v].val;
        if (uNum < vNum) {
            parent[u] = { false, v };
            parent[v].val = uNum + vNum;
        } else {
            parent[v] = { false, u };
            parent[u].val = uNum + vNum;
        }
        return true;
    }
    node_t maxKey()
    {
        return std::max_element(parent.begin(), parent.end(), [](const std::pair<node_t, DSUEntry>& p1, const std::pair<node_t, DSUEntry>& p2) {
            uint64_t p1Size = p1.second.isRoot ? p1.second.val : 0;
            uint64_t p2Size = p2.second.isRoot ? p2.second.val : 0;
            return p1Size < p2Size;
        })->first;
    }
};

namespace gkit {
UnweightedUndiGraph UnweightedUndiGraph::LCC()
{
    DSU dsu(*this);
    node_t rootLCC = dsu.maxKey();
    std::string newName = fmt::format("{}_LCC", name);
    std::unordered_set<node_t> newNodes;
    std::set<std::pair<node_t, node_t>> newEdges;
    auto inLCC = [&dsu, &rootLCC](node_t u) { return dsu.find(u) == rootLCC; };
    std::copy_if(nodes.cbegin(), nodes.cend(), std::inserter(newNodes, newNodes.end()), inLCC);
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLCC](std::pair<node_t, node_t> e) { return inLCC(e.first) && inLCC(e.second); });
    return UnweightedUndiGraph(std::move(newName), std::move(newNodes), std::move(newEdges));
}
WeightedUndiGraph WeightedUndiGraph::LCC()
{
    DSU dsu(*this);
    node_t rootLCC = dsu.maxKey();
    std::string newName = fmt::format("{}_LCC", name);
    std::unordered_set<node_t> newNodes;
    std::map<std::pair<node_t, node_t>, weight_t> newEdges;
    auto inLCC = [&dsu, &rootLCC](node_t u) { return dsu.find(u) == rootLCC; };
    std::copy_if(nodes.cbegin(), nodes.cend(), std::inserter(newNodes, newNodes.end()), inLCC);
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLCC](std::pair<std::pair<node_t, node_t>, weight_t> p) {
        const auto [u, v] = p.first;
        return inLCC(u) && inLCC(v);
    });
    return WeightedUndiGraph(std::move(newName), std::move(newNodes), std::move(newEdges));
}

SimpleUndiGraph::SimpleUndiGraph(UnweightedUndiGraph&& g)
    : name(std::move(g.name))
{
    name.append("_LCC");
    DSU dsu(g);
    node_t rootLCC = dsu.maxKey();
    auto outOfLCC = [&dsu, &rootLCC](node_t u) { return dsu.find(u) != rootLCC; };
    std::erase_if(g.nodes, outOfLCC);
    std::erase_if(g.edges, [&outOfLCC](std::pair<node_t, node_t> e) { return outOfLCC(e.first) || outOfLCC(e.second); });
    n = g.nodes.size();
    m = g.edges.size();
    std::unordered_map<node_t, node_t> o2n;
    std::vector<node_t> degs(n + 1, 0);
    bool renumber = *std::max_element(g.nodes.begin(), g.nodes.end()) != n;
    TickSpinner spinner1("SimpleUndiGraph: Computing degrees...", m);
    if (renumber) {
        for (const auto [u, v] : g.edges) {
            o2n.try_emplace(u, o2n.size() + 1);
            o2n.try_emplace(v, o2n.size() + 1);
            node_t newU = o2n[u], newV = o2n[v];
            degs[newU]++, degs[newV]++;
            spinner1.tick();
        }
    } else {
        for (const auto [u, v] : g.edges) {
            degs[u]++, degs[v]++;
            spinner1.tick();
        }
    }
    spinner1.markAsCompleted();
    adjs.assign(n + 1, {});
    for (int u = 1; u <= n; u++)
        adjs[u].reserve(degs[u]);
    TickSpinner spinner2("SimpleUndiGraph: Computing adjacency list...", m);
    if (renumber) {
        for (const auto [u, v] : g.edges) {
            node_t newU = o2n[u], newV = o2n[v];
            adjs[newU].push_back(newV), adjs[newV].push_back(newU);
            spinner2.tick();
        }
    } else {
        for (const auto [u, v] : g.edges) {
            adjs[u].push_back(v), adjs[v].push_back(u);
            spinner2.tick();
        }
    }
    spinner2.markAsCompleted();
    for (int u = 1; u <= n; u++)
        std::sort(adjs[u].begin(), adjs[u].end());
    std::unordered_set<node_t> nodes;
    std::set<std::pair<node_t, node_t>> edges;
    nodes.swap(g.nodes);
    edges.swap(g.edges);
}
SimpleUndiGraph::SimpleUndiGraph(std::string&& name, std::istream& in)
    : SimpleUndiGraph(UnweightedUndiGraph(std::move(name), in))
{
}
SimpleUndiGraph::SimpleUndiGraph(std::string&& name, std::filesystem::path& source)
    : SimpleUndiGraph(UnweightedUndiGraph(std::move(name), source))
{
}
}
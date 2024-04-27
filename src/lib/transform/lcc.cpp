#include "graphkit.h"
#include "utils.h"
#include <algorithm>
#include <cstdint>
#include <fmt/core.h>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <utility>

using std::uint64_t, gkit::node_t;

struct DSUEntry {
    bool isRoot;
    uint64_t val;
};

struct DSU {
    std::unordered_map<node_t, DSUEntry> parent;
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
};

namespace gkit {
UnweightedUndiGraph UnweightedUndiGraph::LCC()
{
    DSU dsu;
    TickSpinner spinner1("LCC: Adding elements to DSU...", nodes.size());
    for (const node_t u : nodes) {
        dsu.add(u);
        spinner1.tick();
    }
    spinner1.markAsCompleted();
    TickSpinner spinner2("LCC: Performing setUnion...", edges.size());
    for (const auto [u, v] : edges) {
        dsu.setUnion(u, v);
        spinner2.tick();
    }
    spinner2.markAsCompleted();
    node_t rootLCC = std::max_element(dsu.parent.begin(), dsu.parent.end(), [](const std::pair<node_t, DSUEntry>& p1, const std::pair<node_t, DSUEntry>& p2) {
        uint64_t p1Size = p1.second.isRoot ? p1.second.val : 0;
        uint64_t p2Size = p2.second.isRoot ? p2.second.val : 0;
        return p1Size < p2Size;
    })->first;
    std::string newName = fmt::format("{}_LCC", name);
    std::unordered_set<node_t> newNodes;
    std::set<std::pair<node_t, node_t>> newEdges;
    auto inLCC = [&dsu, &rootLCC](node_t u) { return dsu.find(u) == rootLCC; };
    std::copy_if(nodes.cbegin(), nodes.cend(), std::inserter(newNodes, newNodes.end()), inLCC);
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLCC](std::pair<node_t, node_t> e) { return inLCC(e.first) && inLCC(e.second); });
    return UnweightedUndiGraph(newName, newNodes, newEdges);
}
WeightedUndiGraph WeightedUndiGraph::LCC()
{
    DSU dsu;
    TickSpinner spinner1("LCC: Adding elements to DSU...", nodes.size());
    for (const node_t u : nodes) {
        dsu.add(u);
        spinner1.tick();
    }
    spinner1.markAsCompleted();
    TickSpinner spinner2("LCC: Performing setUnion...", edges.size());
    for (const std::pair<std::pair<node_t, node_t>, weight_t>& p : edges) {
        const auto [u, v] = p.first;
        dsu.setUnion(u, v);
        spinner2.tick();
    }
    spinner2.markAsCompleted();
    node_t rootLCC = std::max_element(dsu.parent.begin(), dsu.parent.end(), [](const std::pair<node_t, DSUEntry>& p1, const std::pair<node_t, DSUEntry>& p2) {
        uint64_t p1Size = p1.second.isRoot ? p1.second.val : 0;
        uint64_t p2Size = p2.second.isRoot ? p2.second.val : 0;
        return p1Size < p2Size;
    })->first;
    std::string newName = fmt::format("{}_LCC", name);
    std::unordered_set<node_t> newNodes;
    std::map<std::pair<node_t, node_t>, weight_t> newEdges;
    auto inLCC = [&dsu, &rootLCC](node_t u) { return dsu.find(u) == rootLCC; };
    std::copy_if(nodes.cbegin(), nodes.cend(), std::inserter(newNodes, newNodes.end()), inLCC);
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLCC](std::pair<std::pair<node_t, node_t>, weight_t> p) {
        const auto [u, v] = p.first;
        return inLCC(u) && inLCC(v);
    });
    return WeightedUndiGraph(newName, newNodes, newEdges);
}
}
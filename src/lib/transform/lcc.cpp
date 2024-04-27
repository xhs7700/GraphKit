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
    uint64_t lccSize = 0;
    for (const node_t u : nodes) {
        if (!dsu.parent[u].isRoot)
            continue;
        lccSize = std::max(lccSize, dsu.parent[u].val);
    }
    fmt::println("lccSize = {}", lccSize);
    // std::string newName = fmt::format("{}_LCC", name);
    // std::unordered_set<node_t> newNodes;
    // std::set<std::pair<node_t, node_t>> newEdges;
    // auto inLCC = [&dsu](node_t u) { return dsu.parent[u].isRoot; };
    // std::copy_if(nodes.cbegin(), nodes.cend(), std::inserter(newNodes, newNodes.end()), inLCC);
    // std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLCC](std::pair<node_t, node_t> e) { return inLCC(e.first) && inLCC(e.second); });
    // return UnweightedUndiGraph(newName, newNodes, newEdges);
    return UnweightedUndiGraph("", {}, {});
}
}
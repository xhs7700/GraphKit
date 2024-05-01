#include "graphkit.h"
#include "utils.h"
#include <algorithm>
#include <fmt/core.h>
#include <iterator>
#include <numeric>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace gkit {

std::uint64_t ratioPower(std::uint64_t init, std::uint64_t base, std::uint64_t exp)
{
    for (std::uint64_t _ = 0; _ < exp; _++)
        init *= base;
    return init;
}

SimpleUndiGraph LoadPseudoExt(std::uint64_t m, std::uint64_t g, std::string&& name)
{
    node_t edgeNum = ratioPower(3, m << 1 | 1, g);
    std::vector<std::pair<node_t, node_t>> edges;
    edges.reserve(edgeNum);
    edges.insert(edges.end(), { { 1, 2 }, { 1, 3 }, { 2, 3 } });
    node_t n = 3;
    for (std::uint64_t _ = 0; _ < g; _++) {
        std::vector<std::pair<node_t, node_t>> newEdges;
        newEdges.reserve(2 * m * edges.size());
        for (std::uint64_t _ = 0; _ < m; _++) {
            for (const auto& [u, v] : edges) {
                n++;
                newEdges.emplace_back(u, n), newEdges.emplace_back(v, n);
            }
        }
        std::ranges::move(newEdges, std::back_inserter(edges));
    }
    std::vector<std::vector<node_t>> adjs(n + 1, std::vector<node_t> {});
    for (const auto& [u, v] : edges) {
        adjs[u].push_back(v), adjs[v].push_back(u);
    }
    return SimpleUndiGraph(n, edges.size(), std::move(name), std::move(adjs));
}
SimpleUndiGraph LoadPseudoExt(std::uint64_t m, std::uint64_t g) { return LoadPseudoExt(m, g, fmt::format("PseudoExt_{}_{}", m, g)); }
SimpleUndiGraph LoadPseudo(std::uint64_t g) { return LoadPseudoExt(1, g, fmt::format("Pseudofractal_{}", g)); }
SimpleUndiGraph LoadKoch(std::uint64_t g)
{
    std::vector<std::tuple<node_t, node_t, node_t>> triangles { { 1, 2, 3 } };
    node_t n = 3;
    for (node_t _ = 0; _ < g; _++) {
        std::vector<std::tuple<node_t, node_t, node_t>> newTri;
        newTri.reserve(3 * triangles.size());
        for (const auto& [x, y, z] : triangles) {
            newTri.insert(newTri.end(), { { x, n + 1, n + 2 }, { y, n + 3, n + 4 }, { z, n + 5, n + 6 } });
            n += 6;
        }
        triangles.reserve(triangles.size() + newTri.size());
        std::ranges::move(newTri, std::back_inserter(triangles));
    }
    std::vector<std::vector<node_t>> adjs(n + 1, std::vector<node_t> {});
    for (const auto& [x, y, z] : triangles) {
        adjs[x].insert(adjs[x].end(), { y, z });
        adjs[y].insert(adjs[y].end(), { x, z });
        adjs[z].insert(adjs[z].end(), { x, y });
    }
    return SimpleUndiGraph(n, 3 * triangles.size(), fmt::format("Koch_{}", g), std::move(adjs));
}
SimpleUndiGraph LoadCayleyTree(std::uint64_t b, std::uint64_t g, std::string&& name)
{
    std::vector<std::pair<node_t, node_t>> edges;
    std::vector<node_t> leafs(b);
    std::iota(leafs.begin(), leafs.end(), 2);
    std::ranges::for_each(leafs, [&edges](const node_t& leaf) { edges.emplace_back(1, leaf); });
    node_t n = b + 1;
    for (std::uint64_t _ = 1; _ < g; _++) {
        std::vector<node_t> newLeafs;
        newLeafs.reserve((b - 1) * leafs.size());
        for (const node_t& leaf : leafs) {
            for (node_t i = 1; i < b; i++) {
                node_t newLeaf = n + i;
                edges.emplace_back(leaf, newLeaf);
                newLeafs.push_back(newLeaf);
            }
            n += b - 1;
        }
        leafs.swap(newLeafs);
    }
    std::vector<std::vector<node_t>> adjs(n + 1, std::vector<node_t> {});
    for (const auto& [u, v] : edges) {
        adjs[u].push_back(v), adjs[v].push_back(u);
    }
    return SimpleUndiGraph(n, edges.size(), std::move(name), std::move(adjs));
}
SimpleUndiGraph LoadCayleyTree(std::uint64_t b, std::uint64_t g) { return LoadCayleyTree(b, g, fmt::format("{}CayleyTree_{}", b, g)); }
SimpleUndiGraph Load3CayleyTree(std::uint64_t g) { return LoadCayleyTree(3, g); }
SimpleUndiGraph LoadApollo(std::uint64_t g)
{
    std::vector<std::tuple<node_t, node_t, node_t>> triangles { { 1, 2, 3 }, { 1, 2, 4 }, { 1, 3, 4 }, { 2, 3, 4 } };
    std::vector<std::tuple<node_t, node_t, node_t>> activTri(triangles);
    std::vector<std::vector<node_t>> adjs { {}, { 2, 3, 4 }, { 1, 3, 4 }, { 1, 2, 4 }, { 1, 2, 3 } };
    node_t n = 4, m = 6;
    for (std::uint64_t _ = 0; _ < g; _++) {
        std::vector<std::tuple<node_t, node_t, node_t>> newTri;
        newTri.reserve(3 * activTri.size());
        for (const auto& [x, y, z] : activTri) {
            n++;
            newTri.insert(newTri.end(), { { x, y, n }, { x, z, n }, { y, z, n } });
            adjs[x].push_back(n), adjs[y].push_back(n), adjs[z].push_back(n);
            adjs.push_back({ x, y, z });
        }
        m += newTri.size();
        triangles.reserve(triangles.size() + newTri.size());
        std::ranges::copy(newTri, std::back_inserter(triangles));
        activTri.swap(newTri);
    }
    return SimpleUndiGraph(n, m, fmt::format("Apollo_{}", g), std::move(adjs));
}
}
#include "graphkit.h"
#include <algorithm>
#include <iterator>
#include <utility>

namespace gkit {
SimpleUndiGraph::SimpleUndiGraph(SignedUndiGraph&& g)
{
    SimpleUndiGraph expansG = g.expansion();
    n = expansG.n, m = expansG.m;
    name.swap(expansG.name);
    adjs.swap(expansG.adjs);
    std::string nullName;
    std::vector<std::vector<node_t>> posAdjs, negAdjs;
    nullName.swap(g.name);
    posAdjs.swap(g.posAdjs), negAdjs.swap(g.negAdjs);
}
SimpleDiGraph::SimpleDiGraph(SignedDiGraph&& g)
{
    SimpleDiGraph expansG = g.expansion();
    n = expansG.n, m = expansG.m;
    name.swap(expansG.name);
    adjs.swap(expansG.adjs);
    std::string nullName;
    std::vector<std::vector<node_t>> posAdjs, negAdjs;
    nullName.swap(g.name);
    posAdjs.swap(g.posAdjs), negAdjs.swap(g.negAdjs);
}

SimpleUndiGraph SignedUndiGraph::expansion()
{
    std::string newName(name);
    std::vector<std::vector<node_t>> adjs(n << 1);
    newName.append("_expansion");
    const node_t nodeNum = n;
    auto duplic = [&nodeNum](const node_t& i) { return i + nodeNum; };
    for (node_t u = 0; u < nodeNum; u++) {
        adjs[u].reserve(posAdjs[u].size() + negAdjs[u].size());
        std::ranges::copy(posAdjs[u], std::back_inserter(adjs[u]));
        std::ranges::transform(negAdjs[u], std::back_inserter(adjs[u]), duplic);
        const node_t dupU = u + nodeNum;
        adjs[dupU].reserve(posAdjs[u].size() + negAdjs[u].size());
        std::ranges::copy(negAdjs[u], std::back_inserter(adjs[dupU]));
        std::ranges::transform(posAdjs[u], std::back_inserter(adjs[dupU]), duplic);
    }
    return SimpleUndiGraph(n << 1, m << 1, std::move(newName), std::move(adjs));
}
SimpleDiGraph SignedDiGraph::expansion()
{
    std::string newName(name);
    std::vector<std::vector<node_t>> adjs(n << 1);
    newName.append("_expansion");
    const node_t nodeNum = n;
    auto duplic = [&nodeNum](const node_t& i) { return i + nodeNum; };
    for (node_t u = 0; u < nodeNum; u++) {
        adjs[u].reserve(posAdjs[u].size() + negAdjs[u].size());
        std::ranges::copy(posAdjs[u], std::back_inserter(adjs[u]));
        std::ranges::transform(negAdjs[u], std::back_inserter(adjs[u]), duplic);
        const node_t dupU = u + nodeNum;
        adjs[dupU].reserve(posAdjs[u].size() + negAdjs[u].size());
        std::ranges::copy(negAdjs[u], std::back_inserter(adjs[dupU]));
        std::ranges::transform(posAdjs[u], std::back_inserter(adjs[dupU]), duplic);
    }
    return SimpleDiGraph(n << 1, m << 1, std::move(newName), std::move(adjs));
}
}
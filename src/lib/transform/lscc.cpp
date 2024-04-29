#include "graphkit.h"
#include "utils.h"
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using gkit::node_t, gkit::weight_t;

struct TarjanImpl {
    std::unordered_map<node_t, node_t> dfn, low, sccID;
    std::unordered_set<node_t> inStk, outOfDfsStk, vis;
    std::vector<std::pair<node_t, node_t>> dfsStk;
    std::vector<node_t> stk, sccSizes { 0 };
    node_t dfnCnt = 0ull, sccCnt = 0ull;
    void addToStk(const node_t u)
    {
        vis.insert(u);
        dfn[u] = low[u] = ++dfnCnt;
        stk.push_back(u);
        inStk.insert(u);
    }
    void checkLSCC(node_t pre, node_t u)
    {
        dfsStk.pop_back();
        if (outOfDfsStk.contains(u))
            return;
        outOfDfsStk.insert(u);
        if (pre)
            low[pre] = std::min(low[pre], low[u]);
        if (dfn[u] == low[u]) {
            sccCnt++;
            node_t sccSize = 0, v;
            do {
                v = stk.back();
                stk.pop_back(), inStk.erase(v);
                sccID[v] = sccCnt;
                sccSize++;
            } while (v != u);
            sccSizes.push_back(sccSize);
        }
    }
    TarjanImpl(gkit::UnweightedDiGraph& g)
    {
        TickSpinner spinner("LSCC: Performing Tarjan algorithm...", g.nodeNum());
        for (const auto& p : g.adjs) {
            const node_t i = p.first;
            if (vis.contains(i))
                continue;
            dfsStk.push_back({ 0, i });
            while (!dfsStk.empty()) {
                const auto [pre, u] = dfsStk.back();
                if (!vis.contains(u)) {
                    addToStk(u);
                    spinner.tick();
                    for (const node_t& v : g.adjs[u]) {
                        if (!vis.contains(v))
                            dfsStk.push_back({ u, v });
                        else if (inStk.contains(v))
                            low[u] = std::min(low[u], dfn[v]);
                    }
                } else
                    checkLSCC(pre, u);
            }
        }
        spinner.markAsCompleted();
    }
    TarjanImpl(gkit::WeightedDiGraph& g)
    {
        TickSpinner spinner("LSCC: Performing Tarjan algorithm...", g.nodeNum());
        for (const auto& p : g.adjs) {
            const node_t i = p.first;
            if (vis.contains(i))
                continue;
            dfsStk.push_back({ 0, i });
            while (!dfsStk.empty()) {
                const auto [pre, u] = dfsStk.back();
                if (!vis.contains(u)) {
                    addToStk(u);
                    spinner.tick();
                    for (const auto& p : g.adjs[u]) {
                        node_t v = p.first;
                        if (!vis.contains(v))
                            dfsStk.push_back({ u, v });
                        else if (inStk.contains(v))
                            low[u] = std::min(low[u], dfn[v]);
                    }
                } else
                    checkLSCC(pre, u);
            }
        }
        spinner.markAsCompleted();
    }
    node_t maxKey() { return std::distance(sccSizes.begin(), std::max_element(sccSizes.begin(), sccSizes.end())); }
};

namespace gkit {
UnweightedDiGraph UnweightedDiGraph::LSCC()
{
    TarjanImpl tarjanImpl(*this);
    node_t rootLSCC = tarjanImpl.maxKey();
    std::string newName(name);
    newName.append("_LSCC");
    std::unordered_map<node_t, std::vector<node_t>> newAdjs;
    std::set<std::pair<node_t, node_t>> newEdges;
    auto inLSCC = [&tarjanImpl, &rootLSCC](node_t u) { return tarjanImpl.sccID[u] == rootLSCC; };
    TickSpinner spinner("LSCC: Constructing ans graph...", nodeNum());
    for (const auto& [u, adj] : adjs) {
        spinner.tick();
        if (!inLSCC(u))
            continue;
        newAdjs[u] = {};
        std::copy_if(adj.cbegin(), adj.cend(), std::back_inserter(newAdjs[u]), inLSCC);
    }
    spinner.markAsCompleted();
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLSCC](std::pair<node_t, node_t> e) { return inLSCC(e.first) && inLSCC(e.second); });
    return UnweightedDiGraph(std::move(newName), std::move(newAdjs), std::move(newEdges));
}
WeightedDiGraph WeightedDiGraph::LSCC()
{
    TarjanImpl tarjanImpl(*this);
    node_t rootLSCC = tarjanImpl.maxKey();
    std::string newName(name);
    newName.append("_LSCC");
    std::unordered_map<node_t, std::vector<std::pair<node_t, weight_t>>> newAdjs;
    std::map<std::pair<node_t, node_t>, weight_t> newEdges;
    auto inLSCC = [&tarjanImpl, &rootLSCC](node_t u) { return tarjanImpl.sccID[u] == rootLSCC; };
    TickSpinner spinner("LSCC: Constructing ans graph...", nodeNum());
    for (const auto& [u, adj] : adjs) {
        spinner.tick();
        if (!inLSCC(u))
            continue;
        newAdjs[u] = {};
        std::copy_if(adj.cbegin(), adj.cend(), std::back_inserter(newAdjs[u]), [&inLSCC](std::pair<node_t, weight_t> e) { return inLSCC(e.first); });
    }
    spinner.markAsCompleted();
    std::copy_if(edges.cbegin(), edges.cend(), std::inserter(newEdges, newEdges.end()), [&inLSCC](std::pair<std::pair<node_t, node_t>, weight_t> p) {
        const auto [u, v] = p.first;
        return inLSCC(u) && inLSCC(v);
    });
    return WeightedDiGraph(std::move(newName), std::move(newAdjs), std::move(newEdges));
}
}

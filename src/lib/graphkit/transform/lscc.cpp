#include "graphkit.h"
#include "graphkitutils.h"
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
    node_t maxKey() { return std::distance(sccSizes.begin(), std::ranges::max_element(sccSizes)); }
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
        std::ranges::copy_if(adj, std::back_inserter(newAdjs[u]), inLSCC);
    }
    spinner.markAsCompleted();
    std::ranges::copy_if(edges, std::inserter(newEdges, newEdges.end()), [&inLSCC](std::pair<node_t, node_t> e) { return inLSCC(e.first) && inLSCC(e.second); });
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
        std::ranges::copy_if(adj, std::back_inserter(newAdjs[u]), [&inLSCC](std::pair<node_t, weight_t> e) { return inLSCC(e.first); });
    }
    spinner.markAsCompleted();
    std::ranges::copy_if(edges, std::inserter(newEdges, newEdges.end()), [&inLSCC](std::pair<std::pair<node_t, node_t>, weight_t> p) {
        const auto [u, v] = p.first;
        return inLSCC(u) && inLSCC(v);
    });
    return WeightedDiGraph(std::move(newName), std::move(newAdjs), std::move(newEdges));
}

SimpleDiGraph::SimpleDiGraph(UnweightedDiGraph&& g)
    : name(std::move(g.name))
{
    TarjanImpl tarjanImpl(g);
    node_t rootLSCC = tarjanImpl.maxKey();
    auto outOfLSCC = [&tarjanImpl, &rootLSCC](node_t u) { return tarjanImpl.sccID[u] != rootLSCC; };
    std::erase_if(g.adjs, [&outOfLSCC](std::pair<node_t, std::vector<node_t>> p) { return outOfLSCC(p.first); });
    n = g.nodeNum();
    m = 0ull;
    node_t max_node = 0ull;
    TickSpinner spinner1("SimpleDiGraph: Removing nodes out of LSCC...", n);
    for (auto& [u, adj] : g.adjs) {
        max_node = std::max(max_node, u);
        std::erase_if(adj, outOfLSCC);
        m += adj.size();
        spinner1.tick();
    }
    spinner1.markAsCompleted();
    std::unordered_map<node_t, node_t> o2n;
    adjs.assign(n, {});
    bool renumber = max_node != n - 1;
    TickSpinner spinner2("SimpleDiGraph: Computing adjacency list...", n);
    if (renumber) {
        for (const auto& [u, _] : g.adjs)
            o2n[u] = o2n.size();
        for (auto& [u, adj] : g.adjs) {
            node_t newU = o2n[u];
            std::ranges::transform(adj, adj.begin(), [&o2n](node_t u) { return o2n[u]; });
            std::ranges::sort(adj);
            adjs[newU].swap(adj);
            spinner2.tick();
        }
    } else {
        for (auto& [u, adj] : g.adjs) {
            adjs[u].swap(adj);
            spinner2.tick();
        }
    }
    spinner2.markAsCompleted();
    std::unordered_map<node_t, std::vector<node_t>> nullAdjs;
    std::set<std::pair<node_t, node_t>> edges;
    nullAdjs.swap(g.adjs);
    edges.swap(g.edges);
}
SimpleDiGraph::SimpleDiGraph(std::string&& name, std::istream& in)
    : SimpleDiGraph(UnweightedDiGraph(std::move(name), in))
{
}
SimpleDiGraph::SimpleDiGraph(std::string&& name, const std::filesystem::path& source)
    : SimpleDiGraph(UnweightedDiGraph(std::move(name), source))
{
}

SignedDiGraph::SignedDiGraph(WeightedDiGraph&& g)
    : name(std::move(g.name))
{
    TarjanImpl tarjanImpl(g);
    node_t rootLSCC = tarjanImpl.maxKey();
    auto outOfLSCC = [&tarjanImpl, &rootLSCC](node_t u) { return tarjanImpl.sccID[u] != rootLSCC; };
    std::erase_if(g.adjs, [&outOfLSCC](std::pair<node_t, std::vector<std::pair<node_t, weight_t>>> p) { return outOfLSCC(p.first); });
    n = g.nodeNum();
    m = 0ull;
    node_t max_node = 0ull;
    TickSpinner spinner1("SignedDiGraph: Removing nodes out of LSCC...", n);
    for (auto& [u, adj] : g.adjs) {
        max_node = std::max(max_node, u);
        std::erase_if(adj, [&outOfLSCC](std::pair<node_t, weight_t> p) { return outOfLSCC(p.first); });
        m += adj.size();
        spinner1.tick();
    }
    spinner1.markAsCompleted();
    std::unordered_map<node_t, node_t> o2n;
    posAdjs.assign(n, {}), negAdjs.assign(n, {});
    bool renumber = max_node != n - 1;
    TickSpinner spinner2("SignedDiGraph: Computing adjacency list...", m);
    if (renumber) {
        for (const auto& [u, _] : g.adjs)
            o2n[u] = o2n.size();
        for (const auto& [u, adj] : g.adjs) {
            node_t newU = o2n[u];
            for (const auto& [v, w] : adj) {
                node_t newV = o2n[v];
                if (w > 0)
                    posAdjs[newU].push_back(newV);
                else
                    negAdjs[newU].push_back(newV);
                spinner2.tick();
            }
            std::ranges::sort(posAdjs[newU]);
            std::ranges::sort(negAdjs[newU]);
        }
    } else {
        for (const auto& [u, adj] : g.adjs) {
            for (const auto& [v, w] : adj) {
                if (w > 0)
                    posAdjs[u].push_back(v);
                else
                    negAdjs[u].push_back(v);
                spinner2.tick();
            }
            std::ranges::sort(posAdjs[u]);
            std::ranges::sort(negAdjs[u]);
        }
    }
    spinner2.markAsCompleted();
    std::unordered_map<node_t, std::vector<std::pair<node_t, weight_t>>> adjs;
    std::map<std::pair<node_t, node_t>, weight_t> edges;
    adjs.swap(g.adjs);
    edges.swap(g.edges);
}
SignedDiGraph::SignedDiGraph(std::string&& name, std::istream& in)
    : SignedDiGraph(WeightedDiGraph(std::move(name), in))
{
}
SignedDiGraph::SignedDiGraph(std::string&& name, const std::filesystem::path& source)
    : SignedDiGraph(WeightedDiGraph(std::move(name), source))
{
}
}

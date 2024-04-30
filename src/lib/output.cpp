#include "graphkit.h"
#include "utils.h"
#include <algorithm>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace gkit {
std::ostream& operator<<(std::ostream& os, const UnweightedUndiGraph& g)
{
    std::string desc = fmt::format("# UnweightedUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(fmt::format("Writing UnweightedUndiGraph {}...", g.name), g.edgeNum());
    for (const auto& [u, v] : g.edges) {
        std::string line = fmt::format("{}\t{}\n", u, v);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const WeightedUndiGraph& g)
{
    std::string desc = fmt::format("# WeightedUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(fmt::format("Writing WeightedUndiGraph {}...", g.name), g.edgeNum());
    for (const auto& [e, w] : g.edges) {
        std::string line = fmt::format("{}\t{}\t{}\n", e.first, e.second, w);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SimpleUndiGraph& g)
{
    std::string desc = fmt::format("# SimpleUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.n, g.m);
    os << desc;
    TickSpinner spinner(fmt::format("Writing SimpleUndiGraph {}...", g.name), g.m);
    for (node_t u = 1; u <= g.n; u++) {
        const std::vector<node_t>& adj = g.adjs[u];
        auto it = std::ranges::upper_bound(adj, u);
        std::for_each(it, adj.cend(), [&](const node_t& v) {
            std::string line = fmt::format("{}\t{}\n", u, v);
            os << line;
            spinner.tick();
        });
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const UnweightedDiGraph& g)
{
    std::string desc = fmt::format("# UnweightedDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(fmt::format("Writing UnweightedDiGraph {}...", g.name), g.edgeNum());
    for (const auto& [u, v] : g.edges) {
        std::string line = fmt::format("{}\t{}\n", u, v);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const WeightedDiGraph& g)
{
    std::string desc = fmt::format("# WeightedDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(fmt::format("Writing WeightedDiGraph {}...", g.name), g.edgeNum());
    for (const auto& [e, w] : g.edges) {
        std::string line = fmt::format("{}\t{}\t{}\n", e.first, e.second, w);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SimpleDiGraph& g)
{
    std::string desc = fmt::format("# SimpleDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(fmt::format("Writing SimpleDiGraph {}...", g.name), g.edgeNum());
    for (node_t u = 1; u <= g.n; u++) {
        for (const node_t& v : g.adjs[u]) {
            std::string line = fmt::format("{}\t{}\n", u, v);
            os << line;
            spinner.tick();
        }
    }
    spinner.markAsCompleted();
    return os;
}
}

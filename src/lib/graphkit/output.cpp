#include "graphkit.h"
#include "graphkitutils.h"
#include <algorithm>
#include <format>
#include <string>
#include <vector>

namespace gkit {
std::ostream& operator<<(std::ostream& os, const UnweightedUndiGraph& g)
{
    std::string desc = std::format("# UnweightedUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing UnweightedUndiGraph {}...", g.name), g.edgeNum());
    for (const auto& [u, v] : g.edges) {
        std::string line = std::format("{}\t{}\n", u, v);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const WeightedUndiGraph& g)
{
    std::string desc = std::format("# WeightedUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing WeightedUndiGraph {}...", g.name), g.edgeNum());
    for (const auto& [e, w] : g.edges) {
        std::string line = std::format("{}\t{}\t{}\n", e.first, e.second, w);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SimpleUndiGraph& g)
{
    std::string desc = std::format("# SimpleUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.n, g.m);
    os << desc;
    TickSpinner spinner(std::format("Writing SimpleUndiGraph {}...", g.name), g.m);
    for (node_t u = 0; u < g.n; u++) {
        const std::vector<node_t>& adj = g.adjs[u];
        auto it = std::ranges::upper_bound(adj, u);
        std::for_each(it, adj.cend(), [&](const node_t& v) {
            std::string line = std::format("{}\t{}\n", u, v);
            os << line;
            spinner.tick();
        });
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const UnweightedDiGraph& g)
{
    std::string desc = std::format("# UnweightedDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing UnweightedDiGraph {}...", g.name), g.edgeNum());
    for (const auto& [u, v] : g.edges) {
        std::string line = std::format("{}\t{}\n", u, v);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const WeightedDiGraph& g)
{
    std::string desc = std::format("# WeightedDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing WeightedDiGraph {}...", g.name), g.edgeNum());
    for (const auto& [e, w] : g.edges) {
        std::string line = std::format("{}\t{}\t{}\n", e.first, e.second, w);
        os << line;
        spinner.tick();
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SimpleDiGraph& g)
{
    std::string desc = std::format("# SimpleDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing SimpleDiGraph {}...", g.name), g.edgeNum());
    for (node_t u = 0; u < g.n; u++) {
        for (const node_t& v : g.adjs[u]) {
            std::string line = std::format("{}\t{}\n", u, v);
            os << line;
            spinner.tick();
        }
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SignedUndiGraph& g)
{
    std::string desc = std::format("# SignedUndiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing SignedUndiGraph {}...", g.name), g.edgeNum());
    for (node_t u = 0; u < g.n; u++) {
        const std::vector<node_t>&posAdj = g.posAdjs[u], &negAdj = g.negAdjs[u];
        auto posIt = std::ranges::upper_bound(posAdj, u), negIt = std::ranges::upper_bound(negAdj, u);
        std::for_each(posIt, posAdj.cend(), [&](const node_t& v) {
            std::string line = std::format("{}\t{}\t+1\n", u, v);
            os << line;
            spinner.tick();
        });
        std::for_each(negIt, negAdj.cend(), [&](const node_t& v) {
            std::string line = std::format("{}\t{}\t-1\n", u, v);
            os << line;
            spinner.tick();
        });
    }
    spinner.markAsCompleted();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SignedDiGraph& g)
{
    std::string desc = std::format("# SignedDiGraph: {}\n# Nodes: {} Edges: {}\n", g.name, g.nodeNum(), g.edgeNum());
    os << desc;
    TickSpinner spinner(std::format("Writing SignedDiGraph {}...", g.name), g.edgeNum());
    for (node_t u = 0; u < g.n; u++) {
        for (const node_t& v : g.posAdjs[u]) {
            std::string line = std::format("{}\t{}\t+1\n", u, v);
            os << line;
            spinner.tick();
        }
        for (const node_t& v : g.negAdjs[u]) {
            std::string line = std::format("{}\t{}\t-1\n", u, v);
            os << line;
            spinner.tick();
        }
    }
    spinner.markAsCompleted();
    return os;
}
}

#include "graphkit.h"
#include "graphkitutils.h"

namespace gkit {
std::vector<std::vector<node_t>> SimpleDiGraph::InvAdjs() const
{
    std::vector<std::vector<node_t>> invAdjs(n);
    TickSpinner spinner("SimpleDiGraph::InvAdjs: Computing inverse adjacency list...", m);
    for (node_t u = 0; u < n; u++) {
        for (const node_t& v : adjs[u]) {
            invAdjs[v].push_back(u);
            spinner.tick();
        }
    }
    spinner.markAsCompleted();
    return invAdjs;
}
}

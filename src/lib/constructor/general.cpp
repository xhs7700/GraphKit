#include "graphkit.h"
#include "utils.h"
#include <fmt/format.h>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <string>

namespace gkit {
std::streamsize GetStreamSize(std::istream& in)
{
    std::streampos begin_pos = in.tellg();
    in.seekg(0, std::ios::end);
    std::streampos end_pos = in.tellg();
    std::streamsize file_size = end_pos - begin_pos;
    in.seekg(0, std::ios::beg);
    return file_size;
}

void UnweightedUndiGraph::addNode(node_t u) { nodes.insert(u); }
void WeightedUndiGraph::addNode(node_t u) { nodes.insert(u); }
void UnweightedDiGraph::addNode(node_t u)
{
    if (adjs.contains(u))
        return;
    adjs[u] = {};
}
void WeightedDiGraph::addNode(node_t u)
{
    if (adjs.contains(u))
        return;
    adjs[u] = {};
}

void UnweightedUndiGraph::addEdge(node_t u, node_t v)
{
    if (u > v)
        std::swap(u, v);
    if (u == v || edges.contains({ u, v }))
        return;
    nodes.insert(u), nodes.insert(v);
    edges.insert({ u, v });
}
void WeightedUndiGraph::addEdge(node_t u, node_t v, weight_t w)
{
    if (u > v)
        std::swap(u, v);
    if (u == v || edges.contains({ u, v }))
        return;
    nodes.insert(u), nodes.insert(v);
    edges[{ u, v }] = w;
}
void UnweightedDiGraph::addEdge(node_t u, node_t v)
{
    if (u == v || edges.contains({ u, v }))
        return;
    addNode(u), addNode(v);
    adjs[u].push_back(v);
    edges.insert({ u, v });
}
void WeightedDiGraph::addEdge(node_t u, node_t v, weight_t w)
{
    if (u == v || edges.contains({ u, v }))
        return;
    addNode(u), addNode(v);
    adjs[u].push_back({ v, w });
    edges[{ u, v }] = w;
}

void UnweightedUndiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading UnweightedUndiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v);
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}
void WeightedUndiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading WeightedUndiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v, w;
        std::istringstream iss(line);
        iss >> u >> v >> w;
        addEdge(u, v, w);
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}
void WeightedUndiGraph::Init(std::istream& in, std::function<weight_t()>&& generator)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading WeightedUndiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v, generator());
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}
void UnweightedDiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading UnweightedDiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v);
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}
void WeightedDiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading WeightedDiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v, w;
        std::istringstream iss(line);
        iss >> u >> v >> w;
        addEdge(u, v, w);
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}
void WeightedDiGraph::Init(std::istream& in, std::function<weight_t()>&& generator)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    SetSpinner spinner(fmt::format("Reading WeightedDiGraph {}", name), file_size);
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        node_t u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v, generator());
        spinner.setProgress(in.tellg());
    }
    spinner.markAsCompleted();
}

UnweightedUndiGraph::UnweightedUndiGraph(std::string&& name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedUndiGraph::WeightedUndiGraph(std::string&& name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedUndiGraph::WeightedUndiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator)
    : name(name)
{
    Init(in, std::move(generator));
}
UnweightedDiGraph::UnweightedDiGraph(std::string&& name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedDiGraph::WeightedDiGraph(std::string&& name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedDiGraph::WeightedDiGraph(std::string&& name, std::istream& in, std::function<weight_t()>&& generator)
    : name(name)
{
    Init(in, std::move(generator));
}

UnweightedUndiGraph::UnweightedUndiGraph(std::string&& name, std::filesystem::path& source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedUndiGraph::WeightedUndiGraph(std::string&& name, std::filesystem::path& source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedUndiGraph::WeightedUndiGraph(std::string&& name, std::filesystem::path& source, std::function<weight_t()>&& generator)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin, std::move(generator));
}
UnweightedDiGraph::UnweightedDiGraph(std::string&& name, std::filesystem::path& source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedDiGraph::WeightedDiGraph(std::string&& name, std::filesystem::path& source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedDiGraph::WeightedDiGraph(std::string&& name, std::filesystem::path& source, std::function<weight_t()>&& generator)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin, std::move(generator));
}
}

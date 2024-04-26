#include "graphkit.h"
#include <fmt/format.h>
#include <fstream>
#include <indicators/color.hpp>
#include <indicators/font_style.hpp>
#include <indicators/progress_spinner.hpp>
#include <indicators/setting.hpp>
#include <ios>
#include <iosfwd>
#include <string>

namespace gkit {
indicators::ProgressSpinner PostfixSpinner(std::string postfix)
{
    namespace opt = indicators::option;
    using indicators::FontStyle;
    return indicators::ProgressSpinner {
        opt::PostfixText { postfix },
        opt::FontStyles { std::vector<FontStyle> { FontStyle::bold } },
        opt::ShowElapsedTime { true },
        opt::ShowPercentage { true },
        opt::ShowRemainingTime { true },
        opt::ShowSpinner { false },
        opt::ForegroundColor { indicators::Color::green },
    };
}
std::streamsize GetStreamSize(std::istream& in)
{
    std::streampos begin_pos = in.tellg();
    in.seekg(0, std::ios::end);
    std::streampos end_pos = in.tellg();
    std::streamsize file_size = end_pos - begin_pos;
    in.seekg(0, std::ios::beg);
    return file_size;
}

void UnweightedUndiGraph::addNode(int u) { nodes.insert(u); }
void WeightedUndiGraph::addNode(int u) { nodes.insert(u); }
void UnweightedDiGraph::addNode(int u)
{
    if (adjs.contains(u))
        return;
    adjs[u] = {};
}
void WeightedDiGraph::addNode(int u)
{
    if (adjs.contains(u))
        return;
    adjs[u] = {};
}

void UnweightedUndiGraph::addEdge(int u, int v)
{
    if (u > v)
        std::swap(u, v);
    if (u == v || edges.contains({ u, v }))
        return;
    nodes.insert(u), nodes.insert(v);
    edges.insert({ u, v });
}
void WeightedUndiGraph::addEdge(int u, int v, int w)
{
    if (u > v)
        std::swap(u, v);
    if (u == v || edges.contains({ u, v }))
        return;
    nodes.insert(u), nodes.insert(v);
    edges[{ u, v }] = w;
}
void UnweightedDiGraph::addEdge(int u, int v)
{
    if (u == v || edges.contains({ u, v }))
        return;
    addNode(u), addNode(v);
    adjs[u].push_back(v);
    edges.insert({ u, v });
}
void WeightedDiGraph::addEdge(int u, int v, int w)
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
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading UnweightedUndiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v);
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}
void WeightedUndiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading WeightedUndiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v, w;
        std::istringstream iss(line);
        iss >> u >> v >> w;
        addEdge(u, v, w);
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}
void WeightedUndiGraph::Init(std::istream& in, std::function<int()> generator)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading WeightedUndiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v, generator());
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}
void UnweightedDiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading UnweightedDiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v);
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}
void WeightedDiGraph::Init(std::istream& in)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading WeightedDiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v, w;
        std::istringstream iss(line);
        iss >> u >> v >> w;
        addEdge(u, v, w);
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}
void WeightedDiGraph::Init(std::istream& in, std::function<int()> generator)
{
    std::streamsize file_size = GetStreamSize(in);
    std::string line;
    int prev_progress = 0;
    indicators::ProgressSpinner spinner = PostfixSpinner(fmt::format("Reading WeightedDiGraph {}", name));
    while (std::getline(in, line)) {
        if (line.starts_with('#') || line.starts_with('%'))
            continue;
        int u, v;
        std::istringstream iss(line);
        iss >> u >> v;
        addEdge(u, v, generator());
        int cur_progress = static_cast<double>(in.tellg()) / static_cast<double>(file_size) * 100;
        if (cur_progress > prev_progress) {
            spinner.set_progress(cur_progress);
            prev_progress = cur_progress;
        }
    }
    spinner.mark_as_completed();
}

UnweightedUndiGraph::UnweightedUndiGraph(std::string name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedUndiGraph::WeightedUndiGraph(std::string name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedUndiGraph::WeightedUndiGraph(std::string name, std::istream& in, std::function<int()> generator)
    : name(name)
{
    Init(in, generator);
}
UnweightedDiGraph::UnweightedDiGraph(std::string name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedDiGraph::WeightedDiGraph(std::string name, std::istream& in)
    : name(name)
{
    Init(in);
}
WeightedDiGraph::WeightedDiGraph(std::string name, std::istream& in, std::function<int()> generator)
    : name(name)
{
    Init(in, generator);
}

UnweightedUndiGraph::UnweightedUndiGraph(std::string name, std::filesystem::path source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedUndiGraph::WeightedUndiGraph(std::string name, std::filesystem::path source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedUndiGraph::WeightedUndiGraph(std::string name, std::filesystem::path source, std::function<int()> generator)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin, generator);
}
UnweightedDiGraph::UnweightedDiGraph(std::string name, std::filesystem::path source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedDiGraph::WeightedDiGraph(std::string name, std::filesystem::path source)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin);
}
WeightedDiGraph::WeightedDiGraph(std::string name, std::filesystem::path source, std::function<int()> generator)
    : name(name)
{
    std::ifstream fin(source);
    Init(fin, generator);
}
}

#include "vgraph.h"

using std::pair;

DirectedGraph::DirectedGraph(int num_nodes) : neighbors_(num_nodes), num_arcs_(0) {}

void DirectedGraph::AddArc(int from, int to, double length)
{
    pair<int, double> P(to, length);
    neighbors_[from].push_back(P);
    ++num_arcs_;
}

int DirectedGraph::NumNodes() const
{
    return neighbors_.size();
}

int DirectedGraph::NumArcs() const
{
    return num_arcs_;
}

int DirectedGraph::OutDegree(int node) const
{
    return neighbors_[node].size();
}

const vector<std::pair<int, double>> &DirectedGraph::Neighbors(int node) const
{
    return neighbors_[node];
}

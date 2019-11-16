#include "iostream"
#include "vgraph.cc"
#include "dijkstra.cc"

using namespace std;

std::ostream &operator<<(std::ostream &os, const std::vector<int> &input)
{
    for (auto const &i : input)
    {
        os << i << " ";
    }
    return os;
}

int main()
{
    DirectedGraph g(7);
    g.AddArc(5, 3, 1.2);
    g.AddArc(1, 4, 2.0);
    g.AddArc(4, 0, 1.2);
    g.AddArc(0, 3, 0.1);
    g.AddArc(6, 6, 0.0);
    g.AddArc(3, 6, 7.4);
    g.AddArc(0, 4, 0.1);
    g.AddArc(4, 5, 3.5);
    g.AddArc(0, 5, 7.4);
    g.AddArc(5, 1, 1.0);
    g.AddArc(6, 5, 0.7);
    g.AddArc(6, 0, 0.0);
    g.AddArc(3, 6, 0.4);
    return 0;
}
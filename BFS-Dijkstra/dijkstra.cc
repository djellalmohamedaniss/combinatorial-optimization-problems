#include "dijkstra.h"
#include <queue>
#include "limits"

// Runs a Dijkstra search on the graph, starting from node "src".
// See https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm .
// Returns the same "parent" vector as BFS() in bfs.1.h.

struct dijkstra_node
{
    int id;
    double distance_from_src;
    bool operator>(const dijkstra_node &dn) const
    {
        return distance_from_src < dn.distance_from_src;
    }
};

vector<int> Dijkstra(const DirectedGraph &graph, int src)
{
    if (src < 0 || src >= graph.NumNodes())
    {
        return {};
    }
    //else
    vector<bool> is_visited(graph.NumNodes(), false);
    vector<int> parent(graph.NumNodes(), -1);
    vector<double> distances(graph.NumNodes(), std::numeric_limits<int>::max());
    priority_queue<dijkstra_node, vector<dijkstra_node>, greater<dijkstra_node>>
        dijkstra_queue;

    dijkstra_queue.push({src, 0});
    distances.at(src) = 0;
    parent.at(src) = src;
    is_visited.at(src) = true;

    while (!dijkstra_queue.empty())
    {
        dijkstra_node node = dijkstra_queue.top();
        dijkstra_queue.pop();
        vector<pair<int, double>> node_neighbours = graph.Neighbors(node.id);

        for (size_t i = 0; i < node_neighbours.size(); i++)
        {
            pair<int, double> node_neighbour = node_neighbours.at(i);
            if (!is_visited.at(node_neighbour.first))
            {
                double distance_from_current_node = node.distance_from_src + node_neighbour.second;
                if (distance_from_current_node < distances.at(node_neighbour.first))
                {
                    distances.at(node_neighbour.first) = distance_from_current_node;
                    parent.at(node_neighbour.first) = node.id;
                    dijkstra_queue.push({node_neighbour.first, distance_from_current_node});
                }
            }
        }
    }
    return parent;
}

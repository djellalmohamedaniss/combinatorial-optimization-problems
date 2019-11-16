#include "bfs.1.h"
#include "deque"

using namespace std;

vector<int> BFS(const UndirectedGraph &graph, int src)
{
    if (src < 0 || src >= graph.NumNodes())
    {
        throw "error, node not found";
    }
    //else
    vector<int> parent(graph.NumNodes(), -1);
    vector<bool> visited_nodes(graph.NumNodes(), false);
    deque<int> file;
    file.push_back(src);
    parent.at(src) = src;
    visited_nodes.at(src) = true;
    while (!file.empty())
    {
        int current_parent = file.front();
        vector<int> connectedNodes = graph.Neighbors(current_parent);
        for (size_t i = 0; i < connectedNodes.size(); i++)
        {
            int current_node = connectedNodes.at(i);
            if (!visited_nodes.at(current_node))
            {
                parent.at(current_node) = current_parent;
                visited_nodes.at(current_node) = true;
                file.push_back(current_node);
            }
        }
        file.pop_front();
    }
    return parent;
}
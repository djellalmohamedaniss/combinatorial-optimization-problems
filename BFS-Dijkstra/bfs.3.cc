#include "bfs.3.h"
#include <algorithm>

vector<int> GetShortestPathFromRootedTree(const vector<int> &parent, int target)
{
    vector<int> shortest_path;
    if (!(target < 0 || target >= parent.size()))
    {
        int next_target = target;
        do
        {
            target = next_target;
            next_target = parent.at(target);
            if (next_target == -1)
            {
                shortest_path = {};
                break;
            }
            shortest_path.push_back(target);

        } while (target != next_target);
        reverse(shortest_path.begin(), shortest_path.end());
    }
    return shortest_path;
}

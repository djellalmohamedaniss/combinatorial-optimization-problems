#include "bfs.2.h"

using namespace std;

vector<int> GetBfsDistances(const vector<int> &parents)
{
    vector<int> distances(parents.size(), -1);
    /* this part is used to get the root node */
    int root = -1;
    for (size_t i = 0; i < parents.size(); i++)
    {
        if (i == parents.at(i))
        {
            root = i;
            break;
        }
    }
    if (root != -1)
    {
        distances.at(root) = 0;
        for (size_t i = 0; i < parents.size(); i++)
        {
            if (parents.at(i) != -1 && i != root)
            {
                int current_parent = parents.at(i);
                int distance = 1;
                while (true)
                {
                    if (distances.at(current_parent) != -1)
                    {
                        distances.at(i) = distances.at(current_parent) + distance;
                        break;
                    }
                    //else
                    distance = distance + 1;
                    current_parent = parents.at(current_parent);
                }
            }
        }
    }
    return distances;
}

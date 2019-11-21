#include "color.h"
#include <queue>
#include <iostream>
using namespace std;
struct dunno
{
    int node;
    int degree;
    bool colored;
    bool operator<(const dunno &other) const { return degree < other.degree; }
};

bool check_neighbor(const UndirectedGraph &graph, int node1, int node2);
bool check_all_coloried(vector<dunno> v);

int Color(const UndirectedGraph &graph, vector<int> *color)
{
    int result = 1;
    priority_queue<dunno> pr;
    vector<dunno> parcour;
    color->clear();
    //trier par rapport au degree
    for (int i = 0; i < graph.NumNodes(); i++)
    {
        pr.push({i, graph.Degree(i), false});
        color->push_back(-1);
    }
    // on copie dans un vector qu'il sera trier par ordre decroissant de degree
    while (!pr.empty())
    {
        dunno d = pr.top();
        pr.pop();
        parcour.push_back(d);
    }

    while (!check_all_coloried(parcour))
    {
        for (int i = 0; i < parcour.size(); i++)
        {
            if (parcour[i].colored == true)
                continue; //si deja colorier
            //colorier le node et changer la couleur dans color
            parcour[i].colored = true;
            color->at(i) = result;

            for (int j = i + 1; j < parcour.size(); j++)
            {
                if (parcour[j].colored == true)
                    continue; //si deja colorier
                if (check_neighbor(graph, i, j))
                    continue; // si adjacent a le node pere (i)
                bool verify = true;
                //pour chaqque node non adjacent au node pere (i)
                //on vois s'il est adjacent a un node dans laquel la couleur et result
                for (int kid : graph.Neighbors(j))
                {
                    if (color->at(kid) == result)
                        verify = false;
                }

                if (verify)
                {
                    //colorier le node et changer la couleur dans color
                    parcour[j].colored = true;
                    color->at(j) = result;
                }
            }
            result++;
        }
    }
    return result;
}

//voire si node2 et un neighbor de node 1
bool check_neighbor(const UndirectedGraph &graph, int node1, int node2)
{
    for (int node : graph.Neighbors(node1))
    {
        if (node == node2)
            return true;
    }
    return false;
}
// voire si tout les node son colorier
bool check_all_coloried(vector<dunno> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i].colored == false)
            return false;
    }
    return true;
}
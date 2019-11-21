#include "prim.h"
#include <queue>
/*#include <iostream>
using namespace std; */   

   

int Prim(int N, const vector<Edge>& edges)
{
    struct Edge1
    {
        int node1;
        int weight;
        bool operator<(const Edge1& other) const { return other.weight < weight; }
    };
    
    //********* variable *************
    int result = 0;

    vector <vector <Edge>> neighbor(N);
    // neighbor[i] contiendra la liste des edges adjacente au noeud i
    for(Edge e : edges)
    {
        neighbor[e.node1].push_back(e);
        neighbor[e.node2].push_back(e);

    }

    vector <bool> visited(N, false);
    priority_queue <Edge1> take;

    if(N < 1) return 0;
    //on ajour le node  0
    take.push({0, 0});

    int check;
    while (!take.empty())
    {
        Edge1 e = take.top();
        take.pop();

        if( visited[(e.node1)]) continue; // si deja visiter on vas au prochaine instance

        visited[e.node1] = true;
        result = result + e.weight;

        for(Edge parcour : neighbor[e.node1])
        {
            if(parcour.node1 == e.node1) check = parcour.node2; 
            else check = parcour.node1;
            if (!visited[check]) take.push({check, parcour.weight});
        }
    }
    return result;
}
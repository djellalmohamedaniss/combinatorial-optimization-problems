#include "cut.h"
#include "iostream"
#include "unordered_map"
#include <bits/stdc++.h>

using namespace std;

/*
    @method calculate the optimal cut with maximized gain using a memorization map.
    @param int -> the length of the bar
    @param vector<PricedCut>& -> the available cuts ( length and price )
    @param vector<int>* -> the cuts trace
    @param unordered_map<int, pair<int, double>>& -> the memo map
    @return double -> the max gain
*/
double OptimalCutWithMemo(int L, const vector<PricedCut> &cuts, vector<int> *cut_indices, unordered_map<int, pair<int, double>> &memo);

/* utility method to show the optimal cut and the gain starting for each available cut 
   uncomment the cout line in the optimalCut

std::ostream &operator<<(std::ostream &os, const std::unordered_map<int, std::pair<int, double>> &input)
{
    for (auto const &i : input)
    {
        os << "when L is " << i.first << ", passing by " << i.second.first << " is the most optimal way with " << i.second.second << " gain" << endl;
    }
    return os;
}
*/

double OptimalCut(int L, const vector<PricedCut> &cuts, vector<int> *cut_indices)
{
    if (cuts.size() == 0)
        return 0;
    unordered_map<int, pair<int, double>> m;
    double maxGain = OptimalCutWithMemo(L, cuts, cut_indices, m);
    pair<int, double> optimizedNode;
    while (m.find(L) != m.end())
    {
        optimizedNode = m.at(L);
        if (optimizedNode.second == 0)
            break;
        //cout << L << " loses " << cuts[optimizedNode.first].length << " with the gain of " << optimizedNode.second << endl;
        L -= cuts[optimizedNode.first].length;
        cut_indices->push_back(optimizedNode.first);
    }
    return maxGain;
}

double OptimalCutWithMemo(int L, const vector<PricedCut> &cuts, vector<int> *cut_indices, unordered_map<int, pair<int, double>> &memo)
{
    vector<double> gain(cuts.size(), 0);
    for (int i = 0; i < cuts.size(); i++)
    {
        if (memo.find(L) != memo.end())
        {
            return memo.at(L).second;
        }
        if (L >= cuts[i].length)
        {
            gain[i] = cuts[i].price + OptimalCutWithMemo(L - cuts[i].length, cuts, cut_indices, memo);
        }
    }

    int max_position = max_element(gain.begin(), gain.end()) - gain.begin();
    memo.insert({L, {max_position, gain[max_position]}});
    return gain[max_position];
}
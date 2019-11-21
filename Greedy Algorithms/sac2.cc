#include "sac2.h"
#include <algorithm>
using namespace std;

double Sac2(int C, const vector<int> &p, const vector<double> &v)
{
    const int n = p.size();
    vector<int> indice;
    double result = 0;
    for (int i = 0; i < n; ++i)
        indice.push_back(i);
    sort(indice.begin(), indice.end(), //fonction auto compare donner au td elle compare les indice
         [&](int i, int j) {
             if (p[j] == 0)
                 return false;
             if (p[i] == 0)
                 return true;
             return v[i] * p[j] > v[j] * p[i];
         });

    for (int i : indice)
    {
        if (p[i] <= C)
        {
            C -= p[i];
            result += v[i];
        }
        else
        {
            result += v[i] * double(C) / p[i];
            break;
        }
    }
    return result;
}
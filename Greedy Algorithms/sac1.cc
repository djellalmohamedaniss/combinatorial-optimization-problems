#include "sac1.h"
#include <algorithm> 
#include <vector> 

int Sac1(int C, const vector<int>& p)
{
    vector<int> tmp = p;
    int result = 0;
    sort(tmp.begin(), tmp.end());
    
    for(int i = 0; i < tmp.size(); i++)
    {
        if(tmp[i] > C) continue;
        result++;
        C = C - tmp[i];
    }
    return result;
}

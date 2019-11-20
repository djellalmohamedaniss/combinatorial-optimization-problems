#include "parcel2.h"
#include "math.h"

vector<int> OptimalParcels(const vector<int> &gain)
{
    if (gain.size() == 0)
        return {};
    /*
        positions is a binary vector where positions[i] = 1 if we count the ith element in the gain, otherwise it's 0
    */
    vector<int> positions(gain.size(), 0);
    int gainExcludingActualElement = 0;
    int maxGain = 0;

    // we assume that the first element is counted in the maxGain
    positions[0] = 1;
    int gainIncludingActualElement = gain.at(0);

    for (size_t i = 1; i < gain.size(); i++)
    {
        maxGain = max(gainIncludingActualElement, gainExcludingActualElement);
        gainIncludingActualElement = gainExcludingActualElement + gain.at(i);
        gainExcludingActualElement = maxGain;
        if (gainIncludingActualElement > gainExcludingActualElement)
        {
            positions[i - 1] = 0;
            positions[i] = 1;
            // corner case
            if (i == 2)
            {
                positions[0] = 1;
            }
        }
    }

    /*
     the P vector contains the positions of the elements counted in the maxGain ( the ones with positions[i] = 1)
    */
    vector<int> p;
    for (size_t j = 0; j < positions.size(); j++)
    {
        if (positions.at(j) == 1)
        {
            p.push_back(j);
        }
    }
    return p;
}
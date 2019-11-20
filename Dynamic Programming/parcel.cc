#include "parcel.h"
#include <vector>    // for vector
#include <algorithm> // for copy() and assign()
#include <iterator>  // for back_inserter
#include <math.h>

/* RECURSIVE, FAILS IN LONG VECTORS

int MaxGain(const vector<int> &gain)
{
    if (gain.size() == 0)
        return 0;
    if (gain.size() == 1)
        return gain[0];
    if (gain.size() == 2)
        return max(gain[0], gain[1]);

    vector<int> *v1 = new vector<int>;
    vector<int> *v2 = new vector<int>;

    copy(gain.begin() + 2, gain.end(), back_inserter(*v1));
    copy(gain.begin() + 1, gain.end(), back_inserter(*v2));

    return max(gain[0] + MaxGain(*v1), MaxGain(*v2));
}
*/

/* RECURSIVE WITH MEMORISATION

int MaxGainWithMemo(const vector<int> &gain, vector<int> &memo, int index);


int MaxGainWithMemo(const vector<int> &gain, vector<int> &memo, int index)
{
    if (index == 0)
    {
        return gain.at(0);
    }
    if (index == 1)
    {
        return max(gain.at(0), gain.at(1));
    }
    if (memo.at(index) != -1)
    {
        return memo.at(index);
    }
    memo[index] = max(MaxGainWithMemo(gain, memo, index - 1), gain.at(index) + MaxGainWithMemo(gain, memo, index - 2));
    return memo[index];
}

int MaxGain(const vector<int> &gain)
{
    if (gain.size() == 0)
        return 0;
    if (gain.size() == 1)
        return gain.at(0);
    vector<int> memo(gain.size(), -1);
    return MaxGainWithMemo(gain, memo, gain.size() - 1);
}

*/

/* ITERATIVE */

int MaxGain(const vector<int> &gain)
{
    if (gain.size() != 0)
    {
        int GainIncludingCurrentValue = gain.at(0), GainExcludingCurrentValue = 0, currentMaxGain;
        for (size_t i = 1; i < gain.size(); i++)
        {
            currentMaxGain = max(GainIncludingCurrentValue, GainExcludingCurrentValue);
            GainIncludingCurrentValue = GainExcludingCurrentValue + gain.at(i);
            GainExcludingCurrentValue = currentMaxGain;
        }
        return max(GainIncludingCurrentValue, GainExcludingCurrentValue);
    }
    return 0;
}

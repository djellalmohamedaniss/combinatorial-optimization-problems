#include "parcel.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main()
{
  {
    cout << "Simple examples." << endl;

    CHECK_EQ(MaxGain({3, 7, 2, 1, 8, 4, 3}), 18);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain(vector<int>(100, 42)), 50 * 42);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain(vector<int>(101, 42)), 51 * 42);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Corner cases." << endl;

    CHECK_EQ(MaxGain({9, 5}), 9); // Only 2 parcels
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain({5}), 5); // Only one parcel
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain({4, 7, 4}), 8); // Empty
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain({}), 0); // Empty
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Performance + stress test." << endl;
    vector<int> gains;
    const int num_tests = 200;
    const vector<int> expected = {1462, 1129, 1404, 2381, 907, 358, 1037, 481, 644, 2171, 829, 1140, 2006, 576, 998, 859, 2321, 1574, 97, 1363, 866, 1631, 2679, 2660, 379, 2520, 1599, 1251, 2170, 2742, 2698, 892, 1125, 2143, 1281, 515, 1579, 959, 1065, 1394, 884, 1078, 1270, 2553, 1028, 186, 873, 1706, 612, 2739, 112, 1403, 2658, 1558, 300, 1043, 730, 1549, 486, 2719, 17, 381, 2355, 2313, 1488, 793, 406, 443, 2840, 1907, 2317, 750, 2293, 2353, 1556, 1237, 297, 1426, 1955, 956, 873, 2231, 1477, 465, 1525, 856, 660, 2616, 2544, 2004, 2340, 1857, 2031, 1003, 759, 288, 2137, 1159, 3122, 59, 1246, 942, 1423, 2666, 1374, 798, 499, 2388, 1746, 2462, 2649, 0, 2513, 891, 1576, 2142, 406, 309, 2046, 2374, 87, 1305, 2748, 2255, 2965, 1322, 575, 2487, 1390, 2894, 2156, 1897, 158, 2259, 1084, 213, 1254, 2520, 224, 2839, 1110, 2032, 1662, 660, 2130, 835, 1078, 2593, 985, 656, 2278, 944, 2299, 1410, 2135, 1151, 486, 2356, 2701, 2315, 649, 2360, 1693, 498, 2216, 1496, 2732, 241, 39, 2907, 1881, 1000, 1722, 945, 200, 2138, 2266, 2320, 1839, 1064, 2951, 1361, 1778, 1266, 973, 0, 165, 274, 681, 1852, 2715, 1165, 2441, 1193, 1180, 2100, 183, 616, 2530, 0};
    vector<vector<int>> test_gains;
    for (int i = 0; i < num_tests; ++i)
    {
      Random r(i);
      gains.clear();
      const int size = r.Uniform(100);
      while (gains.size() < size)
        gains.push_back(r.Uniform(100));
      test_gains.push_back(gains);
    }
    clock_t c0 = clock();
    for (int i = 0; i < num_tests; ++i)
    {
      CHECK_EQ(MaxGain(test_gains[i]), expected[i])
          << "Failed on input #" << i << ": " << test_gains[i];
    }
    const double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "PASSED at line " << __LINE__ << endl;
    cout << "Time: " << t << " s." << endl;
    if (t < 0.001)
    {
      cout << "NICE! Complexity is probably linear!" << endl;
    }
    else if (t < 1)
    {
      cout << "NOT IDEAL: The complexity is probably quadratic. That's good, but you can do better!" << endl;
    }
    else
    {
      cout << "SLOW: The complexity seems exponential... Try again!" << endl;
    }
  }
}

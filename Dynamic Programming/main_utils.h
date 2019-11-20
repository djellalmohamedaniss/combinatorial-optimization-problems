#ifndef __MAIN_UTILS_H
#define __MAIN_UTILS_H

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

typedef long long int64;

/* To use if mt19937 proves to be platform-dependent. BEWARE! Less "random" than mt19937.
class Random {
 public:
  explicit Random(int seed) {
    seed_ = seed & 0x7fffffffu;  // Make it non-negative.
    if (seed_ == 0 || seed_ == 0x7fffffffu) seed_ = 1;  // Prevent forbidden values.
  }

  inline int RandInt() {
    unsigned long long p = seed_ * 16807ULL;  // bits 14, 8, 7, 5, 2, 1, 0
    seed_ = (unsigned int)((p >> 31) + (p & 0x7fffffffu));
    if (seed_ > 0x7fffffffu) seed_ -= 0x7fffffffu;
    return seed_;
  }

  inline double RandDouble() { return RandInt() / double(0x7fffffff); }

  inline int Uniform(int max) { return max == 0 ? 0 : RandInt() % max; }

  // To support shuffle(x.begin(), x.end(), Random());
  typedef int result_type;
  int min() const { return 0; }
  int max() const { return INT_MAX; }
  inline int operator()() { return RandInt(); }

 private:
  unsigned int seed_;
};
*/

class Random : public std::mt19937 {
 public:
  explicit Random(int seed) : std::mt19937(seed) {}

  inline int RandInt() { return operator()(); }
  inline double RandDouble() { return operator()() / (max() + 1.0); }
  inline double RandExp() {
    return -log1p(-RandDouble());
  }  // in [0, +inf), with mean=1
  inline int Uniform(int max) { return operator()() % max; }
};

// We re-implement std::shuffle because its algorithm may vary across
// platforms/compilers, thus affecting the random number generator.
// This version is meant to be deterministic.
template <class It, class Random>
void shuffle(It begin, It end, Random& rng) {
  if (begin == end) return;
  while (true) {
    const int dist = end - begin;
    if (dist == 1) return;
    std::swap(*begin, *(begin + rng() % dist));
    ++begin;
  }
}

// Use "DLOG << ..." for debug statements instead of "cout", and
// compile with -DDEBUG for these DLOG to actually print.
#ifdef DEBUG
#define __vlog_is_on__ true
#else
#define __vlog_is_on__ false
#endif
#define LOG LogStream(INFO, __FILE__, __LINE__)
#define DLOG if (__vlog_is_on__) LOG
#define DLOG_IF if (__vlog_is_on__) LOG_IF
#define FAIL LogStream(FATAL, __FILE__, __LINE)


enum LogSeverity {
  INFO = 1,
  FATAL = 2,
};

string NowStr() {
  const chrono::system_clock::time_point now = chrono::system_clock::now();
  time_t now_c = chrono::system_clock::to_time_t(now);
  const chrono::system_clock::time_point now_rounded =
      chrono::system_clock::from_time_t(now_c);
  const int microseconds = chrono::duration_cast<chrono::microseconds>(
      now - now_rounded).count();
  char mbstr[100];
  const int chars = std::strftime(mbstr, sizeof(mbstr), "%y%m%d_%T",
                                  std::localtime(&now_c));
  sprintf(mbstr + chars, ".%06d", microseconds);
  return mbstr;
}

class LogStream {
 public:
  explicit LogStream(LogSeverity severity, const string& filename,
                     int line_num)
    : severity_(severity) {
    if (severity_ == FATAL) {
      std::cerr << "FATAL ERROR: ";
    }
    std::cerr
        << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
        << (severity == FATAL ? 'F' : 'I')
        << NowStr() << " " << filename << ":" << line_num << ": ";
  }

  template<class T>
  LogStream& operator<<(const T& x) {
    std::cerr << x;
    return *this;
  }

  ~LogStream() {
    std::cerr << "\n";
    if (severity_ == FATAL) exit(1);
  }
 private:
  const LogSeverity severity_;
};

class LogMessageVoidify {
 public:
  // We need an operator with precedence lower than << but higher than ?:
  void operator&(LogStream&) {}
  bool operator&(bool b) { return b; }
};

#define LOG_IF(severity, condition)                         \
  switch (0)                                                \
    default: !(condition) ? (void)0 : LogMessageVoidify() & \
  LogStream(severity, __FILE__, __LINE__)

#define CHECK(x) \
  LOG_IF(FATAL, !(x)) << "CHECK condition violated: '" #x << "'. "

#define CHECK_BINARY(test, x, y) \
  LOG_IF(FATAL, !((x) test (y))) \
      << "CHECK violated: \"" #x " " #test " " #y "\" (values: " \
      << (x) << " " #test " " << (y) <<  ") "

#define CHECK_EQ(x, y) CHECK_BINARY(==, x, y)
#define CHECK_NE(x, y) CHECK_BINARY(!=, x, y)
#define CHECK_LT(x, y) CHECK_BINARY(<, x, y)
#define CHECK_GT(x, y) CHECK_BINARY(>, x, y)
#define CHECK_LE(x, y) CHECK_BINARY(<=, x, y)
#define CHECK_GE(x, y) CHECK_BINARY(>=, x, y)

inline bool DoubleOrNanEquals(double x, double y) {
  return std::isnan(x) ? std::isnan(y) : (x == y);
}

#define CHECK_EQ_OR_NAN(x, y) \
  LOG_IF(FATAL, !DoubleOrNanEquals((x), (y))) \
      << "CHECK violated: \"" #x " == " #y "\" (values: " \
      << (x) << " == " << (y) <<  ") "

inline bool DoubleOrNanApproxEquals(double x, double y, double relative_error) {
  if (std::isnan(x) || std::isnan(y)) return std::isnan(x) == std::isnan(y);
  if (!std::isfinite(x) || !std::isfinite(y)) {
    return std::isfinite(x) == std::isfinite(y) && (x < 0) == (y < 0);
  }
  return fabs(fabs(x) - fabs(y)) <= relative_error * 0.5 * (fabs(x) + fabs(y));
}

#define CHECK_NEAR(x, y, eps) \
  LOG_IF(FATAL, !DoubleOrNanApproxEquals(x, y, eps)) \
     << ": CHECK_NEAR violated: \"" #x " ~= " #y \
     << " with relative precision " << #eps << "\" (values: "\
     << (x) << " ~= " << (y) << "), diff: " << (x) - (y)

#ifdef DEBUG
#define DCHECK(x) CHECK(x)
#define DCHECK_EQ(x, y) CHECK_EQ(x, y)
#define DCHECK_NE(x, y) CHECK_NE(x, y)
#define DCHECK_LT(x, y) CHECK_LT(x, y)
#define DCHECK_GT(x, y) CHECK_GT(x, y)
#define DCHECK_LE(x, y) CHECK_LE(x, y)
#define DCHECK_GE(x, y) CHECK_GE(x, y)
#define DCHECK_NEAR(x, y, eps) CHECK_NEAR(x, y, eps)
#else  // DEBUG
#define DCHECK(x)
#define DCHECK_EQ(x, y)
#define DCHECK_NE(x, y)
#define DCHECK_LT(x, y)
#define DCHECK_GT(x, y)
#define DCHECK_LE(x, y)
#define DCHECK_GE(x, y)
#define DCHECK_NEAR(x, y, eps)
#endif  // DEBUG

template <class T>
string ToStr(const T& t);

template <class U, class V>
ostream& operator<<(ostream& o, const pair<U, V>& p);

template <class T>
ostream& operator<<(ostream& out, const vector<T>& v);

template <typename T>
string StrJoin(const T& v, const string& sep);

template <class U, class V>
ostream& operator<<(ostream& o, const pair<U, V>& p) {
  return o << "(" << p.first << ", " << p.second << ")";
}

template <typename T>
string StrJoin(const T& v, const string& sep) {
  stringstream ss;
  int size = 0;
  for (const auto& x : v) {
    if (size++) ss << sep;
    ss << x;
  }
  return ss.str();
}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& v) {
  return out << '[' << StrJoin(v, " ") << ']';
}

template <class T>
string ToStr(const T& t) {
  stringstream ss;
  ss << t;
  return ss.str();
}

template <class T>
void Canonicalize(T*) {}

template <class T>
void Canonicalize(vector<T>* v) {
  for (T& x : *v) Canonicalize(&x);
  sort(v->begin(), v->end());
}

template <class T>
T CanonicalizeCopy(const T& t) {
  T copy(t);
  Canonicalize(&copy);
  return copy;
}

template <class T>
vector<T> CanonicalizeElements(const vector<T>& v) {
  vector<T> out;
  out.reserve(v.size());
  for (const T& x : v) out.push_back(CanonicalizeCopy(x));
  return out;
}

#endif  // __MAIN_UTILS_H

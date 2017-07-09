#include <chrono>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;

struct scoped_timer {
  scoped_timer(double& s) : seconds(s), t0(steady_clock::now()) {}
 ~scoped_timer()
 {
   std::chrono::duration<double> diff = steady_clock::now() - t0;
   seconds = diff.count();
 }
 double&                       seconds;
 steady_clock::time_point      t0;
};

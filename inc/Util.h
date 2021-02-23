#ifndef UTIL_H
#define UTIL_H

bool close(double d, double target = 0.0){ return ((d >= (target - 1e-5)) && (d <= (target + 1e-5))); }

#endif //UTIL_H
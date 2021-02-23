#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Triplet.h"

using namespace std;
const double aspect_ratio = 16.0 / 9.0;
const int sampling_rate = 4;

class View {
public:
    int h_res;
    int v_res;
    int pixel_size;
    double d;
    inline double random_double(double min, double max) { return min + (max-min)*rrand();}
    View(int width, int height, int sideLen, double d);
    View();
    vector<Point> getMultiJittered(int row, int col);
    vector<Point> getSingleRay(int row, int col);
    int getW();
    int getH();
    inline int colVal(int col){ return col + h_res; }
    inline Point getLookAt(int row, int col, double x_off = 0.5, double y_off = 0.5) { return Point(pixel_size * (row - (h_res/2.0) + x_off), pixel_size * (col - (v_res/2.0) + y_off), -d); }
    double gen_offset();
};



#endif //VIEW_H
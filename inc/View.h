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
const int sampling_rate = 16;

class View {
public:
    int h_res;
    int v_res;
    double pixel_size;
    double d;
    inline double random_double(double min, double max) { return min + (max-min)*rrand();}
    View(int width, double sideLen, double d);
    View();
    vector<Point> getMultiJittered(int row, int col);
    vector<Point> getSingleRay(int row, int col);
    int getW();
    int getH();
    inline int colVal(int col){ return col + h_res; } //utility for indexing internally
    Point getLookAt(int row, int col, double xo, double yo); //tranlates row,col and offsets into points in 3D space 
    double gen_offset(); //returns constant offset
};



#endif //VIEW_H
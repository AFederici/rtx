#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Triplet.h"

using namespace std;
const double aspect_ratio = 16.0 / 9.0; //optionally used to gen height if only a width is given to the viewplane

class View {
public:
    int h_res; //pixels vertically
    int v_res; //pixels horizontally
    double pixel_size;
    double d; //distance from the camera (negative applied automatically, should be positive)
    //random within range
    inline double random_double(double min, double max) { return min + (max-min)*rrand();}
    View(int width, double sideLen, double d); //uses aspec ration for height
    View(int width, int height, double sideLen, double d);
    View();
    //returns points sample using multijittering. sampling rate determines number of samples returned
    vector<Point> getMultiJittered(int col, int row, int sampling_rate = 16);
    
    //returns one point in middle of pixel, no sampling
    vector<Point> getSingleRay(int col, int row);
    int getW(); //getter for h_res
    int getH(); //getter for v_res
    inline int colVal(int col){ return col + h_res; } //utility for indexing internally
    Point getLookAt(int col, int row, double xo, double yo); //tranlates row,col and offsets into points in 3D space 
    double gen_offset(); //returns constant offset
};



#endif //VIEW_H
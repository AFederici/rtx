#ifndef CAM_H
#define CAM_H
#include "Shapes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Cam {
public:
    Cam(Vec up, Point eye, Point lookAt); Cam(); //contstructors for the camera
    void basis(); //calculate the basis vectors for the viewing angles
    void setBasis(Vec u, Vec v, Vec w);
    Vec up; Point eye; Point lookAt;
    Vec w; Vec u; Vec v;
};


#endif //CAM_H
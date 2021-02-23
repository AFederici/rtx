#ifndef CAM_H
#define CAM_H
#include "Shapes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Cam {
public:
    Cam(Vec up, Point eye, Point lookAt);
    Cam();
    void basis();
    Vec up; Point eye; Point lookAt;
    Vec w; Vec u; Vec v;
};


#endif //CAM_H
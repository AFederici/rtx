#ifndef CAM_H
#define CAM_H
#include "Shapes.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//clas to represent information abot the camer
class Cam {
public:
    /*
    * camera constructor takes a location for the eye point,
    * a point to determine where the camera is looking,
    * and an up vector to determin camera rotation
    * 
    * Basis is called at the end of constructor call
    */
    Cam(Point eye, Point lookAt, Vec up); Cam();
    void setBasis(Vec u, Vec v, Vec w); //setter for (u,v,w)
    Vec up; Point eye; Point lookAt;
    Vec w; Vec u; Vec v;
private:
    void basis(); //calculate the basis vectors (u,v,w) for the viewing angles
};


#endif //CAM_H
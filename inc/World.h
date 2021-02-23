#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include <string>
#include "View.h"
#include "Shapes.h"
#include "Cam.h"

using namespace std;

#define MODE "P3"

const Color shadow = Color(0,0,0);

class World {
public:
    vector<Dot*> lights;
    vector<Shapes*> objects;
    View viewPlane;
    vector< vector<Color> > output;
    Cam camera;
    bool sampling;
    void setNoSampling();
    bool isOrtho;
    World(View &viewPlane);
    World(View &viewPlane, Cam& c);
    Ray getRay(Point look);
    void savePPM(ostream& ostr);
    void debugPPM();
    int rayColor(Color & c,Ray &r);
    void rayShadow(Color & c, Ray &r, int ind);
    void addLight(Dot * s);
    void addObj(Shapes * s);
    void render();
};



#endif //WORLD_H
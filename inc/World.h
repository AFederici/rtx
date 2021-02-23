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

const Color shadow = Color(0.1,0.1,0.1);

class World {
public:
    vector<Dot*> lights;
    vector<Shapes*> objects;
    View viewPlane;
    vector< vector<Color> > output; //rendering stored here, not directly saves
    Cam camera;
    bool sampling; //if true, multi-jittered sampling is used
    void setNoSampling();
    bool isOrtho;
    World(View &viewPlane);
    World(View &viewPlane, Cam& c);
    Ray getRay(Point look); //generate a ray into the world based on camera and point on viewplane
    void savePPM(ostream& ostr); //save output to file
    void debugPPM(); //write PPM to cout
    int rayColor(Color & c,Ray &r); //find the nearest ray intersection and update the color. Shading also computed here 
    void rayShadow(Color & c, Ray &r, int ind); //adjust the color where a ray hit based on shadows here
    void addLight(Dot * s); //add light to vector
    void addObj(Shapes * s); //add object to vector
    void render(); //do ray tracing
};



#endif //WORLD_H
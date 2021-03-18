#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include <string>
#include "View.h"
#include "Shapes.h"
#include "Cam.h"
#include <limits>
#include <algorithm>

using namespace std;

#define MODE "P3"

const Color shadow = Color(0.01,0.01,0.01);
struct Node {
    pair<Point,Point> _box;
    vector<Shapes*> s;
    bool empty;
    struct Node* left;
    struct Node* right;
    Node(pair<Point,Point> box)
    {
        _box = make_pair(Point(box.first), Point(box.second));
        left = NULL;
        right = NULL;
        empty = true;
    }
};
class World {
public:
    double ambience = 0.2;
    vector<Dot*> lights;
    vector<Shapes*> objects;
    View viewPlane;
    vector< vector<Color> > output; //rendering stored here, not directly saves
    Cam camera;
    int sampling_rate = 0; //if greater than 0, multi-jittered sampling is used
    void setNoSampling();
    bool isOrtho;
    bool isAccel;
    World(View &viewPlane);
    World(View &viewPlane, Cam& c);
    Ray getRay(Point look); //generate a ray into the world based on camera and point on viewplane
    void savePPM(ostream& ostr); //save output to file
    void debugPPM(); //write PPM to cout
    Color computeShading(Ray &r, int closest_ind);
    int rayShadow(Ray &r, int ind, Node * tree); //adjust the color where a ray hit based on shadows here
    void addLight(Dot * s); //add light to vector
    void addObj(Shapes * s); //add object to vector
    void render(); //do ray tracing
    int rayIntersection(Ray & r, Node * tree= NULL, bool accel = false);
    map<int, vector<Shapes*> > regularGrid;
    //void buildRegularGrid();
    vector< vector<Shapes*> > extractVec(vector<Shapes* > v, int axis, Point partition);
    Node * buildBVH(vector<Shapes*> v);
};

void pairPointSwap(pair<Point,Point> &p1, vector<Point> &p2, int ind);
uint64_t encode(uint16_t x, uint16_t y, uint16_t z);


#endif //WORLD_H
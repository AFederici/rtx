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

//data-structure for BVH
struct Node {
    pair<Point,Point> _box; //bounding box of objects in every child node
    vector<Shapes*> s; //container of pointers to objects this node contains
    bool empty; //flag for if this is a leaf node. Should be false if leaf, and s should be non-empty as well
    struct Node* left;
    struct Node* right;

    //contstuctor takes in bounding box
    Node(pair<Point,Point> box)
    {
        _box = make_pair(Point(box.first), Point(box.second));
        left = NULL;
        right = NULL;
        empty = true;
    }
};

//class to take care of general things like setting up the world, rendering, etc
class World {
public:
    double ambience = 0.2; //used as ambience constant in phong-reflection
    vector<Dot*> lights; //all point lights in the scene
    vector<Shapes*> objects; //all objects
    View viewPlane;
    vector< vector<Color> > output; //rendering stored here results stored here
    Cam camera;
    int sampling_rate = 0; //if greater than 0, multi-jittered sampling is used with this as the sampling rate
    bool isOrtho; //if true, rays shot orthogonal to viewplane
    bool isAccel; //if true, bvh is created 
    World(View &viewPlane); //if no camera passed, assume orthogonal viewing
    World(View &viewPlane, Cam& c);
    Ray getRay(Point look); //generate a ray into the world based on camera and point on viewplane
    void savePPM(ostream& ostr); //save output to file
    void debugPPM(); //write PPM to cout
    void addLight(Dot * s); //add light to vector
    void addObj(Shapes * s); //add object to vector
    void render(); //do ray tracing. for each col,row -> sample, find intersections, get shading, find shadows
    int rayIntersection(Ray & r, Node * tree= NULL, bool accel = false);

    //unused, accidentally implemented instead of BVH at first
    map<int, vector<Shapes*> > regularGrid;
    //void buildRegularGrid();

private:
    /*
    * Take the ray and look at the closest hit point (found in some other function)
    * which is passed in as closest_ind
    * Index to that hitpoint, and use the object reference stored in it 
    * as well as the t value to find the hit point. Pass that hit point into 
    * the objects normal function. With the normal and hitpoint, shading
    * can be calculated */
    Color computeShading(Ray &r, int closest_ind); 

    //calculates if intersection point given by r and ind is in shadow.
    //BVH only useed if tree pointer is not null. If not null, BVH intersection is used vs naive iteration
    int rayShadow(Ray &r, int ind, Node * tree);

    //split vector into two parts based on partition point and axis given.
    //Smaller than point on axis is the 0 index of the return value, greater is the 1 index vector in the return nested vector
    vector< vector<Shapes*> > extractVec(vector<Shapes* > v, int axis, Point partition);

    Node * buildBVH(vector<Shapes*> v); //use BVH algo from README
};

//The goal if this function is to iterate over shapes, and pass in their
//bounding box and centroid values in as p2. p1 is initialized at the start
//and by the end of iteration will contain the overall bounding box OR
//overall min and max centroid values across all 3 dimmensions (if ind == 2 its the latter)
//p1.first contains all min vals and p1.first contains all max values
//p2 is compared against both of these. Ind is used to index p2 
//based on if comparisons are for centroids or for bounding boxes
void pairPointSwap(pair<Point,Point> &p1, vector<Point> &p2, int ind);

//hash function for a Triplet object
uint64_t encode(uint16_t x, uint16_t y, uint16_t z);


#endif //WORLD_H
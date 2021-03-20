#ifndef SHAPES_H
#define SHAPES_H
#include "Triplet.h"
#include <iostream>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <math.h>
#include <stdio.h>
#include <fstream>

using namespace std; 

//max and min t used in intersection checks
#define T_MIN 0
#define T_MAX 1000

//very large and very small double used as default vals in max and min calculations
const static double maxD = numeric_limits<double>::max() - 5;
const static double minD = -1*maxD;

//forward declaration
class Ray; 

//color properties for each object
struct Material {
    Color ambient;
    Color diffuse;
    Color specular;
    double shiny; //alpha in phong reflection
    friend Material operator*(const Material &m1, double d){ 
        Material m2;
        m2.ambient=m1.ambient*d;
        m2.diffuse=m1.diffuse*d;
        m2.specular=m1.specular*d;
        m2.shiny=m1.shiny*d;
        return m2;
    }
    friend Material operator*(double d, const Material &m1) { return m1*d; } 
    friend Material operator+(const Material &m1, const Material &m2){ 
        Material m3;
        m3.ambient=m1.ambient+m2.ambient;
        m3.diffuse=m1.diffuse+m2.diffuse;
        m3.specular=m1.specular+m2.specular;
        m3.shiny=m1.shiny+m2.shiny;
        return m3;
    }
};

//generic class for objects in a scene
class Shapes {
public:
    Material material;
    virtual int intersect(Ray& r) = 0; //check whether a ray intersects a shape
    virtual Vec norm(Point intersection) = 0; //get the norm at a point
    virtual ~Shapes() {};  

    //check what sub-class an object is
    template<typename T>
    bool isA() {
        return (dynamic_cast<T*>(this) != NULL);
    }
    void setMaterial(Material m) { material = m; }

    //generic setter for an objects material properties. Note, need to pass in a color to set 
    //the shiny variable. Only the first index is used for that
    void addShader(string s, Color c){
        if (s == "ambient") material.ambient = c;
        if (s == "diffuse") material.diffuse = c;
        if (s == "specular") material.specular = c;
        if (s == "shiny") material.shiny = c[0];
    }
    
    void addShader(double shiny) { material.shiny = shiny; } //overload to allow another way to set the shiny value of the material
    vector<Point> aabb; //axis-aligned bounding box as well as centroid information stored here
    Material getColor() { return material; }
};

struct hit {
    double t; //multiplier for where a ray intersects a shape, t from (p = o + td)
    Point p; //used barycentric points on triangle intersection
    Shapes * shape_obj; //reference the shape an intersection happened at
    Vec normal; //normal vector at intersection 
};

class Ray { 
public:
    Ray(Point origin, Vec direction); 
    Point mult(double t); //returns o + t*d

    //set normal to be outwards facing -> referenced from https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/creatingourfirstraytracedimage
    //if dot product negative, normal is opposite direction as ray which we want
    inline void set_normal(Vec outward_normal, int ind) {
        hits[ind].normal = (dir.dot(outward_normal) < 0.0) ? outward_normal : -1*outward_normal;
    }
    inline Vec& getDir(){ return dir; };
    Vec inverseNorm; //used in bounding volume intersection calculations
    Point o;
    Vec dir;
    vector<hit> hits; //collection hit objects (multiple intersections happen before deciding the closest)
    int lastHit; //index of closest intersection point from hits vector
};

class Plane: public Shapes {
public:
    //used to make triangle and plane logic similar
    //by calculating plane normal from 3 points on it
    Point p1; Point p2; Point p3;
    Vec e1; Vec e2; Vec e3;
    Vec nhat;

    Plane(double ax, double by, double cz, double dShift); //Assuming ax+by+cz+d=0
    Plane() {};
    int intersect(Ray& r) override;
    Vec norm(Point intersection) override;
    double a; double b; double c; double d;
    ~Plane(){};
//used in initialization. Set points to be CCW
private:
    void setNorm();
    void setPoints();
};

class Sphere: public Shapes {
public:
    Sphere(Point c, double r); //center and radius
    Vec norm(Point intersection) override;
    int intersect(Ray& r) override;
    Point center;
    double radius;
    ~Sphere(){};
};

//used for point lights and triangle vertices
class Dot: public Shapes {
public:
    Point loc;
    Dot() { loc = Point(0.0,0.0,0.0); normVec = Vec(0.0,0.0,0.0); }
    Dot(Point l);
    Dot(const Dot &d);
    ~Dot(){};
    int intersect(Ray& r) override;
    Vec norm(Point intersection) override;

    //used to get around default Dot behavior for norm
    //kind of hacky, but these are how the norm for vertices are manipulated
    //the norm of a triangleMesh object is calculated from this variable
    Vec normVec;
    void setNormVec(Vec norm);
    Vec getNormVec();
};

class Triangle: public Plane {
public:
    double area;
    Dot v1; Dot v2; Dot v3; //vertices
    int intersect(Ray& r) override;
    void setNorm();
    Triangle(Dot v1, Dot v2, Dot v3); //points are made to be CCW in the constuctor

    //the material of the triangle is the average of the 3 vertices materials'
    //which are weight based on barcentric coordinate passed in
    Material getColor(Point baryCoord) { return (baryCoord.x*v1.material + baryCoord.y*v2.material + baryCoord.z*v3.material)*(1.0/3.0);}
};

#endif //SHAPES_H
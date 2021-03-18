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

#define DEFAULT_MATERIAL_CONST 0.5
#define T_MIN 0
#define T_MAX 1000

using namespace std; 
const static double maxD = numeric_limits<double>::max() - 5;
const static double minD = -1*maxD;
class Ray; 
struct Material {
    Color ambient;
    Color diffuse;
    Color specular;
    double shiny;
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
    void addShader(string s, Color c){
        if (s == "ambient") material.ambient = c;
        if (s == "diffuse") material.diffuse = c;
        if (s == "specular") material.specular = c;
        if (s == "shiny") material.shiny = c[0];
    }
    void addShader(double shiny) { material.shiny = shiny; }
    vector<Point> aabb;
    Material getColor() { return material; }
};

struct hit {
    double t; //multiplier for where a ray intersects a shape, t from (p = o + td)
    Point p; //needed for bary mostly
    Shapes * shape_obj; //reference the shape an intersection happened at
    Vec normal; //normal vector at intersection 
};

class Ray { 
public:
    Ray(Point origin, Vec d);
    Point mult(double t); //returns o + t*d
    //set normal to be outwards facing -> referenced from https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/creatingourfirstraytracedimage
    //if dot product negative, normal is opposite direction as ray which we want
    inline void set_normal(Vec outward_normal, int ind) {
        hits[ind].normal = (dir.dot(outward_normal) < 0.0) ? outward_normal : -1*outward_normal;
    }
    inline Vec& getDir(){ return dir; };
    Vec inverseNorm;
    Point o;
    Vec dir;
    vector<hit> hits;
    int lastHit;
};

class Plane: public Shapes {
public:
    Point p1; Point p2; Point p3;
    Vec e1; Vec e2; Vec e3;
    Vec nhat;
    Plane(double ax, double by, double cz, double dShift);
    Plane() {};
    int intersect(Ray& r) override;
    void setNorm();
    void setPoints();
    Vec norm(Point intersection) override;
    double a; double b; double c; double d;
    ~Plane(){};
};

class Sphere: public Shapes {
public:
    Sphere(Point c, double r);
    Vec norm(Point intersection) override;
    int intersect(Ray& r) override;
    Point center;
    double radius;
    ~Sphere(){};
};

//used for point lights
class Dot: public Shapes {
public:
    ~Dot(){};
    int intersect(Ray& r) override;
    Vec norm(Point intersection) override;
    Vec normVec;
    void setNormVec(Vec norm);
    Vec getNormVec();
    Point loc;
    Dot() { loc = Point(0.0,0.0,0.0); normVec = Vec(0.0,0.0,0.0); }
    Dot(Point l);
    Dot(const Dot &d);
};

class Triangle: public Plane {
public:
    double area;
    Dot v1; Dot v2; Dot v3;
    int intersect(Ray& r) override;
    void setNorm();
    Triangle(Dot v1, Dot v2, Dot v3);
    Material getColor(Point baryCoord) { return baryCoord.x*v1.material + baryCoord.y*v2.material + baryCoord.z*v3.material;}
};

#endif //SHAPES_H
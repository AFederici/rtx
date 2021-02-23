#ifndef SHAPES_H
#define SHAPES_H
#include "Triplet.h"
#include <iostream>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <string>

#define DEFAULT_MATERIAL_CONST 0.5
#define T_MIN 0
#define T_MAX 100

using namespace std; 
class Ray; 
class Shapes {
public:
    double material_const;
    virtual Color get_shading(Ray &r, int ind) = 0;
    virtual int intersect(Ray& r) = 0;
    virtual Vec norm(Point intersection) = 0;
    virtual ~Shapes() {};
    template<typename T>
    bool isA() {
        return (dynamic_cast<T*>(this) != NULL);
    }
};

struct hit {
    double t;
    Point p; //needed for bary mostly
    Shapes * shape_obj;
    Vec normal;
    bool front_face;
};

class Ray { 
public:
    Ray(Point origin, Vec d);
    Point mult(double t);
    inline void set_normal(Vec outward_normal, int ind) {
        hits[ind].front_face = (dir.dot(outward_normal) < 0.0);
        hits[ind].normal = hits[ind].front_face ? outward_normal : -1.0*outward_normal;
    }
    inline Vec& getDir(){ return dir; };
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
    Plane(double ax, double by, double cz, double dShift, double material);
    Plane() {};
    int intersect(Ray& r) override;
    void setNorm();
    void setPoints();
    Vec norm(Point intersection) override;
    double a; double b; double c; double d;
    Color get_shading(Ray &r, int ind) override;
    ~Plane(){};
};

class Sphere: public Shapes {
public:
    Sphere(Point c, double r);
    Sphere(Point c, double r, double material);
    Vec norm(Point intersection) override;
    int intersect(Ray& r) override;
    Point center;
    double radius;
    Color get_shading(Ray &r, int ind) override;
    ~Sphere(){};
};

class Triangle: public Plane {
public:
    double area;
    Color c1; Color c2; Color c3;
    int intersect(Ray& r) override;
    Color get_shading(Ray &r, int ind) override;
    void setColoring(Color c1, Color c2, Color c3);
    Triangle(Point v1, Point v2, Point v3);
    Triangle(Point v1, Point v2, Point v3, double material);
};

class Dot: public Shapes {
public:
    ~Dot(){};
    int intersect(Ray& r) override;
    Vec norm(Point intersection) override;
    Point loc;
    Dot(Point l);
    Color get_shading(Ray &r, int ind) override;
};



#endif //SHAPES_H
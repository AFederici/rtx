#ifndef TRIPLET_H
#define TRIPLET_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <cstdlib>

using namespace std;

#define MAX_COLOR 255.0

//handle div by zero
#define zeroDiv(d) if (d == 0) { cerr << "div by zero" << endl; exit(1); }

double static const pi = 3.1415926535897932385;

//get a random double between 0 and 1
inline double rrand() { return rand() / (RAND_MAX + 1.0); }

//used in equality checks for doubles
inline bool close(double d, double target = 0.0){ return ((d >= (target - 1e-5)) && (d <= (target + 1e-5))); }
inline double degrees_to_radians(double degrees) { return degrees * (double)pi / 180.0; }

//generic class for a container of 3 doubles
class Triplet {
public:
    double x;
    double y;
    double z; 
    double w; //unused as of now
    Triplet(double p1, double p2, double p3);
    Triplet(int p1, int p2, int p3);
    Triplet(double p1, int p2, int p3);
    Triplet(int p1, double p2, int p3);
    Triplet(int p1, int p2, double p3);
    Triplet(int p1, double p2, double p3);
    Triplet(double p1, int p2, double p3);
    Triplet(double p1, double p2, int p3);
	Triplet();
    Triplet(const Triplet& t);
    vector<int> round(); //returns a vec of x,y,z rounded to their neares ints
    double& operator[](int ind);
    void operator*=(double d);
    void operator/=(double d);
    void operator+=(double d);
    void operator-=(double d);
protected:
    friend Triplet operator*(const Triplet& t, double d ) { return Triplet(t.x * d, t.y * d, t.z * d); }
    friend Triplet operator*(double d, const Triplet& t) { return t*d; }
    friend Triplet operator/(const Triplet& t, double d) { zeroDiv(d); return t*(1/d); }
    friend Triplet operator*(const Triplet& t1, const Triplet& t2) { return Triplet(t1.x * t2.x, t1.y * t2.y, t1.z * t2.z); }
    friend Triplet operator/(const Triplet& t1, const Triplet& t2) { zeroDiv(t2.x);zeroDiv(t2.y);zeroDiv(t2.z); return Triplet(t1.x / t2.x, t1.y / t2.y, t1.z / t2.z); }
    friend Triplet operator+(const Triplet& t, double d) { return Triplet(t.x + d, t.y + d, t.z + d); }
    friend Triplet operator+(double d, const Triplet& t) { return t + d; }
    friend Triplet operator+(const Triplet& t1, const Triplet& t2) { return Triplet(t1.x + t2.x, t1.y + t2.y, t1.z + t2.z); }
    friend Triplet operator-(const Triplet& t, double d) { return t + (-1*d); }
    friend Triplet operator-(const Triplet& t1, const Triplet& t2) { return t1 + (-1*t2); }
    friend bool operator==(const Triplet& t1, const Triplet& t2) { return close(t1.x,t2.x) && close(t1.y,t2.y) && close(t1.z,t2.z); }

};

//extends triplet with functions unique to a vector
//functions do exactly as their names
class Vec: public Triplet {
public:
    Vec(double p1, double p2, double p3);
    Vec(int p1, int p2, int p3);
    Vec(double p1, int p2, int p3);
    Vec(int p1, double p2, int p3);
    Vec(int p1, int p2, double p3);
    Vec(int p1, double p2, double p3);
    Vec(double p1, int p2, double p3);
    Vec(double p1, double p2, int p3);
    Vec(const Triplet &t);
	Vec()=default;
    void normalize();
    double collapse(); //sum the components
    double collapse(Vec &v);
    double len(); 
    double dot(Vec& v2);
    Vec cross(Vec& v2);
    friend ostream& operator<<(ostream& out, const Vec& v);
};

//triplets have no real meaning so Points are used instead
//honestly not much different but would be too much work to deprecate as of now
class Point: public Triplet {
public:
    Point(double p1, double p2, double p3);
    Point(int p1, int p2, int p3);
    Point(double p1, int p2, int p3);
    Point(int p1, double p2, int p3);
    Point(int p1, int p2, double p3);
    Point(int p1, double p2, double p3);
    Point(double p1, int p2, double p3);
    Point(double p1, double p2, int p3);
    Point(const Triplet &t);
	Point()=default;
    void operator-=(const Vec& v);
    void operator+=(const Vec& v);
    friend ostream& operator<<(ostream& out, const Point& p);
};

//adds unique properties that r,g,b should have
class Color: public Triplet {
public:
    int samples; //used to keep track of how many colors are combined in sampling
    double r(); //returns x value clamped between [0,255] after dividing value by num of samples
    double g(); //returns y value clamped between [0,255] after dividing value by num of samples
    double b(); //returns z value clamped between [0,255] after dividing value by num of samples
    void addSample(Color c); //adds together colors and keeps track of how many were added to allow for averaging
    void scale(); //clamps all vals between [0,255] and then scales down to [0,1]. Resets samples to be 1
    Color(double p1, double p2, double p3);
    Color(double p1, double p2, double p3, int s);
    Color(double p1, int p2, int p3);
    Color(int p1, double p2, int p3);
    Color(int p1, int p2, double p3);
    Color(int p1, double p2, double p3);
    Color(double p1, int p2, double p3);
    Color(double p1, double p2, int p3);
    Color(int p1, int p2, int p3);
    Color(const Triplet &t);
    void operator-=(const Color& v);
    void operator+=(const Color& v);
	Color()=default;
    void print(); //pass original values to ostream instead of scaling
    friend ostream& operator<<(ostream& out, const Color& v);
};

Color scale(Color c); //bring values within [0,1]
Vec toVec(Point &t1, Point &t2); //vector in direction t1 from point t2
Vec normalize(const Vec & v);

#endif //TRIPLET_H
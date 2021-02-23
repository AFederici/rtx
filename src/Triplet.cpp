#include "../inc/Triplet.h"
Triplet::Triplet(double p1, double p2, double p3) { x = p1; y = p2; z = p3;}
Triplet::Triplet(int p1, int p2, int p3) { x = 1.0*p1; y = 1.0*p2; z = 1.0*p3;}
Triplet::Triplet() { x = 0.0; y = 0.0; z = 0.0; }
Triplet::Triplet(const Triplet& t) { x = t.x, y=t.y;z=t.z;}
Color::Color(double p1, double p2, double p3) : Triplet(p1,p2,p3){ samples = 1;}
Color::Color(double p1, double p2, double p3, int s) : Triplet(p1,p2,p3){ samples = s;}
Color::Color(int p1, int p2, int p3): Triplet(p1,p2,p3){ samples = 1;}
Color::Color(const Triplet &t) : Triplet(t) { samples = 1; }
Vec::Vec(double p1, double p2, double p3): Triplet(p1,p2,p3){}
Vec::Vec(int p1, int p2, int p3): Triplet(p1,p2,p3){}
Vec::Vec(const Triplet &t) : Triplet(t) {}
Point::Point(double p1, double p2, double p3): Triplet(p1,p2,p3){}
Point::Point(int p1, int p2, int p3): Triplet(p1,p2,p3){}
Point::Point(const Triplet &t): Triplet(t) {}
vector<int> Triplet::round(){ vector<int> v; v.push_back(static_cast<int>(x)); 
                                             v.push_back(static_cast<int>(y)); 
                                             v.push_back(static_cast<int>(z)); return v;}
double Color::r() { return x; }
double Color::g() { return y; }
double Color::b() { return z; }

double& Triplet::operator[](int ind){
    if (ind == 0) return x;
    if (ind == 1) return y;
    if (ind == 2) return z;
    cerr << "bad index" << endl;
    exit(1);
}
void Triplet::operator*=(double d){ 
    x *= d; y *= d; z *= d;
}
void Triplet::operator/=(double d){
    zeroDiv(d); x /= d; y /= d; z /= d;
}
void Triplet::operator+=(double d){ x += d; y += d; z += d;}
void Triplet::operator-=(double d){ x -= d; y -= d; z -= d;}

double Vec::collapse() { return x + y + z; }
double collapse(Vec & v) { return v.collapse(); }
void Vec::normalize() { zeroDiv(len()); (*this) /= len(); }
Vec normalize(const Vec & v) { Vec v2(v); v2.normalize(); return v2; }
double Vec::len() { return sqrt(Vec((*this) * (*this)).collapse()); }
double Vec::dot(Vec& v2) { return (static_cast<Vec>((*this) * v2)).collapse(); } //dont know why compiler wanted cast here
Vec Vec::cross(Vec& v2) { 
    return Vec(y * v2[2] - z * v2[1],
                z * v2[0] - x * v2[2],
                x * v2[1] - y * v2[0]);
}
ostream& operator<<(ostream& out, const Vec& v) { 
    char buf[256]; 
    sprintf(buf, "%.2f %.2f %.2f", v.x, v.y, v.z);
    out << string(buf) << endl;
    return out;
}

Vec toVec(Point &t1, Point &t2) { 
    return Vec(t1.x-t2.x,t1.y-t2.y,t1.z-t2.z);
}

void Point::operator-=(const Vec& v) { x -= v.x; y-=v.y; z-=v.z;}
void Point::operator+=(const Vec& v) { (*this) -= (-1*v);}
ostream& operator<<(ostream& out, const Point& p){
    char buf[256]; 
    sprintf(buf, "%.2f %.2f %.2f", p.x, p.y, p.z);
    out << string(buf) << endl;
    return out;
}

void Color::operator+=(const Color& c) { x += c.x; y += c.y; z += c.z; }
void Color::addSample(Color c) { (*this) += c; samples++;}
void Color::scale() { 
    (*this) /= (1.0 * samples);
    (*this) *= (MAX_COLOR+0.999);
}
void Color::print(){
    char buf[256]; 
    sprintf(buf, "%.2f %.2f %.2f", x, y, z);
    cout << string(buf) << endl;
}

Color scale(Color c){ return (Color(c) / (1.0 * c.samples)) * (MAX_COLOR+0.999); }

ostream& operator<<(ostream& out, const Color& c){
    Color c2 = Color(c.x, c.y, c.z, c.samples);
    c2.scale();
    out << static_cast<int>(c2.r()) << ' '
        << static_cast<int>(c2.g()) << ' '
        << static_cast<int>(c2.b()) << '\n';
    return out;
}



#include "../inc/Shapes.h"


Sphere::Sphere(Point c, double r){
    material_const = DEFAULT_MATERIAL_CONST;
    center = c;
    radius = r;
}
Sphere::Sphere(Point c, double r, double material) {center = c; radius = r; material_const = material;}

Color Sphere::get_shading(Ray &r, int ind){
    return 0.5*Color(r.hits[ind].normal.x+1, r.hits[ind].normal.y+1, r.hits[ind].normal.z+1);
}

double sign(double b){ return (-b > 0) ? 1.0 : -1.0; }


//https://en.wikipedia.org/wiki/Line–sphere_intersection
int Sphere::intersect(Ray& r){
    hit h1, h2; h1.shape_obj = this; h2.shape_obj = this; h1.t = -1; h2.t = -1;
    int ret = 0;
    Vec oc = r.o - center; //f in notes
    Vec dHat = normalize(r.dir);
    Vec l = oc - (oc.dot(dHat)*dHat);
    double b = 2.0 * (oc.dot(r.dir));
    double a = (r.dir).dot(r.dir);
    double discrim = 4.0 * a * ((radius * radius) - (l.dot(l))); //(b**2 - 4ac)
    double c = (-discrim + (b*b)) / (4.0*a); //-(discrim / 4a) + b**2/4a= c 
    double q = -0.5 * (b + (sign(b)*sqrt(discrim)));
    if (close(discrim)) { h1.t = -b / (2.0*a); }//one intersection
    else if (discrim > 0){ //two intersection points
        h1.t = (-b - sqrt(discrim)) / (2.0*a); 
        //h2.t = q / a; 
        //if ((h1.t > h2.t) && (h2.t >= T_MIN)) h1.t = (T_MIN) - 1;
        //else h2.t = (T_MIN) - 1;
    }
    if ((h1.t >= T_MIN) && (h1.t <= T_MAX)) { h1.p = r.mult(h1.t); r.hits.push_back(h1); r.set_normal(norm(h1.p), r.hits.size() - 1); ret++;}
    if ((h2.t >= T_MIN) && (h2.t <= T_MAX)) { h2.p = r.mult(h2.t); r.hits.push_back(h2); r.set_normal(norm(h2.p), r.hits.size() - 1); ret++;}
    return ret;
}


Vec Sphere::norm(Point intersection){
    zeroDiv(radius); 
    Vec v = toVec(intersection, center); 
    v.normalize(); 
    return v;
}

//pass in the norm, kinda ugly rn
Color Plane::get_shading(Ray &r, int ind){
    return Color(1,0,0);
}

//https://math.stackexchange.com/questions/1755856/calculate-arbitrary-points-from-a-plane-equation
void Plane::setPoints(){
    double denom = (a*a) + (b*b) + (c*c);
    p3 = Point(a*d, b*d, c*d) / denom;
    p1 = p3 + Point(c-b, a-c, b-a);
    p2 = p3 + Point((a*b) + (a*c) - (b*b) - (c*c), (b*a) + (b*c) - (a*a) - (c*c), (c*a) + (c*b) - (a*a) - (b*b));
}

void Plane::setNorm(){
    e1 = toVec(p3,p2);
    e2 = toVec(p1,p3);
    e3 = toVec(p2,p1);
    nhat = normalize(e1.cross(e2));
}

Plane::Plane(double ax, double by, double cz, double dShift) {
    a = ax; b = by; c = cz; d = dShift; material_const = DEFAULT_MATERIAL_CONST;
    setPoints(); setNorm();
}

Plane::Plane(double ax, double by, double cz, double dShift, double material) {
     a = ax; b = by; c = cz; d = dShift; material_const = material;
     setPoints(); setNorm();
}


int Plane::intersect(Ray& r){
    hit h1; h1.shape_obj = this;
    double denom = r.dir.dot(nhat);
    if (close(denom)) return 0;
    h1.t = toVec(p3,r.o).dot(nhat) / denom; h1.p = r.mult(h1.t);
    if ((h1.t >= T_MIN) && (h1.t <= T_MAX)) { 
        r.hits.push_back(h1);
        r.set_normal(nhat, r.hits.size() - 1); 
        return 1; 
    }
    return 0;
}

int Triangle::intersect(Ray &r){
    /*
    Vec e1 = toVec(p1,p3);
    Vec e2 = toVec(p2,p3);
    Vec q = r.dir.cross(e2);
    double a = e1.dot(q);
    if (a > -1e-5 && a < 1e-5) return 0;
    double f = 1.0 / a;
    Vec s = toVec(r.o,p3);
    double u = f * (s.dot(q));
    if (u < 0.0) return 0;
    Vec rv = s.cross(e1);
    double v = f * r.dir.dot(rv);
    if (v < 0.0 || (u + v) > 1.0) return 0;
    double t = f * (e2.dot(rv));
    hit h1; h1.normal = norm(Point(p1)); h1.t = t; h1.p = Point(u,v,1.0-u-v); h1.shape_obj = this;
    r.hits.push_back(h1);
    return 1;
    */
    if (!Plane::intersect(r)) return 0;
    int ind = r.hits.size() - 1;
    Point p = r.hits[ind].p;
    Vec d1 = toVec(p,p1); Vec d3 = toVec(p,p3); 
    double at1 = (e1.cross(d3).dot(nhat) / 2.0) / area;
    double at2 = (e2.cross(d1).dot(nhat) / 2.0) / area;
    double at3 = 1 - (at1 + at2);
    r.hits[ind].p = Point(at1, at2, at3);
    if ((r.hits[ind].p.x < 0.0 || r.hits[ind].p.y < 0.0 || r.hits[ind].p.z < 0.0)) { r.hits.pop_back(); return 0; }
    r.hits[ind].shape_obj = this;
    return 1;
}

Vec Plane::norm(Point unused){ return nhat;}

Triangle::Triangle(Point v1, Point v2, Point v3) { p1 = v1; p2 = v2; p3 = v3; material_const = DEFAULT_MATERIAL_CONST; setNorm(); area = e1.cross(e2).dot(nhat) / 2; }
Triangle::Triangle(Point v1, Point v2, Point v3, double material) { p1 = v1; p2 = v2; p3 = v3; material_const = material; setNorm(); area = e1.cross(e2).dot(nhat) / 2; }
void Triangle::setColoring(Color col1, Color col2, Color col3){ c1 = col1; c2 = col2; c3 = col3; }
Color Triangle::get_shading(Ray &r, int ind){ return (r.hits[ind].p.x * c1) + (r.hits[ind].p.y * c2) + (r.hits[ind].p.z * c3); }
Ray::Ray(Point origin, Vec d){ o = origin; dir = Vec(d);}
Point Ray::mult(double t){ return o + (t*dir); }
int Dot::intersect(Ray &r) { return 0;}
Vec Dot::norm(Point intersection) { return Vec();}
Dot::Dot(Point l) { loc = l; };
Color Dot::get_shading(Ray &r, int ind){ return Color(212.0/255.0,235.0/255.0,255.0/255.0); }
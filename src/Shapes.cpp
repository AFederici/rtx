#include "../inc/Shapes.h"


Sphere::Sphere(Point c, double r){
    center = c;
    radius = r;
    aabb.push_back(center - radius);
    aabb.push_back(center + radius);
    aabb.push_back(center);
    aabb.push_back(center);
}

//Color Sphere::get_shading(Ray &r, int ind){
//    return 0.5*Color(r.hits[ind].normal.x, r.hits[ind].normal.y, r.hits[ind].normal.z);
//}

//if ortho use this one
/*
Color Sphere::get_shading(Ray &r, int ind){
    return Color(0.0,0.0,0.8);
}*/

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
        h1.t = c / q;
        h2.t = q / a;
        //cerr << to_string(h1.t) << " " << to_string(h2.t) << endl; 
        //if ((h1.t > h2.t) && (h2.t >= T_MIN)) h1.t = (T_MIN) - 1;
        //else h2.t = (T_MIN) - 1;
    }
    if ((h1.t >= T_MIN) && (h1.t <= T_MAX)) { h1.p = r.mult(h1.t); r.hits.push_back(h1); r.set_normal(norm(h1.p), r.hits.size() - 1); ret++;}
    if ((h2.t >= T_MIN) && (h2.t <= T_MAX)) { h2.p = r.mult(h2.t); r.hits.push_back(h2); r.set_normal(norm(h2.p), r.hits.size() - 1); ret++;}
    return ret;
}


Vec Sphere::norm(Point intersection){
    //cerr << "radius " << to_string(radius) << endl;
    zeroDiv(radius); 
    Vec v = toVec(center, intersection); 
    v.normalize(); 
    return v;
}

//https://math.stackexchange.com/questions/1755856/calculate-arbitrary-points-from-a-plane-equation
void Plane::setPoints(){
    double denom = (a*a) + (b*b) + (c*c);
    p3 = -1*Point(a*d, b*d, c*d) / denom;
    p1 = p3 + Point(c-b, a-c, b-a);
    p2 = p3 + Point((a*b) + (a*c) - (b*b) - (c*c), (b*a) + (b*c) - (a*a) - (c*c), (c*a) + (c*b) - (a*a) - (b*b));

}

void Plane::setNorm(){
    e1 = toVec(p2,p3);
    e2 = toVec(p3,p1);
    e3 = toVec(p1,p2);
    nhat = normalize(e1.cross(e2));
    if (e1.cross(e2).dot(nhat) < 0){
        Point temp = Point(p1);
        p1 = p3;
        p3 = temp;
        setNorm();
    }
}

void Triangle::setNorm(){
    e1 = toVec(p2,p3);
    e2 = toVec(p3,p1);
    e3 = toVec(p1,p2);
    nhat = normalize(e1.cross(e2));
    if (e1.cross(e2).dot(nhat) < 0){
        Point temp = Point(p1);
        p1 = p3;
        p3 = temp;
        Dot tempD = Dot(v1);
        v1 = v3;
        v3 = tempD;
        setNorm();
    }
}

Plane::Plane(double ax, double by, double cz, double dShift) {
    a = ax; b = by; c = cz; d = dShift;
    setPoints(); setNorm();
}

int Plane::intersect(Ray& r){
    hit h1; h1.shape_obj = this;
    double denom = nhat.dot(r.dir);
    //cerr << to_string(denom) << endl;
    if (close(denom)) return 0;
    h1.t = Vec(p3-r.o).dot(nhat) / denom; 
    h1.p = r.mult(h1.t);
    //cerr << to_string(h1.t) << endl;
    if ((h1.t >= T_MIN) && (h1.t <= T_MAX)) { 
        r.hits.push_back(h1);
        r.set_normal(nhat, r.hits.size() - 1); 
        return 1; 
    }
    return 0;
}

int Triangle::intersect(Ray &r){

    // course notes
    Vec e1 = toVec(p1,p2);
    Vec e2 = toVec(p1,p3);
    Vec nr = normalize(r.dir);
    Vec q = nr.cross(e2);
    double a = e1.dot(q);
    if (close(a)) return 0;
    double f = 1.0 / a;
    Vec s = toVec(p1,r.o);
    double u = f * (s.dot(q));
    if (u < 0.0) return 0;
    Vec rv = s.cross(e1);
    double v = f * nr.dot(rv);
    if (v < 0.0 || (u + v) > 1.0) return 0;
    double t = f * (e2.dot(rv));
    hit h1; h1.normal = norm(Point(p1)); 
    h1.t = t; 
    h1.p = Point(u,v,1.0-u-v); 
    h1.shape_obj = this;
    r.hits.push_back(h1);
    return 1;
    
/*
    if (!Plane::intersect(r)) return 0;
    int ind = r.hits.size() - 1;
    Point p = r.hits[ind].p;
    //cerr << p << endl;
    Vec d1 = toVec(p1,p); Vec d3 = toVec(p3,p); 
    double at1 = (e1.cross(d3).dot(nhat) / 2.0) / area;
    double at2 = (e2.cross(d1).dot(nhat) / 2.0) / area;
    double at3 = 1 - (at1 + at2);
    r.hits[ind].p = Point(at1, at2, at3);
    //cerr << r.hits[ind].p << endl;
    if ((r.hits[ind].p.x < 0.0 || r.hits[ind].p.y < 0.0 || r.hits[ind].p.z < 0.0)) { r.hits.pop_back(); return 0; }
    r.hits[ind].shape_obj = this;
    return 1;
    */
}

Vec Plane::norm(Point unused){ return nhat;}

Triangle::Triangle(Dot d1, Dot d2, Dot d3) { 
    v1 = Dot(d1); v2 = Dot(d2); v3 = Dot(d3); 
    p1 = v1.loc; p2 = v2.loc; p3 = v3.loc;
    setNorm(); 
    area = e1.cross(e2).dot(nhat) / 2; 
    Point smaller = Point(p1);
    Point bigger = Point(p1);
    smaller.x = min(min(smaller.x, p2.x), p3.x);
    smaller.y = min(min(smaller.y, p2.y), p3.y);
    smaller.z = min(min(smaller.z, p2.z), p3.z);
    bigger.x = max(max(bigger.x, p2.x), p3.x);
    bigger.y = max(max(bigger.y, p2.y), p3.y);
    bigger.z = max(max(bigger.z, p2.z), p3.z);
    Point centroid = smaller + ((bigger-smaller) / 2.0);   
    aabb.push_back(smaller);
    aabb.push_back(bigger);
    aabb.push_back(centroid);
    aabb.push_back(centroid);
}
//Color Triangle::get_shading(Ray &r, int ind){ return ((r.hits[ind].p.x * c1) + (r.hits[ind].p.y * c2) + (r.hits[ind].p.z * c3)) / 3.0; }
Ray::Ray(Point origin, Vec d){ o = origin; dir = Vec(d);}
Point Ray::mult(double t){ return o + (t*dir); }
int Dot::intersect(Ray &r) { return 0;}
Vec Dot::norm(Point intersection) { return Vec();}
void Dot::setNormVec(Vec norm) { normVec = norm; }
Vec Dot::getNormVec() { return normVec; }
Dot::Dot(Point l) { loc = l; };
Dot::Dot(const Dot &d) { loc = d.loc; material = d.material;}
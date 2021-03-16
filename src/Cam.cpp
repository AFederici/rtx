#include "../inc/Cam.h"

void Cam::basis(){ 
    w = toVec(lookAt, eye); w.normalize();
    u = up.cross(w); u.normalize(); 
    v = w.cross(u);
}
void Cam::setBasis(Vec vu, Vec vv, Vec vw){
    u = normalize(vu); v = normalize(vv); w = normalize(vw);
}
Cam::Cam() { eye = Point(0,0,0); lookAt = Point(0,0,-1); up = Vec(0,1,0); basis(); }
Cam::Cam(Vec u, Point e, Point l) { up = u; eye = e; lookAt = l; basis(); }
#include "../inc/Cam.h"

void Cam::basis(){ 
    w = toVec(eye, lookAt); w.normalize();
    u = up.cross(w); u.normalize(); 
    v = w.cross(u);
}
Cam::Cam() { eye = Point(0,0,10); lookAt = Point(0,0,-1); up = Vec(0,1,0); basis(); }
Cam::Cam(Vec u, Point e, Point l) { up = u; eye = e; lookAt = l; basis(); }
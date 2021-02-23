#include "../inc/Triplet.h"
#include "../inc/View.h"
#include "../inc/World.h" 

int main(){
    Cam cam = Cam();
    View v = View();
    World w(v, cam);
    //w.isOrtho = true;
    //w.setNoSampling();
    Triangle tri = Triangle(Point(-5.0,-5.0,-10.0), Point(-5.0,5.0,-10.0), Point(0.0,5.0,-10.0));
    Sphere sp = Sphere(Point(0.0,-10.5,-7.0), 10);
    Sphere sp2 = Sphere(Point(0.0,-100.5,-20.0), 100);
    Dot l1 = Dot(Point(1,100,-1));
    w.addObj(&tri); 
    w.addObj(&sp); 
    w.addObj(&sp2); 
    w.addLight(&l1);
    w.render();
    w.debugPPM();
    return 0;
}
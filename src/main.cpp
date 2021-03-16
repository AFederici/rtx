#include "../inc/Triplet.h"
#include "../inc/View.h"
#include "../inc/World.h" 
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

int main(){
    signal(SIGFPE, handler); 
    Cam cam = Cam(Vec(0,1,0), Point(0,3,3), Point(0,0,0));
    View v = View(500,500, 1, 7);
    World w(v, cam);
    //w.isOrtho = true;
    //w.setNoSampling();
    Plane p = Plane(1,10000,11,6);
    Triangle tri = Triangle(Point(-5.0,-5.0,-10.0), Point(-5.0,5.0,-10.0), Point(0.0,5.0,-10.0));
    tri.setColoring(Color(0,1,0), Color(1,1,1), Color(0,1,0));
    Sphere sp = Sphere(Point(0.0,-10.5,-3.0), 10);
    Sphere sp2 = Sphere(Point(-20.0,-10.5,-3.0), 10);
    Dot l1 = Dot(Point(15.0,10.0,-3.0));
    w.addObj(&tri); 
    w.addObj(&sp); 
    w.addObj(&sp2); 
    w.addObj(&p);
    w.addLight(&l1);
    w.render();
    w.debugPPM();
    return 0;
}


/* looked dec
Plane p = Plane(1,10000,11,6);
    Triangle tri = Triangle(Point(-5.0,-5.0,-10.0), Point(-5.0,5.0,-10.0), Point(0.0,5.0,-10.0));
    tri.setColoring(Color(0,1,0), Color(1,1,1), Color(0,1,0));
    Sphere sp = Sphere(Point(0.0,-10.5,-3.0), 10);
    Sphere sp2 = Sphere(Point(-20.0,-10.5,-3.0), 10);
    Dot l1 = Dot(Point(15.0,10.0,-3.0));
    */

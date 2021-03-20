#include "../inc/Triplet.h"
#include "../inc/View.h"
#include "../inc/World.h" 
#include "../inc/Mesh.h"
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

//mesh main
int main(){
    signal(SIGFPE, handler); 
    //View v = View(400, 400, .1, 1); //width, height
    View v = View(400, 400, .5, 1); //width, height
    Cam c = Cam(Point(0,0,75), Point(0,0,0), Vec(0,1,0));

    World w = World(v,c);
    Dot l = Dot(Point(0,100,1));
    l.addShader("diffuse", Color(0.3,0.3,0.3));
    l.addShader("specular", Color(0.3,0.3,0.3));
    w.addLight(&l);
    w.ambience = 0.1;
    w.sampling_rate = 64;
    Color col = Color(171,164,144);
    col.scale();
    srand( time(NULL) );
    Mesh tm = Mesh();
    tm.loadMesh("dragon.obj.txt");
    cerr << " mesh faces " << tm.faces.size() << endl;
    for (int v = 0; v < tm.faces.size(); v++){ w.addObj(tm.faces[v]); }
    w.isAccel = true;
    w.render();
    w.debugPPM();
    return 0;
}

//spheres main
/*
int main(){
    signal(SIGFPE, handler); 
    View v = View(300, 200, .75, 4); //width, height
    Cam c = Cam(Point(0,0,50), Point(0,0,0), Vec(0,1,0));
    World w = World(v,c);
    Dot l = Dot(Point(0,0,-2));
    l.addShader("diffuse", Color(0.5,0.5,0.5));
    l.addShader("specular", Color(0.3,0.3,0.3));
    w.addLight(&l);
    w.ambience = 0.3;
    w.sampling_rate = 25;
    srand( time( NULL ) );
    for (int i = 0; i < 100000; i++){
      double x = 250*rrand() - 125;
      double y = 180*rrand() - 90;
      double z = 20 + 70*rrand() - 3;
      Sphere * s = new Sphere(Point(x,y,-z), 4);
      Color c = Color(rrand(), rrand(), rrand());
      s->addShader("ambient", c);
      s->addShader("diffuse", c / 2);
      s->addShader("specular", c / 2);
      s->addShader("shiny", Color(.3,.3,.3));
      w.addObj(s);
    }
    w.isAccel = true;
    w.render();
    w.debugPPM();
    return 0;
}
*/

/* looked dec

int main(){
    signal(SIGFPE, handler); 
    Cam cam = Cam(Vec(0,1,0), Point(0,3,3), Point(0,0,0));
    View v = View(500,500, 1, 7);
    World w(v, cam);
    //w.isOrtho = true;
    //w.setNoSampling();
    Plane p = Plane(1,10000,11,6);
    Triangle tri = Triangle(Point(-5.0,-5.0,-10.0), Point(-5.0,5.0,-10.0), Point(0.0,5.0,-10.0));
    //tri.setColoring(Color(0,1,0), Color(1,1,1), Color(0,1,0));
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

Plane p = Plane(1,10000,11,6);
    Triangle tri = Triangle(Point(-5.0,-5.0,-10.0), Point(-5.0,5.0,-10.0), Point(0.0,5.0,-10.0));
    tri.setColoring(Color(0,1,0), Color(1,1,1), Color(0,1,0));
    Sphere sp = Sphere(Point(0.0,-10.5,-3.0), 10);
    Sphere sp2 = Sphere(Point(-20.0,-10.5,-3.0), 10);
    Dot l1 = Dot(Point(15.0,10.0,-3.0));
    */

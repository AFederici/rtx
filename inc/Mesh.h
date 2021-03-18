#include "Shapes.h"
#include <map>

class TriangleMesh: public Triangle {
public:
    Dot * vertex1;
    Dot * vertex2;
    Dot * vertex3;
    TriangleMesh(Dot *v1, Dot *v2, Dot *v3) : Triangle(Dot(*v1),Dot(*v2),Dot(*v3)){ 
        vertex1 = v1; vertex2 = v2; vertex3 = v3;
    };
    Vec norm(Point baryC) override;
};

class Mesh {
    public:
        vector<Dot*> verts;
        map<int, vector<int> > adjFaces;
        map<int, Vec> computed;
        vector<Triangle*> faces;
        void loadMesh(const char *filename);
        void setNorms();
        Mesh() {};
};
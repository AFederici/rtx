#include "Shapes.h"
#include <map>

//sub-class of the triangle to allow for special properties for triangles in a mesh
class TriangleMesh: public Triangle {
public:
    //Difference from triangle contructor are three vertex pointers used in interpolation
    Dot * vertex1;
    Dot * vertex2;
    Dot * vertex3;
    TriangleMesh(Dot *v1, Dot *v2, Dot *v3) : Triangle(Dot(*v1),Dot(*v2),Dot(*v3)){ 
        vertex1 = v1; vertex2 = v2; vertex3 = v3;
    };

    //uses barycentric interpolation of the triangle per-vertex normal 
    //instead of the normal of the plane the triangle is contained in
    //
    //input: barycentric coordinates, which are stored in a ray's hitpoint variable p
    Vec norm(Point baryC) override;
};

//Class to manage a Collection of TriangleMesh objects
class Mesh {
    public:
        vector<Dot*> verts; //List of vertices read in from obj
        vector<Triangle*> faces; //List of TriangleMeshes created. Uses parent class to make type-checking simpler
        /*
        * Reads from an obj file
        * Directly applies Material properties and affine translation while reading from file
        * This function also initializes the verts and faces vectors.
        * After the file is read, per-vertex normals are caluclated based the adjacent faces
        * normals, weighted by area. The usage of shape pointers allows for the manipulation
        * of the vertices normals to directly be accessed by the TriangleMesh objects.
        */
        void loadMesh(const char *filename); 
        Mesh() {};
};
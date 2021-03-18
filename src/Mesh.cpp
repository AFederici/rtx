#include "../inc/Mesh.h"

Vec TriangleMesh::norm(Point baryC) {
    return baryC.x*vertex1->getNormVec() + baryC.y*vertex2->getNormVec() + baryC.z*vertex3->getNormVec();
}

//https://stackoverflow.com/questions/23319490/loading-obj-file-using-c-and-opengl
void Mesh::loadMesh(const char *filename) 
{
   string s;
   ifstream fin(filename);
   Color col = Color(171,164,144);
   Color dif = Color(.1, .9*rrand(), .1);
   Color spec = Color(.3,.3,.3);

   col.scale();
   if(!fin) {cerr << " no file - " << filename << endl; return; }
   while(fin>>s)
   {
      switch(*s.c_str())
      {
      case 'v':
            {
                  
                  Dot * v = new Dot();
                  v->addShader("ambient", col);
                  v->addShader("diffuse", dif);
                  v->addShader("specular", spec);
                  v->addShader("shiny", Color(0.3,.3,.3));
                  fin>>(v->loc.x)>>(v->loc.y)>>(v->loc.z);
                  
                  verts.push_back(v);
                  
            }
            break;            
      case 'f':
            {
                  int v1, v2, v3;
                  fin>>v1>>v2>>v3;
                  //cerr << "1";
                  TriangleMesh * t = new TriangleMesh(verts[v1-1],verts[v2-1],verts[v3-1]);
                  t->vertex1->setNormVec(t->vertex1->getNormVec() + 0.5*t->nhat);
                  t->vertex2->setNormVec(t->vertex2->getNormVec() + 0.5*t->nhat);
                  t->vertex3->setNormVec(t->vertex3->getNormVec() + 0.5*t->nhat);
                  //cerr << "2";
                  faces.push_back(t);
            }
            break;
      }
   }
   cerr << "3";
   for (int v = 0; v < verts.size(); v++){
         verts[v]->setNormVec(normalize(verts[v]->getNormVec()));
   }     
}

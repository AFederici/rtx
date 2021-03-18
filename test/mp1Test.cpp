#include <iostream>
#include "../inc/World.h"
#include <gtest/gtest.h>

/*
testing::internal::CaptureStderr();
std::string output = testing::internal::GetCapturedStderr();
  ofstream fs;
  fs.open("test.log", ios::out | ios::app);
  fs << output;
  fs.close();
  */
TEST (Triplet, testIntDivision)
{
  Triplet t1 = Triplet(2.2,4.2,3.5);
  Triplet t2 = Triplet(2,2,5);
  Triplet t3 = t1 / t2;
  EXPECT_EQ(t3, Triplet(1.1,2.1,0.7));
}
TEST (Triplet, testZeroDiv)
{
    Triplet t1 = Triplet(2.2,4.2,3.5);
    double d = 0;
    ASSERT_EXIT(t1 / d, ::testing::ExitedWithCode(1), ".*");
}
TEST (Triplet, testRound)
{
  Triplet t1 = Triplet(2.2,4.2,3.5);
  vector<int> v = t1.round();
  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 4);
  EXPECT_EQ(v[2], 3);
}

TEST (Triplet, testMult)
{
  Triplet t1 = Triplet(2.2,4.2,3.5);
  t1 *= 2;
  EXPECT_EQ(t1, Triplet(4.4,8.4,7.0));
}

TEST (Triplet, testAdd)
{
  Triplet t1 = Triplet(2.2,4.2,3.5);
  t1 += 2;
  EXPECT_EQ(t1, Triplet(4.2,6.2,5.5));
}

TEST (Triplet, testSub)
{
  Triplet t1 = Triplet(2.2,4.2,3.5);
  Triplet t2 = Triplet(0.2,2.2,1.5);
  t1 -= 2;
  EXPECT_TRUE(close(t1.x, t2.x));
  EXPECT_TRUE(close(t1.y, t2.y));
  EXPECT_TRUE(close(t1.z, t2.z));
}

TEST (Vec, testSumDimmensions)
{
  Vec t1 = Vec(2.2,4.2,3.5);
  EXPECT_EQ(t1.collapse(), 9.9);
}

TEST (Vec, normalizeFunc)
{
  Vec t1 = Vec(3,3,3);
  t1.normalize();
  Vec t2= Vec(1.0/sqrt(3),1.0/sqrt(3),1.0/sqrt(3));
  EXPECT_EQ(t1, t2);
  EXPECT_EQ(t2.len(), 1);
  EXPECT_EQ(normalize(Vec(10.0, 5, 66)).len(), 1);
}

TEST (Vec, lenFunc)
{
  Vec t1 = Vec(1,2,3);
  EXPECT_EQ(t1.len(), sqrt(14));
}

TEST (Vec, dot)
{
  Vec t1 = Vec(-12,16,1);
  Vec t2 = Vec(12,9,1);
  EXPECT_EQ(t1.dot(t2), 1);
}

TEST (Vec, cross)
{
  Vec t1 = Vec(1,2,3);
  Vec t2 = Vec(1,5,7);
  EXPECT_EQ(t1.cross(t2), Vec(-1,-4,3));
}

TEST (Vec, toVec)
{
  Point origin = Point(-5,-4,2);
  Point dest = Point(0,10,-1);
  EXPECT_EQ(toVec(origin, dest), Vec(5,14,-3));
}

TEST (Point, testAdd)
{
  Point t1 = Point(2.2,4.2,3.5);
  Point t2 = Point(2,2,2);
  t1 += t2;
  EXPECT_EQ(t1, Point(4.2,6.2,5.5));
}

TEST (Point, testSub)
{
  Point t1 = Point(2.2,4.2,3.5);
  Point t2 = Point(2,2,2);
  t1 -= t2;
  EXPECT_EQ(t1, Point(0.2,2.2,1.5));
}

TEST (Color, testAdd)
{
  Color t1 = Color(2.2,4.2,3.5);
  Color t2 = Color(2,2,2);
  t1 += t2;
  EXPECT_EQ(t1, Color(4.2,6.2,5.5));
}

TEST (Color, testSub)
{
  Color t1 = Color(2.2,4.2,3.5);
  Color t2 = Color(2,2,2);
  t1 -= t2;
  EXPECT_EQ(t1, Color(0.2,2.2,1.5));
}

TEST (Color, sampling)
{
  Color t1 = Color(1.4,0.6,0.2);
  EXPECT_EQ(t1.samples, 1);
  Color t2 = Color(1,2,0.8);
  t1.addSample(t2);
  EXPECT_EQ(t1.samples, 2);
  EXPECT_EQ(t1.r(), 255);
  EXPECT_EQ(t1.g(), 255);
  EXPECT_EQ(t1.b(), 127.5);
  EXPECT_FALSE(close(t1.x, 1));
  EXPECT_FALSE(close(t1.y, 1));
  EXPECT_FALSE(close(t1.z, 0.5));
  t1.scale();
  EXPECT_EQ(t1.samples, 1);
  //after scale, the vals should be their expected without needing rgb scaling
  EXPECT_EQ(t1.r(), 255);
  EXPECT_EQ(t1.g(), 255);
  EXPECT_EQ(t1.b(), 127.5);
  EXPECT_EQ(t1.x, 1);
  EXPECT_EQ(t1.y, 1);
  EXPECT_EQ(t1.z, 0.5);
}

TEST (Color, colorCeil)
{
  Color t1 = Color(0.4,0.6,0.2);
  Color t2 = Color(1,2,0.2);
  Color t3 = t1 * t2;
  EXPECT_TRUE(close(t3.r() / 255.0, 0.4));
  EXPECT_TRUE(close(t3.g() / 255.0, 1));
  EXPECT_TRUE(close(t3.b() / 255.0, 0.04));
  EXPECT_TRUE(close(t3.x, 0.4));
  EXPECT_FALSE(close(t3.y, 1));
  EXPECT_TRUE(close(t3.z, 0.04));
}

TEST (Color, testColorRangeOverBounds)
{
  Color cPos = Triplet(275,236236,255.1);
  EXPECT_EQ(cPos.r(), 255);
  EXPECT_EQ(cPos.g(), 255);
  EXPECT_EQ(cPos.b(), 255);
}

TEST (Color, testColorRangeUnderBounds)
{
  Color cNeg = Triplet(-1,0,-5.0);
  EXPECT_EQ(cNeg.r(), 0);
  EXPECT_EQ(cNeg.g(), 0);
  EXPECT_EQ(cNeg.b(), 0);
}

TEST (Color, testColorRangeNormal)
{
  Color cReg = Triplet(.2,.4,.6);
  EXPECT_TRUE(close(cReg.r(), 51));
  EXPECT_TRUE(close(cReg.g(), 102));
  EXPECT_TRUE(close(cReg.b(), 153));
}

TEST (View, testRaysToViewPlane)
{
  View v = View(8, 6, 2, 6);
  vector<Point> p = v.getSingleRay(0, 2);
  EXPECT_EQ(p[0], Point(-7,-1,-6));
  vector<Point> p2 = v.getMultiJittered(0,2,4);
  for (int i = 0; i < p2.size(); i++){
    for (int j = i + 1; j < p2.size(); j++){
      EXPECT_NE(p2[i].x, p2[j].x);
      EXPECT_NE(p2[i].y, p2[j].z);
      EXPECT_GE(p2[i].x, -8);
      EXPECT_LE(p2[i].x, -6);
      EXPECT_GE(p2[i].y, -2);
      EXPECT_LE(p2[i].y, 0);
    }
  }
}
//http://www.ambrsoft.com/TrigoCalc/Sphere/SpherLineIntersection_.htm
TEST (Sphere, intersection){
  Ray r = Ray(Point(1,2,3), Vec(1, 0, -2));
  Sphere s = Sphere(Point(1,4,0), 4);
  EXPECT_EQ(s.intersect(r), 1);
  ASSERT_EQ(r.hits.size(), 1); //2 intersections, but one t is negative
  EXPECT_TRUE(close(std::ceil(r.hits[0].t * 100.0) / 100.0, 2.63));
  
  //intersect from the other direction
}

TEST (Sphere, testNorm){
  Sphere s = Sphere(Point(0,0,0), 1);
  Vec v = s.norm(Point(-1,0,0));
  Ray r = Ray(Point(-5,0,0), Vec(1,0,0));
  EXPECT_EQ(v, Vec(-1,0,0));
  s.intersect(r);
  ASSERT_GE(r.hits.size(), 1);
  EXPECT_EQ(r.hits[0].normal, Vec(-1,0,0));
}

TEST (Plane, testNorm){
  Plane p = Plane(3,9,1,8);
  Vec v = p.norm(Point(-1,1,2));
  EXPECT_TRUE(((v == normalize(Vec(3,9,1))) || (-1*v == normalize(Vec(3,9,1)))));
  Ray r = Ray(Point(0,0,0), Vec(0,0,-2));
  p.intersect(r);
  ASSERT_EQ(r.hits.size(), 1);
  EXPECT_EQ(r.hits[0].normal, normalize(Vec(3,9,1)));
}
//https://ocw.mit.edu/courses/mathematics/18-02sc-multivariable-calculus-fall-2010/1.-vectors-and-matrices/part-c-parametric-equations-for-curves/session-16-intersection-of-a-line-and-a-plane/MIT18_02SC_we_9_comb.pdf
TEST (Plane, intersection){
  Plane p = Plane(2,1,-4,-4);
  Ray r = Ray(Point(1,4,0), Vec(1,2,1));
  EXPECT_EQ(p.intersect(r), 0); //no intersection
  ASSERT_EQ(r.hits.size(), 0);
  Ray r2 = Ray(Point(0,2,0), Vec(1,3,1));
  EXPECT_EQ(p.intersect(r2), 1);
  ASSERT_EQ(r2.hits.size(), 1);
  EXPECT_EQ(r2.mult(r2.hits[0].t), Point(2,8,2));
  EXPECT_TRUE(close(r2.hits[0].t, 2));
  EXPECT_EQ(r2.hits[0].p, Point(2,8,2));
}
//https://stackoverflow.com/questions/31178874/how-to-get-the-intersection-point-ray-triangle-intersection-c
TEST (Triangle, intersection){
  Triangle t = Triangle(Point(0,3,0), Point(-0.5,3,-1), Point(0.5,3,-1));
  Ray r = Ray(Point(0,2,-1), Vec(0,1,0));
  EXPECT_EQ(t.intersect(r), 1);
  ASSERT_EQ(r.hits.size(), 1);
  EXPECT_EQ(r.hits[0].t, 1);
}

TEST (Triangle, testNorm){
  Triangle t = Triangle(Point(-1,-1,4), Point(1,1,-20), Point(-4,0,4));
  Vec v = t.norm(Point(-1,1,2));
  EXPECT_TRUE(v==normalize(Vec(3,9,1)) || -1*v==normalize(Vec(3,9,1)));
  Ray r = Ray(Point(1,1,0), Vec(0,0,-2));
  t.intersect(r);
  ASSERT_EQ(r.hits.size(), 1);
  EXPECT_EQ(r.hits[0].normal, normalize(Vec(3,9,1)));
}

TEST (World, rayIntersectionOrtho){
  View v = View(2, 2, 1, 1); //width, height
  World w = World(v);
  Sphere s1 = Sphere(Point(-.5,.5,-2), 1);
  Plane p1 = Plane(0,0,1,2); //right 1 unit in front of view plane
  Plane p2 = Plane(0,0,1,-.5); w.addObj(&p2); //half unit behind eye point
  Triangle t1 = Triangle(Dot(Point(.5,.5,-1)), Dot(Point(.5,-.5,-1)), Dot(Point(-1,0,-10)));
  w.addObj(&p1);
  w.addObj(&s1);
  w.addObj(&t1);
  vector<Point> samples;
  samples = w.viewPlane.getSingleRay(0,0);
  int indInt;

  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    indInt = w.rayIntersection(r);
    EXPECT_EQ(r.hits[indInt].shape_obj,&p1);
  }
  samples = w.viewPlane.getSingleRay(1,0);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    indInt = w.rayIntersection(r);
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
  samples = w.viewPlane.getSingleRay(0,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    indInt = w.rayIntersection(r);
    EXPECT_EQ(r.hits[indInt].shape_obj,&s1);
  }
  samples = w.viewPlane.getSingleRay(1,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    indInt = w.rayIntersection(r);
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
}

TEST (World, rayIntersectionPerspective){
  Cam c = Cam();
  View v = View(2, 2, 1, 1); //width, height
  World w = World(v,c);
  Sphere s1 = Sphere(Point(-.5,.5,-2), 1);
  Plane p1 = Plane(0,0,1,2); //right 1 unit in front of view plane
  Plane p2 = Plane(0,0,1,-.5); w.addObj(&p2); //half unit behind eye point
  Triangle t1 = Triangle(Dot(Point(.5,.5,-1)), Dot(Point(.5,-.5,-1)), Dot(Point(-1,0,-10)));
  w.addObj(&p1);
  w.addObj(&s1);
  w.addObj(&t1);
  w.addObj(&p2);
  vector<Point> samples;
  samples = w.viewPlane.getSingleRay(0,0);
  int indInt;
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(-.5,-.5,-1)));
    indInt = w.rayIntersection(r);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else if (r.hits[j].shape_obj->isA<Plane>()) s = "Plane";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&p1);
  }
  samples = w.viewPlane.getSingleRay(1,0);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(.5,-.5,-1)));
    indInt = w.rayIntersection(r);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else if (r.hits[j].shape_obj->isA<Plane>()) s = "Plane";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
  samples = w.viewPlane.getSingleRay(0,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(-.5,.5,-1)));
    indInt = w.rayIntersection(r);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else if (r.hits[j].shape_obj->isA<Plane>()) s = "Plane";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&s1);
  }
  samples = w.viewPlane.getSingleRay(1,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(.5,.5,-1)));
    indInt = w.rayIntersection(r);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else if (r.hits[j].shape_obj->isA<Plane>()) s = "Plane";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
}



TEST (World, testSphereBVH){
  Cam c = Cam();
  View v = View(2, 2, 1, 1); //width, height
  World w = World(v,c);
  Sphere s1 = Sphere(Point(13,-1,0), 1);

  Sphere s2 = Sphere(Point(4,1,-6), 1);
  Sphere s3 = Sphere(Point(4,4,-6), 1);
  cerr << "s2" << s2.aabb[1];
  cerr << "s3" << s3.aabb[1];

  Sphere s4 = Sphere(Point(1,0,0), 1);
  Sphere s5 = Sphere(Point(2,0,0), 1);
  Sphere s6 = Sphere(Point(1,2,0), 1);
  Sphere s7 = Sphere(Point(2,2,0), 1);

  Sphere s8 = Sphere(Point(1,4,0), 1);
  Sphere s9 = Sphere(Point(2,4,0), 1);
  Sphere s10 = Sphere(Point(1,5,0), 1);
  Sphere s11 = Sphere(Point(2,5,0), 1);
  w.addObj(&s1);
  w.addObj(&s2);
  w.addObj(&s3);
  w.addObj(&s4);
  w.addObj(&s5);
  w.addObj(&s6);
  w.addObj(&s7);
  w.addObj(&s8);
  w.addObj(&s9);
  w.addObj(&s10);
  w.addObj(&s11);
  Node * tree = w.buildBVH(w.objects);
  EXPECT_EQ(tree->_box.first, Point(0 , -2, -7));
  EXPECT_EQ(tree->_box.second, Point(14 , 6, 1));
  EXPECT_TRUE(tree->empty);

  Node * firstRight = tree->right;
  ASSERT_NE(firstRight, (Node * )nullptr);
  EXPECT_EQ(firstRight->_box.first, Point(12 , -2, -1));
  EXPECT_EQ(firstRight->_box.second, Point(14 , 0, 1));
  EXPECT_FALSE(firstRight->empty);
  EXPECT_EQ(firstRight->s.size(), 1);
  EXPECT_EQ(firstRight->s[0] , &s1);

  Node * firstLeft = tree->left;
  EXPECT_EQ(firstLeft->_box.first, Point(0 , -1, -7));
  EXPECT_EQ(firstLeft->_box.second, Point(5 , 6, 1));
  EXPECT_TRUE(firstLeft->empty);

  Node * secondLeft = firstLeft->left;
  Node * secondRight = firstLeft->right;
  ASSERT_NE(secondLeft, (Node * )nullptr);
  EXPECT_EQ(secondLeft->_box.first, Point(3 , 0, -7));
  EXPECT_EQ(secondLeft->_box.second, Point(5 , 5, -5));
  EXPECT_FALSE(secondLeft->empty);
  EXPECT_EQ(secondLeft->s.size(), 2);
  EXPECT_TRUE(secondLeft->s[0] == &s2 || secondLeft->s[0] == &s3);
  EXPECT_TRUE(secondLeft->s[1] == &s2 || secondLeft->s[1] == &s3);

  EXPECT_EQ(secondRight->_box.first, Point(0 , -1, -1));
  EXPECT_EQ(secondRight->_box.second, Point(3 , 6, 1));
  EXPECT_TRUE(secondRight->empty);

  Node * thirdLeft = secondRight->left;
  ASSERT_NE(thirdLeft, (Node * )nullptr);
  EXPECT_EQ(thirdLeft->_box.first, Point(0 , -1, -1));
  EXPECT_EQ(thirdLeft->_box.second, Point(3 , 3, 1));
  EXPECT_FALSE(thirdLeft->empty);
  EXPECT_EQ(thirdLeft->s.size(), 4);
  EXPECT_TRUE(thirdLeft->s[0] == &s4 || thirdLeft->s[0] == &s5 || thirdLeft->s[0] == &s6 || thirdLeft->s[0] == &s7);
  EXPECT_TRUE(thirdLeft->s[1] == &s4 || thirdLeft->s[1] == &s5 || thirdLeft->s[1] == &s6 || thirdLeft->s[1] == &s7);
  EXPECT_TRUE(thirdLeft->s[2] == &s4 || thirdLeft->s[2] == &s5 || thirdLeft->s[2] == &s6 || thirdLeft->s[2] == &s7);
  EXPECT_TRUE(thirdLeft->s[3] == &s4 || thirdLeft->s[3] == &s5 || thirdLeft->s[3] == &s6 || thirdLeft->s[3] == &s7);

  Node * thirdRight = secondRight->right;
  ASSERT_NE(thirdRight, (Node * )nullptr);
  EXPECT_EQ(thirdRight->_box.first, Point(0 , 3, -1));
  EXPECT_EQ(thirdRight->_box.second, Point(3 , 6, 1));
  EXPECT_FALSE(thirdRight->empty);
  EXPECT_EQ(thirdRight->s.size(), 4);
  EXPECT_TRUE(thirdRight->s[0] == &s8 || thirdRight->s[0] == &s9 || thirdRight->s[0] == &s10 || thirdRight->s[0] == &s11);
  EXPECT_TRUE(thirdRight->s[1] == &s8 || thirdRight->s[1] == &s9 || thirdRight->s[1] == &s10 || thirdRight->s[1] == &s11);
  EXPECT_TRUE(thirdRight->s[2] == &s8 || thirdRight->s[2] == &s9 || thirdRight->s[2] == &s10 || thirdRight->s[2] == &s11);
  EXPECT_TRUE(thirdRight->s[3] == &s8 || thirdRight->s[3] == &s9 || thirdRight->s[3] == &s10 || thirdRight->s[3] == &s11);

}



TEST (World, testTriangleBVH){
  View v = View(200, 100, 2, 3); //width, height
  Cam c = Cam();
  World w = World(v,c);
  Triangle t1 = Triangle(Dot(Point(0,0,0)), Dot(Point(3,0,0)), Dot(Point(0,3,0))); //1,1
  Triangle t2 = Triangle(Dot(Point(6,0,0)), Dot(Point(9,0,0)), Dot(Point(6,3,0))); //7,1
  Triangle t5 = Triangle(Dot(Point(0,6,0)), Dot(Point(3,6,0)), Dot(Point(0,9,0))); //1,7

  Triangle t3 = Triangle(Dot(Point(18,0,0)), Dot(Point(21,0,0)), Dot(Point(18,3,0))); //19,1
  Triangle t4 = Triangle(Dot(Point(24,0,0)), Dot(Point(27,0,0)), Dot(Point(24,3,0))); //25.1

  w.addObj(&t1);
  w.addObj(&t2);
  w.addObj(&t3);
  w.addObj(&t4);
  w.addObj(&t5);
  Node * tree = w.buildBVH(w.objects);
  EXPECT_EQ(tree->_box.first, Point(0 , 0, 0));
  EXPECT_EQ(tree->_box.second, Point(27 , 9, 0));
  EXPECT_TRUE(tree->empty);

  Node * left = tree->left;
  Node * right = tree->right;
  ASSERT_NE(left, (Node * )nullptr);
  EXPECT_EQ(left->_box.first, Point(0 , 0, 0));
  EXPECT_EQ(left->_box.second, Point(9 , 9, 0));
  EXPECT_FALSE(left->empty);
  EXPECT_EQ(left->s.size(), 3);
  EXPECT_TRUE(left->s[0] == &t1 || left->s[0] == &t2 || left->s[0] == &t5);
  EXPECT_TRUE(left->s[1] == &t1 || left->s[1] == &t2 || left->s[1] == &t5);
  EXPECT_TRUE(left->s[2] == &t1 || left->s[2] == &t2 || left->s[2] == &t5);

  ASSERT_NE(right, (Node * )nullptr);
  EXPECT_EQ(right->_box.first, Point(18 , 0, 0));
  EXPECT_EQ(right->_box.second, Point(27 , 3, 0));
  EXPECT_FALSE(right->empty);
  EXPECT_EQ(right->s.size(), 2);
  EXPECT_TRUE(right->s[0] == &t3 || right->s[0] == &t4);
  EXPECT_TRUE(right->s[1] == &t3 || right->s[1] == &t4);
}


TEST (World, rayIntersectionBVH){
  Cam c = Cam();
  View v = View(2, 2, 1, 1); //width, height
  World w = World(v,c);
  Sphere s1 = Sphere(Point(-.5,.5,-2), 1);
  Sphere s2 = Sphere(Point(-.5,-.5,-2), 1);
  Triangle t1 = Triangle(Dot(Point(.5,.5,-1)), Dot(Point(.5,-.5,-1)), Dot(Point(-1,0,-10)));
  w.addObj(&s1);
  w.addObj(&s2);
  w.addObj(&t1);

  Sphere s3 = Sphere(Point(-.5,-.5,-7), 1);
  Triangle t3 = Triangle(Dot(Point(.5,.5,-2)), Dot(Point(.5,-.5,-2)), Dot(Point(-1,0,-11)));
  Sphere s4 = Sphere(Point(-.5,-.5,-8), 1);
  Triangle t4 = Triangle(Dot(Point(.5,.5,-10)), Dot(Point(.5,-.5,-10)), Dot(Point(-1,0,-20)));
  Sphere s5 = Sphere(Point(-50,-.5,-7), 1);
  Triangle t5 = Triangle(Dot(Point(1,1,-2)), Dot(Point(1,-1,-1)), Dot(Point(-1,10,-10)));
  Sphere s6 = Sphere(Point(10,10,-1), 1);
  Triangle t6 = Triangle(Dot(Point(.5,.5,-10)), Dot(Point(.5,-.5,-10)), Dot(Point(-1,0,-15)));
  Sphere s7 = Sphere(Point(-70,100,-2), 1);
  Triangle t7 = Triangle(Dot(Point(3,.5,-1)), Dot(Point(3,-.5,-1)), Dot(Point(-3,0,-10)));
  w.addObj(&s3);
  w.addObj(&s4);
  w.addObj(&s5);
  w.addObj(&s6);
  w.addObj(&s7);
  w.addObj(&t3);
  w.addObj(&t4);
  w.addObj(&t5);
  w.addObj(&t6);
  w.addObj(&t7);
  Node * tree = w.buildBVH(w.objects);
  vector<Point> samples;
  samples = w.viewPlane.getSingleRay(0,0);
  int indInt;
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(-.5,-.5,-1)));
    indInt = w.rayIntersection(r, tree, true);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&s2);
  }
  samples = w.viewPlane.getSingleRay(1,0);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(.5,-.5,-1)));
    indInt = w.rayIntersection(r, tree, true);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
  samples = w.viewPlane.getSingleRay(0,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(-.5,.5,-1)));
    indInt = w.rayIntersection(r, tree, true);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&s1);
  }
  samples = w.viewPlane.getSingleRay(1,1);
  for (int p = 0; p < samples.size(); p++){
    Ray r = w.getRay(samples[p]);
    EXPECT_EQ(r.dir,normalize(Vec(.5,.5,-1)));
    indInt = w.rayIntersection(r, tree, true);
    for (int j = 0; j < r.hits.size(); j++){
      string s;
      if (r.hits[j].shape_obj->isA<Triangle>()) s = "Triangle";
      else s = "Sphere";
      cerr << "hit " << to_string(j) << ": " << s << " at " << r.mult(r.hits[j].t);
    }
    cerr << endl;
    EXPECT_EQ(r.hits[indInt].shape_obj,&t1);
  }
}

//revisit all logic in world

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
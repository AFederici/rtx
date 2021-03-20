#include "../inc/World.h"

World::World(View &v){
    viewPlane = v;
    isOrtho = true;
    sampling_rate = 36;
    isAccel = true;
}

World::World(View &v, Cam & c){
    viewPlane = v;
    camera = c;
    isOrtho = false;
    sampling_rate = 36;
    isAccel = true;
}

void World::savePPM(ostream& ostr){
    ostr << MODE << endl << to_string(viewPlane.getW()) << " " << to_string(viewPlane.getH()) << endl << to_string((int)MAX_COLOR) << endl;
    for (int row = output.size() - 1; row >= 0; row--){
        //cerr << "\rrow: " << row << ',';
        for (int col = 0; col < output[0].size(); col++){
            ostr << output[row][col] << endl;
        }
    }
    //cerr << endl;
}

Ray World::getRay(Point look){
    if (isOrtho) return Ray(look, Vec(0,0,-1));
    Point worldView = camera.u*look.x + camera.v*look.y + camera.w*look.z;
    //worldView += camera.eye;
    Vec directionVec = toVec(camera.eye, worldView);
    directionVec.normalize();
    Ray r = Ray(camera.eye, directionVec);
    return r;
}

void World::debugPPM() { savePPM(cout); }

void World::addLight(Dot * s){ lights.push_back(s); }

void World::addObj(Shapes * s) { objects.push_back(s); }

Color blackBackground() { return Color(0,0,0); }

Color blueGradientBackground(Ray &r) { 
    Vec unit_direction = normalize(r.dir);
    double t = 0.5*(unit_direction.y + 1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}

double aabbIntersection(pair<Point,Point> box, Ray &r) {
    double tx1 = (box.first.x - r.o.x)*r.inverseNorm.x;
    double tx2 = (box.second.x - r.o.x)*r.inverseNorm.x;
    double tmin = min(tx1, tx2);
    double tmax = max(tx1, tx2);
    double ty1 = (box.first.y - r.o.y)*r.inverseNorm.y;
    double ty2 = (box.second.y - r.o.y)*r.inverseNorm.y;
    tmin = max(tmin, min(ty1, ty2));
    tmax = min(tmax, max(ty1, ty2));
    double tz1 = (box.first.z - r.o.z)*r.inverseNorm.z;
    double tz2 = (box.second.z - r.o.z)*r.inverseNorm.z;
    tmin = max(tmin, min(tz1, tz2));
    tmax = min(tmax, max(tz1, tz2));
    return (tmax >= tmin) ? tmin : -1.0;
}

int rayIntersectionAcceleratedHelperQuick(Ray & r, Node * root){
    if (!root) return 0;
    if (!aabbIntersection(root->_box, r)) return 0;
    if (!root->empty){
        int i = 0;
        while (i++ < root->s.size()){ 
            root->s[i]->intersect(r); 
            if (r.hits.size()){
                if (r.hits[0].t <= 1) return 1;
                r.hits.pop_back();
            }
        }
        return 0;
    } else {
        return rayIntersectionAcceleratedHelperQuick(r, root->left) || rayIntersectionAcceleratedHelperQuick(r, root->right);
    }
}
//V2
/*
void rayIntersectionAcceleratedHelper(Ray & r, Node * root){
    if (!root) return;
    if (close(aabbIntersection(root->_box, r),-1)) return;
    if (!root->empty){
        for (int i = 0; i < root->s.size(); i++){ root->s[i]->intersect(r); }
    } else {
        double lt = aabbIntersection(root->left->_box, r);
        double rt = aabbIntersection(root->right->_box, r);
        if (close(lt,-1) && close(rt,-1)) return;
        if (close(lt,-1)) lt = maxD;
        if (close(rt,-1)) rt = maxD;
        if (lt < rt) {
            rayIntersectionAcceleratedHelper(r, root->left);
            if (!r.hits.size()) rayIntersectionAcceleratedHelper(r, root->right);
        }
        else {
            rayIntersectionAcceleratedHelper(r, root->right);
            if (!r.hits.size()) rayIntersectionAcceleratedHelper(r, root->left);
        }
    }
}
*/
//V1
/*
void rayIntersectionAcceleratedHelper(Ray & r, Node * root){
    if (!root) return;
    if (close(aabbIntersection(root->_box, r),-1)) return;
    if (!root->empty){
        for (int i = 0; i < root->s.size(); i++){ root->s[i]->intersect(r); }
    } else {
        rayIntersectionAcceleratedHelper(r, root->left);
        rayIntersectionAcceleratedHelper(r, root->right);
    }
}
*/

void rayIntersectionAcceleratedHelper(Ray & r, Node * root){
    if (!root) return;
    double hit = aabbIntersection(root->_box, r);
    if (close(hit,-1)) return;
    if (r.hits.size() && (hit > r.lastHit)) return;
    if (!root->empty){
        for (int i = 0; i < root->s.size(); i++){ root->s[i]->intersect(r); }
        for (int i = 0; i < r.hits.size(); i++){
            if (r.hits[i].t < r.lastHit){ r.lastHit = r.hits[i].t; }
        }
    } else {
        rayIntersectionAcceleratedHelper(r, root->left);
        rayIntersectionAcceleratedHelper(r, root->right);
    }
}

int World::rayIntersection(Ray &r, Node * tree, bool accel) {
    if (accel && tree){
        r.inverseNorm = normalize(r.dir);
        r.inverseNorm.x = (r.inverseNorm.x ) ? 1.0 / r.inverseNorm.x  : 0;
        r.inverseNorm.y = (r.inverseNorm.y) ? 1.0 / r.inverseNorm.y : 0;
        r.inverseNorm.z = (r.inverseNorm.z) ? 1.0 / r.inverseNorm.z : 0;
        r.lastHit = maxD;
        rayIntersectionAcceleratedHelper(r, tree);
    }
    else { for (int i = 0; i < objects.size(); i++){ objects[i]->intersect(r); } }
    int closest_t = T_MAX; 
    int closest_ind = -1;
    for (int i = 0; i < r.hits.size(); i++){ 
        if ((r.hits[i].t < closest_t) && (r.hits[i].t >= T_MIN)) { 
            closest_t = r.hits[i].t; 
            closest_ind = i; 
        } 
    }
    return closest_ind;
}

Color World::computeShading(Ray &r, int closest_ind){
    if (closest_ind < 0) return Color(0.0,0.0,0.0);
    hit h = r.hits[closest_ind];
    Material objMat = (h.shape_obj->isA<Triangle>()) ? ((Triangle*)h.shape_obj)->getColor(h.p) : h.shape_obj->getColor();
    Color c = objMat.ambient * ambience;
    if (isOrtho) return c;
    //if (isOrtho || r.hits[closest_ind].shape_obj->isA<Triangle>()) 
    Point onSurface = r.mult(h.t);
    Vec N = h.normal;
    Vec toCam = toVec(onSurface, camera.eye);
    for (int light = 0; light < lights.size(); light++){
        Material lighting = lights[light]->material;
        Vec toLight = toVec(onSurface, lights[light]->loc);
        Vec R = (2*N*N.dot(toLight)) - toLight;
        toCam.normalize(); toLight.normalize(); R.normalize();
        c += scale(lighting.diffuse*max(0.0,toLight.dot(N))*objMat.diffuse);
        c += scale(lighting.specular*pow(max(0.0,R.dot(toCam)),objMat.shiny)*objMat.diffuse);
    }
    c.x = min(1.0,c.x);
    c.z = min(1.0,c.y);
    c.y = min(1.0,c.z);
    return c;
    
}

int World::rayShadow(Ray &r, int closestInd, Node * tree){
    if (closestInd < 0 || isOrtho || !lights.size()) { return 0; }
    else{
        for (int light = 0; light < lights.size(); light++){
            Point onSurface = r.mult(r.hits[closestInd].t);
            Vec toLight = toVec(onSurface, lights[light]->loc);
            Ray shadowRay = Ray(onSurface + 0.01*normalize(toLight), toLight);
            if (!tree){
                int i = 0;
                while (i < objects.size()) {  
                    if (objects[i++]->intersect(shadowRay)){
                        if ((shadowRay.hits[0].t <= 1) && (shadowRay.hits[0].shape_obj != r.hits[closestInd].shape_obj)) return 1;
                    }
                }
                return 0;
            } else {
                shadowRay.inverseNorm = normalize(r.dir);
                shadowRay.inverseNorm.x = (shadowRay.inverseNorm.x ) ? 1.0 / shadowRay.inverseNorm.x  : 0;
                shadowRay.inverseNorm.y = (shadowRay.inverseNorm.y) ? 1.0 / shadowRay.inverseNorm.y : 0;
                shadowRay.inverseNorm.z = (shadowRay.inverseNorm.z) ? 1.0 / shadowRay.inverseNorm.z : 0;
                int shadowInd = rayIntersection(shadowRay, tree, true);
                return ((shadowInd == -1) || shadowRay.hits[shadowInd].t > 1) ? 0 : 1;
            }
        }   
    }
    return 0;
}

uint64_t encode(uint16_t x, uint16_t y, uint16_t z){
    uint64_t ret = 0;
    for (int topBit = 15; topBit >= 0; topBit--){
        ret |= (((z >> topBit) & 1U) << ((3*topBit)+2));
        ret |= (((y >> topBit) & 1U) << ((3*topBit)+1));
        ret |= (((z >> topBit) & 1U) << ((3*topBit)  ));
    }
    return ret;
    
}

/*

void World::buildRegularGrid(){
    int o = objects.size();
    pair<Point,Point> bb = make_pair(Point(maxD, maxD, maxD),Point(minD, minD, minD));
    for (int obj = 0; obj < o; obj++){
        pairPointSwap(bb, objects[obj]->aabb, 0);
    }
    Vec w = toVec(bb.first, bb.second);
    double s = pow((w.x*w.y*w.z) / (1.0*o), 1.0/3.0);
    Triplet n = Triplet(floor(2*w.x/s) + 1, floor(2*w.y/s) + 1, floor(2*w.z/s) + 1);
}

struct less_than_key
{
    int axis;
    less_than_key(int ax) : axis(ax) {}
    inline bool operator() (Shapes*& s1, Shapes*& s2)
    {
        pair<Point,Point> p1 = s1->getAABB();
        pair<Point,Point> p2 = s2->getAABB();
        return (p1.first[axis] < struct2.key);
    }
};
*/

void pairPointSwap(pair<Point,Point> &p1, vector<Point> &p2, int ind){
        p1.first.x = min(p1.first.x, p2[ind].x);
        p1.first.y = min(p1.first.y, p2[ind].y);
        p1.first.z = min(p1.first.z, p2[ind].z);
        p1.second.x = max(p1.second.x, p2[ind+1].x);
        p1.second.y = max(p1.second.y, p2[ind+1].y);
        p1.second.z = max(p1.second.z, p2[ind+1].z);
}

vector< vector<Shapes*> > World::extractVec(vector<Shapes*> v, int axis, Point partition){
    vector< vector<Shapes*> > ret;
    vector<Shapes*> less; 
    vector<Shapes*> more;
    for (int i = 0; i < v.size(); i++){
        if (v[i]->aabb[2][axis] < partition[axis]) less.push_back(v[i]);
        else more.push_back(v[i]);
    } 
    ret.push_back(less);
    ret.push_back(more);
    return ret;
}


Node * World::buildBVH(vector<Shapes*> v){
    int o = v.size();
    //cerr << "---------\n" << to_string(o) << " at node\n";
    if (!o) return NULL;
    pair<Point,Point> bb = make_pair(Point(maxD, maxD, maxD),Point(minD, minD, minD));
    pair<Point,Point> centroids = make_pair(Point(maxD, maxD, maxD),Point(minD, minD, minD));
    for (int obj = 0; obj < o; obj++){
        pairPointSwap(bb, v[obj]->aabb, 0);
        pairPointSwap(centroids, v[obj]->aabb, 2);
    }
    //cerr << "bbSmall: " << bb.first; 
    //cerr << "bbBig: " << bb.second; 
    //cerr << "cSmall: " << centroids.first; 
    //cerr << "cBig: " << centroids.second; 
    Node * root = new Node(bb);
    Point partition = (centroids.second + centroids.first) / 2.0;
    //cerr << "partition " << partition;
    int axis = 2;
    if (partition.x > max(partition.y, partition.z)) axis = 0;
    if (partition.y > max(partition.x, partition.z)) axis = 1;
    vector< vector<Shapes*> > bvhSplit = extractVec(v, axis, partition);
    //cerr << "split to left: " << to_string(bvhSplit[0].size()) << endl;
    if ((o < 8) || !bvhSplit[0].size() || !bvhSplit[1].size()){
        for (int el = 0; el < o; el++){ root->s.push_back(v[el]); } 
        root->empty = false; 
        //cerr << "built: " << to_string(o) << endl;
        return root; 
    }
    root->left = buildBVH(bvhSplit[0]);
    root->right = buildBVH(bvhSplit[1]);
    return root;
}

void World::render(){
    int hits = 0;
    Node * tree = (isAccel) ? buildBVH(objects) : NULL;
    for (int row = 0; row < viewPlane.v_res; row++) {
        //std::cerr << "\rScanlines remaining: " << viewPlane.h_res - row << endl;
        vector<Color> nextRow;
        for (int col = 0; col < viewPlane.h_res; col++) {
            Color nextColor = Color(0.0,0.0,0.0,0.0);
            vector<Point> samples = (sampling_rate) ? viewPlane.getMultiJittered(col,row, sampling_rate) : viewPlane.getSingleRay(col,row);
            for (int p = 0; p < samples.size(); p++){
                Ray r = getRay(samples[p]);
                int indInt = (isAccel) ? rayIntersection(r, tree, true) : rayIntersection(r);
                hits += ((indInt >= 0 ) ? 1 : 0);
                Color shading = computeShading(r,indInt);
                shading = rayShadow(r, indInt, tree) ? (shading + shadow) / 2.0 : shading;
                nextColor.addSample(shading);
            }
            nextRow.push_back(nextColor);
        }
        output.push_back(nextRow);
    }
    cerr << "render hits " << to_string(hits) << endl;
}
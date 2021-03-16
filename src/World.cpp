#include "../inc/World.h"

World::World(View &v){
    viewPlane = v;
    isOrtho = true;
    sampling = true;
}

World::World(View &v, Cam & c){
    viewPlane = v;
    camera = c;
    isOrtho = false;
    sampling = true;
}

void World::setNoSampling() { sampling = false; }

void World::savePPM(ostream& ostr){
    ostr << MODE << endl << to_string(viewPlane.getW()) << " " << to_string(viewPlane.getH()) << endl << to_string(MAX_COLOR) << endl;
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
    worldView += camera.eye;
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

int World::rayIntersection(Ray &r) {
    for (int i = 0; i < objects.size(); i++){ objects[i]->intersect(r); }
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
    Color c = h.shape_obj->material.ambient;
    if (isOrtho) return c;
    //if (isOrtho || r.hits[closest_ind].shape_obj->isA<Triangle>()) 
    Point onSurface = r.mult(h.t);
    Vec N = h.shape_obj->norm(onSurface);
    Vec toCam = toVec(onSurface, camera.eye);
    for (int light = 0; light < lights.size(); light++){
        Material lighting = lights[light]->material;
        Vec toLight = toVec(lights[light]->loc, onSurface);
        Vec R = (2*N*N.dot(toLight)) - toLight;
        c += (lighting.diffuse*toLight.dot(N)*h.shape_obj->material.diffuse);
        c += (lighting.specular*pow(R.dot(toCam),h.shape_obj->material.shiny)*h.shape_obj->material.diffuse);
    }
    return c;
}

int World::rayShadow(Ray &r, int closestInd){
    if (closestInd < 0 || isOrtho) return 0;
    for (int light = 0; light < lights.size(); light++){
        Point onSurface = r.mult(r.hits[closestInd].t);
        Vec toLight = toVec(onSurface, lights[light]->loc);
        Ray shadowRay = Ray(onSurface + 0.01*normalize(toLight), toLight);
        int i = 0;
        while (i < objects.size()) {  
            if (objects[i++]->intersect(shadowRay)){
                if ((shadowRay.hits[0].t <= 1) && (shadowRay.hits[0].shape_obj != r.hits[closestInd].shape_obj)) return 1;
            }
        }
    }
    return 0;
}

void World::render(){
    for (int row = 0; row < viewPlane.v_res; row++) {
        std::cerr << "\rScanlines remaining: " << viewPlane.h_res - row << endl;
        vector<Color> nextRow;
        for (int col = 0; col < viewPlane.h_res; col++) {
            Color nextColor = Color(0.0,0.0,0.0,0.0);
            vector<Point> samples = (sampling) ? viewPlane.getMultiJittered(col,row) : viewPlane.getSingleRay(col,row);
            for (int p = 0; p < samples.size(); p++){
                Ray r = getRay(samples[p]);
                int indInt = rayIntersection(r);
                Color shading = computeShading(r,indInt);
                shading = rayShadow(r, indInt) ? (shading + shadow) / 2 : shading;
                nextColor.addSample(shading);
            }
            nextRow.push_back(nextColor);
        }
        output.push_back(nextRow);
    }
}
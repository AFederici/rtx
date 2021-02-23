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
        cerr << "\rrow: " << row << ',';
        for (int col = 0; col < output[0].size(); col++){
            ostr << output[row][col] << endl;
        }
    }
    cerr << endl;
}

Ray World::getRay(Point look){
    if (isOrtho) return Ray(look, Vec(0,0,-1));
    Point worldView = camera.u*look.x + camera.v*look.y + camera.w*look.z;
    Vec directionVec = toVec(worldView, camera.eye);
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

int World::rayColor(Color & c, Ray &r) {
    for (int i = 0; i < objects.size(); i++){ objects[i]->intersect(r); }
    int closest_t = T_MAX;
    int closest_ind = -1;
    cerr << "Ray Intersections:" << to_string(r.hits.size()) << endl;
    for (int i = 0; i < r.hits.size(); i++){ 
        if (r.hits[i].t < closest_t) { 
            closest_t = r.hits[i].t; 
            closest_ind = i; 
        } 
    }
    if (closest_ind >= 0){
        for (int light = 0; light < lights.size(); light++){
            Point onSurface = r.mult(r.hits[closest_ind].t);
            Vec toLight = toVec(lights[light]->loc, onSurface);
            toLight.normalize();
            double diffuse = r.hits[closest_ind].normal.dot(toLight);
            diffuse = (diffuse < 0 ) ? 0 : diffuse;
            Color shading = r.hits[closest_ind].shape_obj->get_shading(r, closest_ind);
            if (r.hits[closest_ind].shape_obj->isA<Triangle>()) c.addSample(shading);
            else c.addSample(shading * diffuse * lights[light]->get_shading(r, closest_ind));
        }
    }
    else c.addSample(blackBackground());
    return closest_ind;
}


void World::rayShadow(Color & c, Ray &r, int closestInd){
    for (int light = 0; light < lights.size(); light++){
        Point onSurface = r.mult(r.hits[closestInd].t);
        Vec toLight = toVec(lights[light]->loc, onSurface);
        Ray shadowRay = Ray(onSurface + 0.001*toLight, toLight);
        for (int i = 0; i < objects.size(); i++){ objects[i]->intersect(shadowRay); }
        for (int i = 0; i < shadowRay.hits.size(); i++){ 
            if (shadowRay.hits[i].t < 1) c.addSample(shadow);
        }
    }
}

void World::render(){
    for (int j = 0; j < viewPlane.v_res; j++) {
        std::cerr << "\rScanlines remaining: " << viewPlane.h_res - j << endl;
        vector<Color> nextRow;
        for (int i = 0; i < viewPlane.h_res; i++) {
            int ind = nextRow.size() - 1; 
            Color nextColor = Color(0.0,0.0,0.0,0.0);
            vector<Point> samples = (sampling) ? viewPlane.getMultiJittered(i,j) : viewPlane.getSingleRay(i,j);
            cerr << "Pixel (r,c): (" << to_string(j) << "," << to_string(i) << ")" << endl; 
            for (int p = 0; p < samples.size(); p++){
                Ray r = getRay(samples[p]);
                //Ray r = Ray(Point(0,0,0), samples[p]);
                cerr << "Sample " << to_string(p) << r.dir << endl;
                int indInt = rayColor(nextColor, r);
                //std::cerr << " " << std::to_string(indInt);
                if (indInt >= 0) rayShadow(nextColor, r, indInt);
            }
            nextRow.push_back(nextColor);
            //std::cerr << std::endl;
        }
        output.push_back(nextRow);
    }
}
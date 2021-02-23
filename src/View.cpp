#include "../inc/View.h"

View::View(int width, double sideLen, double dist){
    h_res = width;
    v_res = h_res / aspect_ratio;
    pixel_size = sideLen;
    d = dist;
}
View::View(){
    h_res = 100;
    v_res = h_res / aspect_ratio;
    pixel_size = 1.0;
    d = 1;
}

int View::getW() { return h_res; }
    
int View::getH() { return v_res; }

vector<Point> View::getSingleRay(int row, int col){ 
    vector<Point> ret;
    ret.push_back(getLookAt(row, col, 0.5, 0.5));
    return ret;
}

vector<Point> View::getMultiJittered(int row, int col){
    vector<Point> ret;
    vector < vector<int> > choices;
    int gran = static_cast<int>(sqrt(sampling_rate));
    for (int i = 0; i < (2*gran); i++) {
        vector<int> offsets;
        for (int j = 0; j < gran; j++){ offsets.push_back(j);}
        choices.push_back(offsets);
    }
    //cerr << "pixel: " << to_string(row) << "," << to_string(col) << ":" << endl;
    for (int coarse_row = 0; coarse_row < gran; coarse_row++){
        for (int coarse_col = 0; coarse_col < gran; coarse_col++){
            int u = static_cast<int>(random_double(0,choices[coarse_row].size()));
            int v = static_cast<int>(random_double(0,choices[coarse_col+gran].size()));
            double y_off = ((coarse_row*gran) / (1.0*sampling_rate)) + ((1.0*choices[coarse_row][u]) / (1.0*sampling_rate));
            double x_off = ((coarse_col*gran) / (1.0*sampling_rate)) + ((1.0*choices[coarse_col+gran][v]) / (1.0*sampling_rate));
            //cerr << "\t" << to_string(x_off)  << "_" << to_string(y_off) << endl;
            ret.push_back(getLookAt(row, col, x_off, y_off));
            choices[coarse_row].erase(choices[coarse_row].begin() + u);
            choices[coarse_col+gran].erase(choices[coarse_col+gran].begin() + v);
        }
    }
    return ret;
}

Point View::getLookAt(int row, int col, double x_off = 0.5, double y_off = 0.5) { return Point(pixel_size * (row - (h_res/2.0) + x_off), pixel_size * (col - (v_res/2.0) + y_off), -d); }
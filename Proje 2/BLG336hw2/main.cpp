//
//  main.cpp
//  BLG336hw2
//
//  Created by Tuğba Özkal on 4.04.2018.
//  Copyright © 2018 Tuğba Özkal. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <string.h>

using namespace std;

class Point{
private:
    int x;
    int y;
    int z;
public:

    Point(int = 0, int = 0, int = 0);       // default values of all x, y, and z are zero
    ~Point();   // deconstructor
    float getDistance(Point);   // distance between two points are calculated by using euclidean-distance
    void Print() const;
    
    int getX(){
        return x;
    }
    
    int getY(){
        return y;
    }
    
    int getZ(){
        return z;
    }
    
    bool operator<(const Point& other) const{   // < operator is used for comparison by x default
        return x < other.x;
    }
    
};

class ClosestPair{
private:
    Point p1;
    Point p2;
    float minDistance;
public:
    ClosestPair();
    ~ClosestPair();
    ClosestPair * findClosestPair(vector<Point>, vector<Point>);
    ClosestPair * bruteForce(vector<Point>);
    vector<Point> readFromFile(char*);
    void Print();
};

bool compareY(Point p1, Point p2){
    return (p1.getY() < p2.getY());
}

vector<Point> ClosestPair::readFromFile(char * filename){
    ifstream file(filename);
    if (!file.is_open()){
        return vector<Point>(); // return NULL;
    }
    
    vector<Point> p;
    int i = 0;
    string lineNumber;
    string x, y, z;
    
    getline(file, lineNumber); // skip the first line
    int size = atoi(lineNumber.c_str());

    while (file.good() && i++ < size){
        getline(file, x, ' ');
        getline(file, y, ' ');
        getline(file, z, '\n');
        
        Point point(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()));
        p.push_back(point);
//        p[i-1].Print();
    }
    return p;
}

Point::Point(int new_x, int new_y, int new_z){
    this->x = new_x;
    this->y = new_y;
    this->z = new_z;
}

Point::~Point(){
    
}

void Point::Print() const{
    cout << "x:" << this->x << "\ty:" << this->y << "\tz:" << this->z << endl;
}

ClosestPair::ClosestPair(){
    this->p1 = NULL;
    this->p2 = NULL;
    this->minDistance = 0;
}

ClosestPair::~ClosestPair(){
    
}

float Point::getDistance(Point p){
    return sqrt(pow((this->x - p.x), 2) + pow((this->y - p.y), 2) + pow((this->z - p.z), 2));
}

ClosestPair * ClosestPair::bruteForce(vector<Point> p){
    unsigned long size = p.size();
    float distance = 0;
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            distance = p[i].getDistance(p[j]);
            if (minDistance == 0 || distance < minDistance){
                this->p1 = p[i];
                this->p2 = p[j];
                this->minDistance = distance;
            }
        }
    }
    return this;
}

ClosestPair * ClosestPair::findClosestPair(vector<Point> px, vector<Point> py){
    unsigned long size = px.size();
    
    if (size <= 3){
        return bruteForce(px);
    }
    
    int mid = ceil(size/2.0);
    int p_mid = px[mid - 1].getX();
    
    vector<Point> xl;
    vector<Point> xr;
    vector<Point> yl;
    vector<Point> yr;
    
    for (int i = 0; i < mid; i++){
        xl.push_back(px[i]);
    }
    for (int i = mid; i < size; i++){
        xr.push_back(px[i]);
    }

    for (vector<Point>::iterator y = py.begin(); y != py.end(); y++){
        if(y->getX() <= p_mid){
            yl.push_back(*y);
        }
        else{
            yr.push_back(*y);
        }
    }
    
    ClosestPair * dl = findClosestPair(xl, yl);
    ClosestPair * dr = findClosestPair(xr, yr);
    if(dl->minDistance < dr->minDistance){
        this->minDistance = dl->minDistance;
        this->p1 = dl->p1;
        this->p2 = dl->p2;
    }
    else{
        this->minDistance = dr->minDistance;
        this->p1 = dr->p1;
        this->p2 = dr->p2;
    }
    
    int totalPointNumberInY = 0;
    vector<Point> ys;
    for (vector<Point>::iterator y = py.begin(); y != py.end(); y++){
        if (abs(mid - y->getX()) < p_mid){
            ys.push_back(*y);
            totalPointNumberInY++;
        }
    }
    
    ClosestPair * cp = this;
    
    int k;
    for (int i = 0; i < totalPointNumberInY; i++){
        k = i + 1;
        while (k < totalPointNumberInY && (ys[k].getY() - ys[i].getY()) < this->minDistance){
            if (ys[k].getDistance(ys[i]) < cp->minDistance){
                cp->p1 = ys[k];
                cp->p2 = ys[i];
                cp->minDistance = ys[k].getDistance(ys[i]);
            }
            k++;
        }
    }
    return cp;
}

void ClosestPair::Print(){
    cout << "Closest points are given below." << endl;
    cout << "1st point -> ";
    this->p1.Print();
    cout << "2nd point -> ";
    this->p2.Print();
    cout << "distance = " <<  this->minDistance << endl;
}


int main(int argc, const char * argv[]) {
    clock_t begin;
    clock_t end;
    
    ClosestPair cp;
    
//    size_t length = strlen(argv[1]);
    size_t length = 8;
    char filename[length + 1];
//    strcpy(filename, argv[1]);
    strcpy(filename, "data5000.txt");
    
    
    vector<Point> px = cp.readFromFile(filename);
    vector<Point> py = px;
    
    sort(px.begin(), px.end()); // x is used default for comparison by operator overloading
    sort(py.begin(), py.end(), compareY);
    
    begin = clock();
//    cp.bruteForce(px);
    cp.findClosestPair(px, py);
    end = clock();
    
    cp.Print();
    
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << endl << "Time elapsed: " << elapsed_secs << " seconds." << endl << endl;
    return 0;
}






//
//  main.cpp
//  BLG336hw3
//
//  Created by Tuğba Özkal on 28.04.2018.
//  Copyright © 2018 Tuğba Özkal. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

class Subset{
    int parent;
    int rank;
    
    friend class Graph;
};

class Road{
    int source;
    int destination;
    
    friend class Graph;
};

class Graph{
    vector<Road> road;
    int numberOfShops;
    int numberOfRoads;
    
    int findParent(Subset *, int);
    void Union(Subset *, int, int);
public:
    Graph();
    ~Graph();
    bool ReadFromFile(char *);
    int KargerMinCut();
};

Graph::Graph(){
    numberOfShops = 0;
    numberOfRoads = 0;
}

Graph::~Graph(){

}

bool Graph::ReadFromFile(char * filename){
    ifstream file(filename);
    if (!file.is_open()){
        return false;
    }
    
    int i = 0, id1, id2;
    string shopNumber, roadNumber;
    string shopID1, shopID2;
    
    getline(file, shopNumber, ' ');
    getline(file, roadNumber, '\n');
    numberOfShops = atoi(shopNumber.c_str());
    numberOfRoads = atoi(roadNumber.c_str());
    
    if(numberOfShops < 2 || numberOfShops > 1000){
        cout << "ERR: Number of shops should be between 2 and 1000!" << endl;
        return false;
    }
    
    if(numberOfRoads < 1 || numberOfRoads > 1000){
        cout << "ERR: Number of roads should be between 1 and 1000!" << endl;
        return false;
    }
    while (file.good() && i++ < numberOfRoads){
        getline(file, shopID1, ' ');
        getline(file, shopID2, '\n');
        id1 = atoi(shopID1.c_str());
        id2 = atoi(shopID2.c_str());
        
        if (id1 >= 1 && id1 <= 1000 && id2 >= 1 && id2 <= 1000){
            Road r;
            r.source = id1;
            r.destination = id2;
            road.push_back(r);
        }
        else{
            cout << "ERR: Shop IDs should be between 1 and 1000!" << endl;
            return false;
        }
    }
    return true;
}


int Graph::KargerMinCut(){
    Subset *subsets = new Subset[numberOfShops];
    for (int v = 0; v < numberOfShops; ++v){
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    
    srand(time(NULL));
    int i;
    for (int shops = numberOfShops; shops > 2; ){
        i = rand() % numberOfRoads;
        
        int subset1 = findParent(subsets, road[i].source);
        int subset2 = findParent(subsets, road[i].destination);
        
        if(subset1 != subset2){
            shops--;
            Union(subsets, subset1, subset2);
        }
        
    }
    int cutedges = 0;
    for (int i = 0; i < numberOfRoads; i++){
        int subset1 = findParent(subsets, road[i].source);
        int subset2 = findParent(subsets, road[i].destination);
        if (subset1 != subset2)
            cutedges++;
    }
    
    return cutedges;
}

int Graph::findParent(Subset * subsets, int i){
    if (subsets[i].parent != i){
        subsets[i].parent = findParent(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

void Graph::Union(Subset * subsets, int x, int y){
    int xroot = findParent(subsets, x);
    int yroot = findParent(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else{
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int main(int argc, const char * argv[]) {
    size_t length = strlen(argv[1]);
    char filename[length + 1];
    strcpy(filename, argv[1]);
    
    Graph ThomasShops;
    if(ThomasShops.ReadFromFile(filename)){
        cout << ThomasShops.KargerMinCut() <<  endl;
    }
    return 0;
}

















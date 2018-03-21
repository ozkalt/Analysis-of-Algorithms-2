//
//  main.cpp
//  BLG336hw1
//
//  Created by Tuğba Özkal on 16.03.2018.
//  Copyright © 2018 Tuğba Özkal. All rights reserved.
//

#include <iostream>
#include <ctype.h>
#include <vector>
#include <map>
#include <stack>
#include <string.h>
#include <time.h>

#define NODE_COUNT 8

using namespace std;

vector <bool> discovered;
vector <map <int, string> > graph(NODE_COUNT);
vector <string> node;

stack <int> S;

int move_count = 0;
int visited_node_count = 0;

void setup(){
    string * data = new string[8];
    data[0] = "(Farmer, Carrot, Rabbit, Fox || ...)";
    data[1] = "(Carrot, Fox || Farmer, Rabbit)";
    data[2] = "(Farmer, Carrot, Fox || Rabbit)";
    data[3] = "(Fox || Farmer, Carrot, Rabbit)";
    data[4] = "(Farmer, Rabbit, Fox || Carrot)";
    data[5] = "(Rabbit || Farmer, Fox, Carrot)";
    data[6] = "(Farmer, Rabbit || Fox, Carrot)";
    data[7] = "(... || Farmer, Rabbit, Fox, Carrot)";
    
    for (int i = 0; i < NODE_COUNT; i++){
        node.push_back(data[i]);
        discovered.push_back(false);
        //cout << i << ". " << node[i] << endl;
    }
    cout << endl << endl;
     
    delete []data;
    
    graph[0].insert(make_pair(1, "(Farmer, Rabbit >)"));
    graph[0].insert(make_pair(2, "(Rabbit >)"));
    graph[0].insert(make_pair(4, "(Carrot >)"));
    graph[0].insert(make_pair(6, "(Fox, Carrot >)"));
    
    graph[1].insert(make_pair(0, "(Farmer, Rabbit <)"));
    graph[1].insert(make_pair(2, "(Farmer <)"));
    graph[1].insert(make_pair(3, "(Carrot >)"));
    graph[1].insert(make_pair(7, "(Fox, Carrot >)"));

    graph[2].insert(make_pair(0, "(Rabbit <)"));
    graph[2].insert(make_pair(1, "(Farmer <)"));
    graph[2].insert(make_pair(3, "(Farmer, Carrot >)"));
    
    graph[3].insert(make_pair(1, "(Carrot <)"));
    graph[3].insert(make_pair(2, "(Farmer, Carrot <)"));
    graph[3].insert(make_pair(4, "(Farmer, Rabbit <)"));
    graph[3].insert(make_pair(7, "(Fox >)"));

    graph[4].insert(make_pair(0, "(Carrot <)"));
    graph[4].insert(make_pair(3, "(Farmer, Rabbit >)"));
    graph[4].insert(make_pair(5, "(Farmer, Fox >)"));
    graph[4].insert(make_pair(6, "(Carrot >)"));

    graph[5].insert(make_pair(4, "(Farmer, Fox <)"));
    graph[5].insert(make_pair(6, "(Farmer <)"));
    graph[5].insert(make_pair(7, "(Rabbit >)"));

    graph[6].insert(make_pair(0, "(Fox, Carrot <)"));
    graph[6].insert(make_pair(4, "(Fox <)"));
    graph[6].insert(make_pair(5, "(Farmer <)"));
    graph[6].insert(make_pair(7, "(Farmer, Rabbit >)"));

    graph[7].insert(make_pair(1, "(Fox, Carrot <)"));
    graph[7].insert(make_pair(3, "(Fox <)"));
    graph[7].insert(make_pair(5, "(Rabbit <)"));
    graph[7].insert(make_pair(6, "(Farmer, Rabbit <)"));
    
    /*
    for(int i = 0; i < NODE_COUNT; i++){
        cout << i << ".";
        for (vector<int>::iterator it = graph[i].begin() ; it != graph[i].end(); ++it){
            cout << ' ' << *it;
        }
        cout << endl;
    }
    cout << endl << endl;
     */
}

void DFS(int i){
    visited_node_count++;
    if(discovered[i]){
        return;
    }
    else{
        discovered[i] = true;
        if(S.empty() == false){
            move_count++;
            int j = S.top();
            S.pop();
            cout << graph[j][i] << endl;
        }
        cout << node[i] << endl;
        S.push(i);
        for (map<int, string>::iterator it = graph[i].begin() ; it != graph[i].end(); it++){
            DFS(it->first);
        }
    }
    return;
}

void BFS(int i){
    visited_node_count++;
    cout << node[i] << endl;
    discovered[i] = true;
    for (map<int, string>::iterator it = graph[i].begin() ; it != graph[i].end(); it++){
        if(discovered[it->first] == false){
            cout << graph[i][it->first] << endl;
            move_count++;
            BFS(it->first);
        }
    }
    return;
}


int main(int argc, const char * argv[]) {
    clock_t begin;
    clock_t end;
    setup();
    begin = clock();
    if(strcmp(argv[1], "dfs") == 0 || strcmp(argv[1], "DFS") == 0){
        cout << "ALGORITHM: DFS" << endl;
        DFS(0);
    }
    else if(strcmp(argv[1], "bfs") == 0 || strcmp(argv[1], "BFS") == 0) {
        cout << "ALGORITHM: BFS" << endl;
        BFS(0);
    }
    else{
        cout << "Parameter error!" << endl;
    }
    end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << endl << "Number of the visited nodes: " << visited_node_count << endl;
    cout << "Solution move count: " << move_count << endl;
    cout << "Running time: " << elapsed_secs << " seconds." << endl << endl;
    
    return 0;
}

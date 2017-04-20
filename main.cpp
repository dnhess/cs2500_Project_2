//#include "node.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <iostream>
#include <utility>
using namespace std;

//Declaring Functions
void vecerase(vector< list< pair<int, int> > > &adjL, int i);

int main() {
//    vector<node> nodes;
//    srand(time(NULL));
//    int id, loc, countv = 0, counte = 0;
//    bool firstfind = true;
//    string line;
//    ifstream fin;
//
//    fin.open("../data.gml");
//
//    if(fin.fail())
//    {
//        cout <<"IT HAS FAILED TO OPEN!"<<endl;
//    }
//    if(fin.good())
//    {
//        while(true) {
//            fin >>line;
//            //   cout <<"Line: "<<line<<endl;
//            if (line == "node") {
//                fin >> line; //Grabs the [ after node
//                fin >>line; //Should grab id
//                if(line == "id")
//                {
//                    fin >> line; //Grabs the value of id
//                    countv++;
//                }
//                id = atoi(line.c_str());
//                cout <<id<<endl;
//                nodes.push_back(node(id,true,rand() % 20 + 1));
//                nodes[id].id = id;
//                nodes[id].active = true;
//            }
//            if (line == "source") {
//                if(firstfind)
//                {
//                    firstfind = false;
//                } else counte++;
//                fin >> line;
//                id = atoi(line.c_str());
//            }
//            if (line == "target") {
//                fin >>line;
//                loc = atoi(line.c_str());
//            }
//            if(fin.eof()) break;
//        }
//    }
//
//    fin.close();
//
//    cout<<"Number of verticies: "<<countv<<endl;
//    cout<<"Number of edges: "<<counte<<endl;
//    cout <<"Size of nodes: "<<nodes.size()<<endl;
//    for(int i = 0; i < nodes.size(); i++)
//    {
//        cout <<"ID:"<<nodes[i].id<<endl;
//        cout <<"ACTIVE: "<<nodes[i].active<<endl;
//        cout <<"WEIGHT: "<<nodes[i].weight<<endl;
//    }
//    return 0;

    //=======TRY NUMBER 2=======

    int vertices, edges, v1, v2, weight;
    srand(time(NULL));
    int id, loc, countv = 0, counte = 0;
    bool firstfind = true;


    string line;
    ifstream fin;
    fin.open("../data.gml");

    if(fin.fail())
    {
        cout <<"IT HAS FAILED TO OPEN!"<<endl;
    }
    if(fin.good())
    {
        while(true) {
            fin >>line;
            //   cout <<"Line: "<<line<<endl;
            if (line == "node") {
                fin >> line; //Grabs the [ after node
                fin >>line; //Should grab id
                if(line == "id")
                {
                    fin >> line; //Grabs the value of id
                    countv++;
                }
            }
            if (line == "source") {
                if(firstfind)
                {
                    firstfind = false;
                } else counte++;
                fin >> line;
                id = atoi(line.c_str());
            }
            if (line == "target") {
                fin >>line;
                loc = atoi(line.c_str());
            }
            if(fin.eof()) break;
        }
    }
    fin.close();


    vertices = countv;
    edges = counte;

    vector< list< pair<int, int> > > adjacencyList(vertices + 1);

    fin.open("../data.gml");
    counte = 0;

    if(fin.fail())
    {
        cout <<"IT HAS FAILED TO OPEN!"<<endl;
    }
    if(fin.good())
    {
        while(true) {
            fin >>line;
            //   cout <<"Line: "<<line<<endl;
            if (line == "node") {
                fin >> line; //Grabs the [ after node
                fin >>line; //Should grab id
                if(line == "id")
                {
                    fin >> line; //Grabs the value of id
                }
            }
            if (line == "source") {
                if(firstfind)
                {
                    firstfind = false;
                }
                fin >> line;
                id = atoi(line.c_str());
            }
            if (line == "target") {
                fin >>line;
                loc = atoi(line.c_str());
                adjacencyList[id].push_back(make_pair(loc, (rand() % 20) + 1));
                counte++;
            }
            if(fin.eof()) break;
        }
    }
    fin.close();



    //Works for removing an element!
//    list<pair<int,int>> li = adjacencyList[10];
//    adjacencyList[10].clear();
//    for(auto it=li.rbegin();it!=li.rend();++it) {
//        cout << (*it).first<< '\n';
//        if(it->first != 272) {
//            adjacencyList[10].push_back(make_pair(it->first,it->second));
//        }
//    }


    for (int i = 0; i < adjacencyList.size(); ++i) {
        printf("adjacencyList[%d] ", i);

        list< pair<int, int> >::iterator itr = adjacencyList[i].begin();

        while (itr != adjacencyList[i].end()) {
            printf(" -> %d(%d)", itr->first, itr->second);
            ++itr;
        }

        printf("\n");
    }
}


//Functions
void vecerase(vector< list< pair<int, int> > >& adjList, int i)
{
    list<pair<int,int>> li = adjList[i];
    adjList[10].clear();
    for(auto it=li.rbegin();it!=li.rend();++it) {
        cout << (*it).first<< '\n';
        if(it->first != 272) {
            adjList[i].push_back(make_pair(it->first,it->second));
        }
    }


}

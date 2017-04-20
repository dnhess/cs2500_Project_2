//#include "node.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <queue>
using namespace std;

//Declaring Functions
void vecerase(vector< list< pair<int, int> > > &adjL, int i);
bool bfs(vector< list< pair<int, int> > > &adjL, int s, int d, int temp);

int main() {
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


//Used to remove an edge (Attack it)
void vecerase(vector< list< pair<int, int> > >& adjList, int i)
{
    list<pair<int,int>> li = adjList[i];
    adjList[i].clear();
    for(auto it=li.rbegin();it!=li.rend();++it) {
        cout << (*it).first<< '\n';
        if(it->first != 272) {
            adjList[i].push_back(make_pair(it->first,it->second));
        }
    }


}


//Checks to see if the selected values can actually be reached
bool bfs(vector<list<pair<int, int> > > &adjL, int s, int d, int V) {


    bool *visited = new bool[V];

    for(int i = 0; i < V; i++)
        visited[i] = false;

    list<int> queue;

    visited[s] = true;
    queue.push_back(s);

    list< pair<int, int> >::iterator itr = adjL[s].begin();

    while(!queue.empty())
    {
        s = queue.front();
        queue.pop_front();


        for(itr = adjL[s].begin(); itr != adjL[s].end(); itr++)
        {
            if(!visited[itr->first])
            {
                visited[itr->first] = true;
                if(itr->first == d)
                    return true;
                queue.push_back((*itr).first);
            }
        }
    }
    return false;
}

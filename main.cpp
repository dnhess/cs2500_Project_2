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


int depth;
ofstream fot;
ofstream fdp;
ofstream ipk;

//Declaring Functions
void vecerase(vector< list< pair<int, int> > > &adjL, int i, int s);
bool bfs(vector< list< pair<int, int> > > &adjL, int s, int d, int V);
int fordFulkerson(vector< list< pair<int, int> > > adjL, int s, int d, int V, list<pair<int,int>> &to_delete, pair<int,int> &simple_delete, bool &firstrun);

void routeone(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete);

int main() {
    int vertices;
    int flow;
    srand(time(NULL));
    int id, loc, countv = 0, counte = 0, source, dest;
    list<pair<int,int>> to_delete;
    pair<int,int> simple_delete;
    bool firstfind = true;

    //Opening Output Files

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


    do
    {
        source = rand() % vertices;
        dest = rand() % vertices;
    }while(!(bfs(adjacencyList, source, dest, vertices) && depth >= 10));

    //===========Change Source to weight 20===========
    list< pair<int, int> >::iterator itr = adjacencyList[source].begin();
    while (itr != adjacencyList[source].end()) {
        itr->second = 20;
        ++itr;
    }
    cout <<"Source: "<<source<<endl;
    cout <<"Dest: "<<dest<<endl;
    cout <<"Depth: "<<depth<<endl;
    routeone(adjacencyList, source, dest, vertices, to_delete, simple_delete);
    return 0;

}

//Used to remove an edge (Attack it)
void vecerase(vector< list< pair<int, int> > >& adjList, int i, int s)
{
    list<pair<int,int>> li = adjList[s];
    adjList[i].clear();
    for(auto it=li.rbegin();it!=li.rend();++it) {
        //cout << (*it).first<< '\n';
        if(it->first != i) {
            adjList[i].push_back(make_pair(it->first,it->second));
        }
    }
}


//Checks to see if the selected values can actually be reached
bool bfs(vector<list<pair<int, int> > > &adjL, int s, int d, int V) {


    bool *visited = new bool[V];
    depth = 0;
    bool notzero = true;
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
            if(itr->second == 0)
                notzero = false;
           else if(!visited[itr->first] && itr->second >= 1)
            {
                notzero = true;
                visited[itr->first] = true;
                if(itr->first == d) {
                    itr->second = 20;
                    return true;
                }
                queue.push_back((*itr).first);
            }
        }
        if(notzero)
            depth++;
    }
   delete[](visited);

    return false;
}

int fordFulkerson(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete, bool &firstrun1) {
    bool firstrun = true;
    int max_delete = 0;
    int path_flow;
    int foundd = 0;
    int perms = s;
    int countforvec = 0;
    int tempflow;
    bool addedmax = true;
    vector< list< pair<int, int> > > rlist(adjL);

    int max_flow = 0;
    list< pair<int, int> >::iterator itr = rlist[s].begin();
    list<int> queue;
    queue.push_back(s);

    while(bfs(rlist,perms,d,V))
    {
        if(firstrun)
            path_flow = INT_MAX;
        s = queue.front();
        queue.pop_front();

        for(itr = rlist[s].begin(); itr != rlist[s].end(); ++itr) {
            if (firstrun)
                firstrun = false;
            queue.push_back((*itr).first);
            if (foundd != 1 && itr->second != 0)
            {
                path_flow = min(path_flow, itr->second);
            }
            if(foundd == 1)
            {
                tempflow = itr->second;
                itr->second -= path_flow;
                if(itr->second < 0)
                    itr->second = 0;
                if(max_delete < itr->second && s != perms && itr->first != d)
                {
                    max_delete = itr->second;
                    //Change to list
                    if(firstrun1) {
                        to_delete.push_back(make_pair(s, itr->first));
                    }
                    simple_delete.first = s;
                    simple_delete.second = itr->first;
                }
                if(tempflow == path_flow)
                    if(foundd == 1 && !addedmax) {
                        max_flow += path_flow;
                        addedmax = true;
                    }
            }
            if(itr->second < 0)
                itr->second = 0;
            if(itr->first == d) {
                if(foundd == 0) {
                    foundd++;
                    addedmax = false;
                    queue.clear();
                    queue.push_back(perms);
                    break;
                }
                else{
                    foundd = 0;
                    path_flow = INT_MAX;
                    addedmax = false;
                    queue.clear();
                    queue.push_back(perms);
                    break;
                }
            }
        }
    }
    firstrun1 = false;
    queue.clear();
    return max_flow;
}

void routeone(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete) {
    int time = 100;
    bool firstrun = true;
    bool firstrun1 = true;
    cout <<"Depth inside routeone: "<<depth<<endl;
    int tempdepth = depth;
    int flow = fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
    fot.open("../flowovertime.csv");
    fot<<"Time, Flow"<<endl;
    fdp.open("../flowdisp.csv"); //Not sure what to output on this one
    ipk.open("../impactK.csv");
    ipk<<"Time, Number of Nodes"<<endl;
    do {
        fot << time << "," << flow << endl;
        if(firstrun1) {
            firstrun1 = false;
            ipk << time << "," << tempdepth << endl;
        }
        else
            ipk << time <<"," << depth<<endl;
        vecerase(adjL, to_delete.front().second, to_delete.front().first);
        if(!to_delete.empty())
            to_delete.pop_front();
        time--;
        flow = fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
        bfs(adjL, s, d, V);
    }while(time != 0);
}

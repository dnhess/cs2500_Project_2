//#include "node.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <utility>
#include <queue>
using namespace std;


int depth;
bool getpath;
int globalmaxflow = INT_MAX;
int globalsource_delete = NULL;
int globallocation_delete = NULL;
ofstream fot;
ofstream fdp;
ofstream ipk;

//Declaring Functions
void vecerase(vector< list< pair<int, int> > > &adjL, int i, int s);
bool bfs(vector< list< pair<int, int> > > &adjL, int s, int d, int V);

void printAllPaths(vector<list<pair<int, int> > > adjL, int s, int d, int V, vector<int> &cp, bool getpath);
void printAllPathsUtil(vector<list<pair<int, int> > > adjL, int u, int d, bool visited[],
                       int path[], int &path_index, int weight, vector<int> &cp);
void attacktwo(vector<list<pair<int, int> > > &adjL);
void attackone(vector<list<pair<int, int> > > &adjL, int s, int d, int V, vector<int> &cp);
void randomattack(vector<list<pair<int, int> > > adjL, int V);


int main() {
    int vertices;
    int tempid = 0;
    int choice;
    srand(time(NULL));
    int id, loc, countv = 0, counte = 0, source, dest;
    list<pair<int,int>> to_delete;
    pair<int,int> simple_delete;
    bool firstfind = true;

    //Opening Output Files
    cout <<"1. Static Calculation"<<endl;
    cout <<"2. Reactive Calculation"<<endl;
    cin >>choice;
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

    cout <<"Verticies: "<<vertices<<endl;

    vector< list< pair<int, int> > > adjacencyList(vertices + 1);

    vector<int> currentpath(vertices);

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
                if(id != tempid-1 || id != tempid)
                {
                    while(id != tempid) {
                        adjacencyList[tempid].push_back(make_pair(rand() % vertices, (rand() % 20) + 1));
                        tempid++;
                    }
                }
                else
                {
                    tempid = id;
                }
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

//    for (int i = 0; i < adjacencyListstatic_attack2.size(); ++i) {
//        printf("adjacencyList[%d]", i);
//
//        list<pair<int, int> >::iterator itr1_attck2 = adjacencyListstatic_attack2[i].begin();
//
//        while (itr1_attck2 != adjacencyListstatic_attack2[i].end()) {
//            printf(" -> %d(%d)", itr1_attck2->first, itr1_attck2->second);
//            ++itr1_attck2;
//        }
//        cout <<endl;
//    }
    do
    {
        source = rand() % vertices;
        dest = rand() % vertices;
        printAllPaths(adjacencyList, source, dest, vertices, currentpath, true);
//        if(depth >=25)
          //  cout <<"The value of depth: "<<depth<<endl;
        cout<<currentpath.size()<<endl;
    }while(!(currentpath.size() == 50 && globalmaxflow >= 0 && globalmaxflow != INT_MAX));

//    ===========Change Source to weight 20===========
    list< pair<int, int> >::iterator itr = adjacencyList[source].begin();
    while (itr != adjacencyList[source].end()) {
        itr->second = 20;
        ++itr;
    }
    vector< list< pair<int, int> > > adjacencyListstatic_attack2 = adjacencyList;
    vector< list< pair<int, int> > > randomlist = adjacencyList;

//    source = 428;
//        dest = 595;
//        printAllPaths(adjacencyList, source, dest, vertices, currentpath);
    cout <<"Source: "<<source<<endl;
    cout <<"Dest: "<<dest<<endl;
    cout <<"Depth: "<<currentpath.size()<<endl;
    cout <<"Value of Flow before anything: "<<globalmaxflow<<endl;
//    routeone(adjacencyList, source, dest, vertices, to_delete, simple_delete);
//    routetwo(adjacencyList, source, dest, vertices, to_delete, simple_delete);


    if(choice == 1)     //======STATIC======
    {
        //ATTACK 1
        fot.open("../attack1flowovertime.csv");
        fot<<"Time, Flow"<<endl;
        ipk.open("../attack1impactK.csv");
        ipk<<"Time, Number of Nodes"<<endl;
        int time = 100;
        cout <<"value of flow "<<globalmaxflow<<endl;

        //Static routing sticks with the same path so I guess when one link is deleted the flow drops to 0. This is due
        //to the static sticking to certain paths.
        //First attack strategy will delete every edge past depth 1 (assuming the source starts at 0)
        //Starting the first attack
        while(time != 0)
        {
            fot << time << "," << globalmaxflow << endl;
            ipk << time <<"," << currentpath.size()<<endl;
            attackone(adjacencyList, source, dest, vertices, currentpath);
            time--;
            if(globalmaxflow != 0)
                printAllPaths(adjacencyList, source, dest, vertices, currentpath, false);
        }
        fot.close();
        ipk.close();

        //ATTACK 2
        globalmaxflow = INT_MAX;
        fot.open("../attack2flowovertime.csv");
        fot<<"Time, Flow"<<endl;
        ipk.open("../attack2impactK.csv");
        ipk<<"Time, Number of Nodes"<<endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, false);
        while(time != 0)
        {
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time <<"," << currentpath.size()<<endl;
            attacktwo(adjacencyListstatic_attack2);
            time--;
            if(globalmaxflow != 0)
                printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, false);
        }
        fot.close();
        ipk.close();
        //TODO: Random is not outputting anything
        //Random
        globalmaxflow = INT_MAX;
        fot.open("../randflowovertime.csv");
        fot<<"Time, Flow"<<endl;
        ipk.open("../randimpactK.csv");
        ipk<<"Time, Number of Nodes"<<endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, false);
        while(time != 0)
        {
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time <<"," << currentpath.size()<<endl;
            randomattack(randomlist, vertices);
            time--;
            if(globalmaxflow != 0)
                printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, false);
        }
        fot.close();
        ipk.close();
    }

    //TODO: The reactive path is not being recaulted at each time
    if(choice == 2)     //======REACTIVE======
    {
        //ATTACK 1
        fot.open("../attack1flowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../attack1impactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        int time = 100;
        cout << "value of flow " << globalmaxflow << endl;
        while (time != 0) {
            fot << time << "," << globalmaxflow << endl;
            ipk << time << "," << currentpath.size() << endl;
            attackone(adjacencyList, source, dest, vertices, currentpath);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = INT_MAX;
                printAllPaths(adjacencyList, source, dest, vertices, currentpath, true);
            }
        }
        fot.close();
        ipk.close();

        //ATTACK 2
        globalmaxflow = INT_MAX;
        fot.open("../attack2flowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../attack2impactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
        while (time != 0) {
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time << "," << currentpath.size() << endl;
            attacktwo(adjacencyListstatic_attack2);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = INT_MAX;
                printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
                cout <<"Current path size: "<<currentpath.size()<<endl;
            }

        }
        //TODO: Random is not outputting anything
        //Random
        globalmaxflow = INT_MAX;
        fot.open("../randflowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../randimpactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
        while (time != 0) {
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time << "," << currentpath.size() << endl;
            globalmaxflow = INT_MAX;
            randomattack(randomlist, vertices);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = INT_MAX;
                printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
            }
        }
        fot.close();
        ipk.close();
        return 0;
    }
}

//Used to remove an edge (Attack it)
void vecerase(vector< list< pair<int, int> > >& adjList, int i, int s)
{
    //i is what it is looking for
    //s is the position it starts from
    list<pair<int,int>> li = adjList[s];
    adjList[s].clear();
//    if(distance(li.begin(),li.end()) == 0)
//    else
//    {
        for (auto it = li.rbegin(); it != li.rend(); ++it) {
            //cout << (*it).first<< '\n';
            if (it->first != i) {
//                cout << "Making a pair out of: " << it->first << " and " << it->second << endl;
                adjList[i].push_back(make_pair(it->first, it->second));
            } //else cout << "Not adding: " << it->first << " and " << it->second << endl;
        }
//    }
}


//Checks to see if the selected values can actually be reached
bool bfs(vector<list<pair<int, int> > > &adjL, int s, int d, int V) {
    bool *visited = new bool[V];
    bool *finddv = new bool[V];
    depth = 0;
    bool notzero = true;
    bool findazero = false;
    for(int i = 0; i < V; i++)
        visited[i] = false;

    list<int> queue;
    list<int> findd;

    visited[s] = true;
    queue.push_back(s);
    findd.push_back(s);

    list< pair<int, int> >::iterator itr = adjL[s].begin();

    while(!queue.empty())
    {
        s = queue.front();
        queue.pop_front();
        for(itr = adjL[s].begin(); itr != adjL[s].end(); itr++)
        {
            if(itr->second == 0)
                notzero = false;
           if(!visited[itr->first] && itr->second != 0)
            {
                notzero = true;
                visited[itr->first] = true;
                if(itr->first == d) {
                    depth++;
                    itr->second = 20;
                    return true;
                }
                if(findazero == false)
                  queue.push_back((*itr).first);
            }
        }
        if(notzero && distance(adjL[s].begin(),adjL[s].end()) != 0)
            depth++;
    }
    depth = 0;
   delete[](visited);
    return false;
}

void attacktwo(vector<list<pair<int, int> > > &adjL)
{
    if(globalmaxflow != 0)
    {
        vecerase(adjL, globallocation_delete, globalsource_delete);
        cout <<"Looking for1: "<<globallocation_delete<<endl;
        cout <<"Starting at1: "<<globalsource_delete<<endl;
    }
}

void randomattack(vector<list<pair<int, int> > > adjL, int V) {
    fot.open("../attack2flowovertime.csv");
    fot<<"Time, Flow"<<endl;
    fdp.open("../attack2flowdisp.csv"); //Not sure what to output on this one
    ipk.open("../attack2impactK.csv");
    vecerase(adjL, rand() % V, rand() % V);
}

// Prints all paths from 's' to 'd'
void printAllPaths(vector<list<pair<int, int> > > adjL, int s, int d, int V, vector<int> &cp, bool getpath)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];

    // Create an array to store paths
    int *path = new int[V];
    int path_index = 0; // Initialize path[] as empty

    // Initialize all vertices as not visited
    for (int i = 0; i < V; i++)
        visited[i] = false;
    // Call the recursive helper function to print all paths
    if(getpath)
        cp.clear();
    printAllPathsUtil(adjL, s, d, visited, path, path_index, 20, cp);
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void printAllPathsUtil(vector<list<pair<int, int> > > adjL, int u, int d, bool visited[],
                              int path[], int &path_index, int weight, vector<int> &cp)
{
    bool foundsomething = false;
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
//    cout <<"Gets here"<<endl;
//    cout <<"Gets here2"<<endl;
    path_index++;
    // If current vertex is same as destination, then print
    // current path[]
    if (u == d)
    {
        globalmaxflow = INT_MAX;
        cp.clear();
        for (int i = 0; i < path_index; i++)
            cp.push_back(path[i]);
        for(int ip = 0; ip < cp.size(); ip++)
        {
            list< pair<int, int> >::iterator itr = adjL[cp[ip]].begin();
//            cout <<"value of ip: "<<ip<<endl;
//            for(itr = adjL[ip].begin(); itr != adjL[cp[ip]].end(); ++itr) {
            do
            {
                if(itr->first == cp[ip + 1] || ip == cp.size() - 1 || itr->first == d) {
                    globalmaxflow = min(globalmaxflow, itr->second);
//                    cout<<"value of max flow: "<<globalmaxflow<<endl;
                    globalsource_delete = cp[ip];
                    globallocation_delete =  itr->first;
                    foundsomething = true;
                }
                itr++;
            }while(itr != adjL[cp[ip]].end());
            if(!foundsomething)
            {
                cout <<"IN HERE"<<endl;
                globalmaxflow = 0;
                break;
            }
            foundsomething = false;
        }
        for (int i = 0; i<path_index; i++)
//            cout << path[i] << " value of i "<<i<<" ";
//        cout << endl;
//        cout <<"Path index size: "<<path_index - 1<<endl;
//        cout <<"Size of cp: "<<cp.size()<<endl;
        return;
    }
//    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        list< pair<int, int> >::iterator i = adjL[u].begin();
        for (i = adjL[u].begin(); i != adjL[u].end(); ++i)
            if (!visited[i->first])
                printAllPathsUtil(adjL, i->first, d, visited, path, path_index, i->second, cp);
    }
}

void attackone(vector<list<pair<int, int> > > &adjL, int s, int d, int V, vector<int> &cp) {
//    //i is what it is looking for
//    //s is the position it starts from
//    void vecerase(vector< list< pair<int, int> > >& adjList, int i, int s)
    list<pair<int, int>>::iterator itr = adjL[s].begin();
    vecerase(adjL, cp[2], cp[1]);
//    cout <<"Looking for: "<<cp[2]<<endl;
//    cout <<"Starting at: "<<cp[1]<<endl;
}



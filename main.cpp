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
int globalmaxflow = 0;
int globalsource_delete = NULL;
int globallocation_delete = NULL;
ofstream fot;
ofstream fdp;
ofstream ipk;

//Declaring Functions
void getmaxflow(vector<list<pair<int, int> > > adjL, vector<int> &cp);
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

    do
    {
        source = rand() % vertices;
        dest = rand() % vertices;
        printAllPaths(adjacencyList, source, dest, vertices, currentpath, true);
        getmaxflow(adjacencyList, currentpath);
    }while(!(currentpath.size() <= 50 && globalmaxflow > 0));

//    ===========Change Source to weight 20===========
    list< pair<int, int> >::iterator itr = adjacencyList[source].begin();
    while (itr != adjacencyList[source].end()) {
        itr->second = 20;
        ++itr;
    }
    vector< list< pair<int, int> > > adjacencyListstatic_attack2 = adjacencyList;
    vector< list< pair<int, int> > > randomlist = adjacencyList;

    cout <<"Depth: "<<currentpath.size()<<endl;
    cout <<"Value of Flow before anything: "<<globalmaxflow<<endl;


    if(choice == 1)     //======STATIC======
    {
        //ATTACK 1
        fot.open("../attack1flowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../attack1impactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        int time = 100;
        cout << "value of flow " << globalmaxflow << endl;
        while (time != 0) {
            getmaxflow(adjacencyList, currentpath);
            fot << time << "," << globalmaxflow << endl;
            ipk << time << "," << currentpath.size() << endl;
            attackone(adjacencyList, source, dest, vertices, currentpath);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = 0;
                printAllPaths(adjacencyList, source, dest, vertices, currentpath, true);
            }
        }
        fot.close();
        ipk.close();

        //ATTACK 2
        globalmaxflow = 0;
        fot.open("../attack2flowovertime.csv");
        fot<<"Time, Flow"<<endl;
        ipk.open("../attack2impactK.csv");
        ipk<<"Time, Number of Nodes"<<endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, false);
        getmaxflow(adjacencyListstatic_attack2, currentpath);
        while(time != 0)
        {
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time <<"," << currentpath.size()<<endl;
            attacktwo(adjacencyListstatic_attack2);
            time--;
            if(globalmaxflow != 0)
                getmaxflow(adjacencyListstatic_attack2, currentpath);
        }
        fot.close();
        ipk.close();
        //TODO: Random is not outputting anything
        //Random
        globalmaxflow = 0;
        fot.open("../randflowovertime.csv");
        fot<<"Time, Flow"<<endl;
        ipk.open("../randimpactK.csv");
        ipk<<"Time, Number of Nodes"<<endl;
        time = 100;
        printAllPaths(randomlist, source, dest, vertices, currentpath, false);
        getmaxflow(randomlist, currentpath);
        while(time != 0)
        {
            fot << time << "," << globalmaxflow << endl;
            cout << time << "," << globalmaxflow << endl;
            ipk << time <<"," << currentpath.size()<<endl;
            randomattack(randomlist, vertices);
            time--;
            if(globalmaxflow != 0)
                getmaxflow(randomlist, currentpath);
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
            getmaxflow(adjacencyList, currentpath);
            fot << time << "," << globalmaxflow << endl;
            ipk << time << "," << currentpath.size() << endl;
            attackone(adjacencyList, source, dest, vertices, currentpath);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = 0;
                printAllPaths(adjacencyList, source, dest, vertices, currentpath, true);
            }
        }
        fot.close();
        ipk.close();

        //ATTACK 2
        globalmaxflow = 0;
        fot.open("../attack2flowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../attack2impactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
        while (time != 0) {
            getmaxflow(adjacencyList, currentpath);
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time << "," << currentpath.size() << endl;
            attacktwo(adjacencyListstatic_attack2);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = 0;
                printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
                cout <<"Current path size: "<<currentpath.size()<<endl;
            }

        }
        fot.close();
        ipk.close();
        //TODO: Random is not outputting anything
        //Random
        globalmaxflow = 0;
        fot.open("../randflowovertime.csv");
        fot << "Time, Flow" << endl;
        ipk.open("../randimpactK.csv");
        ipk << "Time, Number of Nodes" << endl;
        time = 100;
        printAllPaths(adjacencyListstatic_attack2, source, dest, vertices, currentpath, true);
        while (time != 0) {
            getmaxflow(adjacencyList, currentpath);
            fot << time << "," << globalmaxflow << endl;
//            cout <<time <<" , "<<globalmaxflow<<endl;
            ipk << time << "," << currentpath.size() << endl;
            globalmaxflow = 0;
            randomattack(randomlist, vertices);
            time--;
            if (globalmaxflow != 0) {
                globalmaxflow = 0;
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
            } //else cout << "Not adding: " << i << " and " << s << endl;
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
    bool stop = false;
    int tempflow;
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
        cp.clear();
        for (int i = 0; i < path_index; i++)
            cp.push_back(path[i]);
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
    list<pair<int, int>>::iterator itr = adjL[cp[2]].begin();
//            cout << "SOURCE: " << cp[2] << endl;
//            cout << "DEST: " << itr->first << endl;
    if(itr->first == 0){}
    else
            vecerase(adjL, cp[2], cp[1]);
}

void getmaxflow(vector<list<pair<int, int> > > adjL, vector<int> &cp)
{
    globalmaxflow = 0;
    int tempflow;
    bool findsnothing = false;
    for(int i = 0; i < cp.size(); i++)
    {
        list< pair<int, int> >::iterator itr;
        for(itr = adjL[cp[i]].begin(); itr != adjL[cp[i]].end(); ++itr)
        {
//            cout <<"Itr first value: "<<itr->first<<endl;
//            cout <<"CP I+1 value: "<<cp[i + 1]<<endl;
            if (itr->first != cp[i + 1])
                findsnothing = true;
            if(itr->first == cp[i + 1] || i == cp.size() - 1) {
                findsnothing = false;
                tempflow = globalmaxflow;
                globalmaxflow = max(globalmaxflow, itr->second);
                if(tempflow != globalmaxflow)
                {
                    globallocation_delete = itr->first;
                    globalsource_delete = cp[i];
                }
            }
        }
    }
    if(findsnothing) {
        globalmaxflow = 0;
        cp.clear();
    }
}


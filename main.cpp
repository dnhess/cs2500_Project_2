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

void printAllPaths(vector<list<pair<int, int> > > adjL, int s, int d, int V, vector<list<pair<int, int> > > &cp);
void printAllPathsUtil(vector<list<pair<int, int> > > adjL, int u, int d, bool visited[],
                       int path[], int &path_index, int weight, vector<list<pair<int, int> > > &cp);
void routeone(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete,pair<int,int> & simple_delete);
void routetwo(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete);
void randomattack(vector<list<pair<int, int> > > adjL, int s, int d, int V);


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

    vector< list< pair<int, int> > > adjacencyList(vertices + 1);
    vector< list< pair<int, int> > > currentpath(vertices);

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

    for (int i = 0; i < adjacencyList.size(); ++i) {
        printf("adjacencyList[%d]", i);

        list<pair<int, int> >::iterator itr1 = adjacencyList[i].begin();

        while (itr1 != adjacencyList[i].end()) {
            printf(" -> %d(%d)", itr1->first, itr1->second);
            ++itr1;
        }
        cout <<endl;
    }
    do
    {
        source = rand() % vertices;
        dest = rand() % vertices;
        printAllPaths(adjacencyList, source, dest, vertices, currentpath);
//        if(depth >=25)
          //  cout <<"The value of depth: "<<depth<<endl;
        cout<<currentpath.size()<<endl;
    }while(!(bfs(adjacencyList, source, dest, vertices) && currentpath.size() == 50));

    //===========Change Source to weight 20===========
    list< pair<int, int> >::iterator itr = adjacencyList[source].begin();
    while (itr != adjacencyList[source].end()) {
        itr->second = 20;
        ++itr;
    }
    cout <<"Source: "<<source<<endl;
    cout <<"Dest: "<<dest<<endl;
    cout <<"Depth: "<<currentpath.size()<<endl;
//    routeone(adjacencyList, source, dest, vertices, to_delete, simple_delete);
//    routetwo(adjacencyList, source, dest, vertices, to_delete, simple_delete);


    //======STATIC======
    if(choice == 1)
    {
    }
    return 0;

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

int fordFulkerson(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete, bool &firstrun1) {
    bool firstrun = true;
    int max_delete = 0;
    int path_flow;
    int foundd = 0;
    int perms = s;
    int tempflow = 0;
    bool addedmax = true;
    simple_delete.first = 0;
    simple_delete.second = 0;
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
            if(foundd == 1 && itr->second != 0)
            {
                tempflow = itr->second;
                itr->second -= path_flow;
                if(itr->second < 0)
                    itr->second = 0;
                if(max_delete < itr->second && s != perms && itr->first != d)
                {
                    max_delete = itr->second;
                    if(to_delete.empty()) {
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
    rlist.clear();
    firstrun1 = false;
    queue.clear();
    return max_flow;
}

void routeone(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete) {
    int time = 100;
    bfs(adjL, s, d, V);
    bool firstrun = true;
    bool firstrun1 = true;
    int tempdepth = depth;
    int flow = fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
    fot.open("../attack1flowovertime.csv");
    fot<<"Time, Flow"<<endl;
    fdp.open("../attack1flowdisp.csv"); //Not sure what to output on this one
    ipk.open("../attack1impactK.csv");
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
    fot.close();
    ipk.close();
    fdp.close();
}

void routetwo(vector<list<pair<int, int> > > adjL, int s, int d, int V,list<pair<int,int>> &to_delete, pair<int,int> & simple_delete)
{
    int time = 100;
    bfs(adjL, s, d, V);
    bool firstrun = true;
    bool firstrun1 = true;
    int tempdepth = depth;
    int flow = fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
    fot.open("../attack2flowovertime.csv");
    fot<<"Time, Flow"<<endl;
    fdp.open("../attack2flowdisp.csv"); //Not sure what to output on this one
    ipk.open("../attack2impactK.csv");
    ipk<<"Time, Number of Nodes"<<endl;
    do {
//        cout << time <<","<<flow<<endl;
        fot << time << "," << flow << endl;
        if(firstrun1) {
            firstrun1 = false;
            ipk << time << "," << tempdepth << endl;
        }
        else
            ipk << time <<"," << depth<<endl;
        fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
        vecerase(adjL, simple_delete.second, simple_delete.first);
        time--;
        if(bfs(adjL, s, d, V))
            flow = fordFulkerson(adjL, s, d, V, to_delete, simple_delete, firstrun);
        else
            flow = 0;
        bfs(adjL, s, d, V);
        cout <<"DEPTH======: "<<depth<<endl;
    }while(time != 0);
    fot.close();
    ipk.close();
    fdp.close();
}

void randomattack(vector<list<pair<int, int> > > adjL, int s, int d, int V) {
    fot.open("../attack2flowovertime.csv");
    fot<<"Time, Flow"<<endl;
    fdp.open("../attack2flowdisp.csv"); //Not sure what to output on this one
    ipk.open("../attack2impactK.csv");
    vecerase(adjL, rand() % V, rand() % V);
}

// Prints all paths from 's' to 'd'
void printAllPaths(vector<list<pair<int, int> > > adjL, int s, int d, int V, vector<list<pair<int, int> > > &cp)
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
    printAllPathsUtil(adjL, s, d, visited, path, path_index, 20, cp);
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void printAllPathsUtil(vector<list<pair<int, int> > > adjL, int u, int d, bool visited[],
                              int path[], int &path_index, int weight, vector<list<pair<int, int> > > &cp)
{
    // Mark the current node and store it in path[]
    visited[u] = true;
    path[path_index] = u;
//    cout <<"Gets here"<<endl;
    cp[path_index].push_back(make_pair(u,weight));
//    cout <<"Gets here2"<<endl;
    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d)
    {
        cp.resize(path_index);
//        for (int i = 0; i<path_index; i++)
//            cout << path[i] << " value of i "<<i<<" ";
////        cout <<"Path index size: "<<path_index - 1;
//        cout << endl;
//        cout <<"Size of cp: "<<cp.size();
//        cout <<"=================OUT PUTTING CP=================="<<endl;
//        for (int i = 0; i < cp.size(); ++i) {
//            printf("adjacencyList[%d]", i);
//
//            list<pair<int, int> >::iterator itr1 = cp[i].begin();
//
//            while (itr1 != cp[i].end()) {
//                printf(" -> %d(%d)", itr1->first, itr1->second);
//                ++itr1;
//            }
//            cout <<endl;
//        }
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
        list< pair<int, int> >::iterator i = adjL[u].begin();
        for (i = adjL[u].begin(); i != adjL[u].end(); ++i)
            if (!visited[i->first])
                printAllPathsUtil(adjL, i->first, d, visited, path, path_index, i->second, cp);
    }
}

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
//Declaring Functions
void vecerase(vector< list< pair<int, int> > > &adjL, int i, int s);
bool bfs(vector< list< pair<int, int> > > adjL, int s, int d, int V);
int fordFulkerson(vector< list< pair<int, int> > > adjL, int s, int d, int V);

int main() {
    int vertices;
    srand(time(NULL));
    int id, loc, countv = 0, counte = 0, source, dest;
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

//
//    for (int i = 0; i < adjacencyList.size(); ++i) {
//        printf("adjacencyList[%d] ", i);
//
//        list< pair<int, int> >::iterator itr = adjacencyList[i].begin();
//
//        while (itr != adjacencyList[i].end()) {
//            printf(" -> %d(%d)", itr->first, itr->second);
//            ++itr;
//        }
//        printf("\n");
//    }


    do
    {
        source = rand() % vertices;
        dest = rand() % vertices;
//        printf("Source: %d Dest: %d \n", source, dest);
//        printf("True of false? %d \n", bfs(adjacencyList, source, dest, vertices));
//        printf("Depth: %d \n", depth);
    }while(!(bfs(adjacencyList, source, dest, vertices) && depth >= 10));
//    cout <<"source: "<<source<<endl;
//    cout <<"dest: "<<dest<<endl;
//        for (int i = 0; i < adjacencyList.size(); ++i) {
//        printf("adjacencyList[%d] ", i);
//
//        list< pair<int, int> >::iterator itr = adjacencyList[i].begin();
//
//        while (itr != adjacencyList[i].end()) {
//            printf(" -> %d(%d)", itr->first, itr->second);
//            ++itr;
//        }
//        printf("\n");
//    }
    cout <<"Result: \n"<<fordFulkerson(adjacencyList, source, dest, vertices)<<endl;
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
bool bfs(vector<list<pair<int, int> > > adjL, int s, int d, int V) {


    bool *visited = new bool[V];
    depth = 0;
    for(int i = 0; i < V; i++)
        visited[i] = false;

    list<int> queue;

    visited[s] = true;
    queue.push_back(s);

    list< pair<int, int> >::iterator itr = adjL[s].begin();

    while(!queue.empty())
    {
        s = queue.front();
        //cout <<s<<" "<<endl;
        queue.pop_front();


        for(itr = adjL[s].begin(); itr != adjL[s].end(); itr++)
        {
            if(!visited[itr->first] && itr->second >= 1)
            {
                visited[itr->first] = true;
                if(itr->first == d) {
//                    queue.clear();
                    return true;
                }
                queue.push_back((*itr).first);
            }
        }
        depth++;
    }
   delete[](visited);

    return false;
}

int fordFulkerson(vector<list<pair<int, int> > > adjL, int s, int d, int V) {
    bool firstrun = true;
    int path_flow;
    int foundd = 0;
    int perms = s;
    int tempflow;
    bool addedmax = true;
    vector< list< pair<int, int> > > rlist(adjL);

    int max_flow = 0;
    cout <<"GET HERES: "<<endl;
    list< pair<int, int> >::iterator itr = rlist[s].begin();
    list<int> queue;
    //list<int> tofollow;
    queue.push_back(s);

    cout <<"NOW HERE"<<endl;
    while(bfs(rlist,perms,d,V))
    {
        if(firstrun)
            path_flow = INT_MAX;
        cout <<"Does this thing cause issues? L "<<endl;
        s = queue.front();
        cout <<"What is in front of the queue? "<<queue.front()<<endl;
        cout <<"What about this? : "<<endl;
        //This causes issues when trying to pop 0
        queue.pop_front();
        cout <<"Get past the popping? "<<endl;

        for(itr = rlist[s].begin(); itr != rlist[s].end(); ++itr) {
            cout <<"Gets here"<<endl;
            if (firstrun)
                firstrun = false;
            cout << "ITR Value: " << itr->first << endl;
            cout << "Weight: " << itr->second << endl;
            queue.push_back((*itr).first);
            if (foundd != 1 && itr->second != 0)
            {
                path_flow = min(path_flow, itr->second);
            }
            cout <<"path flow value: "<<path_flow<<endl;
            if(foundd == 1)
            {
                cout <<"Old weight: "<<itr->second<<endl;
                tempflow = itr->second;
                itr->second -= path_flow;
                if(itr->second < 0)
                    itr->second = 0;
                if(tempflow == path_flow)
                    goto jump;
                cout <<"New Weight: "<<itr->second<<endl;
            }
            cout<<"Step 1"<<endl;
            if(itr->second < 0)
                itr->second = 0;
            cout<<"Step 2"<<endl;
            if(itr->first == d) {
                if(foundd == 0) {
                    foundd++;
                    addedmax = false;
                    cout <<"Gets in here1"<<endl;
                    queue.clear();
                    queue.push_back(perms);
                    cout <<"After mumbo jumbo"<<endl;
                    break;
                }
                else{
                    foundd = 0;
                    path_flow = INT_MAX;
                    addedmax = false;
                    cout <<"LELLLELELEWFWEF: "<<queue.size()<<endl;
                    queue.clear();
                    queue.push_back(perms);
                    cout <<"AFTER CAADFAD"<<endl;
                    break;
                }
            }
        }
jump:
        cout<<"Step 3"<<endl;
        if(foundd == 1 && !addedmax) {
            max_flow += path_flow;
            addedmax = true;
        }
        //cout <<"Still reachable?: "<<bfs(rlist,s,d,V)<<endl;
    }
    cout <<"Does it get here?: "<<endl;
    queue.clear();
    return max_flow;
}


/*
 * 322 -> 323(10) -> 429(3)
 * 323 -> 336(6)
 * 336
 * 429 -> 430(20) -> 431(19)
 * 430  -> 535(19)
 * 431 -> 432(16)
 * 432 -> 433(1) -> 437(14)
 * 433 -> 435(8)
 * 435
 * 437 -> 670
 * 535 -> 536(19)
 * 536 -> 537(10)
 * 536 -> 538(14)
 * 538
 */

#include "node.h"
#include <fstream>
#include <string>
using namespace std;

int main() {
    vector<node> nodes;
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
//                id = atoi(line.c_str());
//                cout <<id<<endl;
//                nodes.push_back(node(id,true));
//                nodes[id].id = id;
//                nodes[id].active = true;
            }
            if (line == "source") {
                if(firstfind)
                {
                    firstfind = false;
                } else counte++;
                fin >> line;
                id = atoi(line.c_str());
            }
//            if (line == "target") {
//                fin >>line;
//                loc = atoi(line.c_str());
//                //nodes[id].beside.push_back(&nodes[loc]);
//            }
            if(fin.eof()) break;
        }
    }

    fin.close();

    cout<<"Number of verticies: "<<countv<<endl;
    cout<<"Number of edges: "<<counte<<endl;
//    cout <<"Size of nodes: "<<nodes.size()<<endl;
//    for(int i = 0; i < nodes.size(); i++)
//    {
//        cout <<"ID:"<<nodes[i].id<<endl;
//        cout <<"ACTIVE: "<<nodes[i].active<<endl;
//    }
    return 0;
}

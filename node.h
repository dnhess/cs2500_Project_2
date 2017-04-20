//
// Created by Dillon Hess on 4/17/17.
//

#ifndef CS2500_PROJECT_2_NODE_H
#define CS2500_PROJECT_2_NODE_H

#include <iostream>
#include <vector>

using namespace std;
class node{
public:
    int src;
    int dest;
    int weight;
    vector<shared_ptr<node>> beside;
    node(int src1, int dest1, int weight1) : src(src1), dest(dest1), weight(weight1){}
};
//class less_than
//{
//public:
//    inline bool operator() (const node& id1, const node& id2)
//    {
//        return (id1.id < id2.id);
//    }
//};
#endif //CS2500_PROJECT_2_NODE_H

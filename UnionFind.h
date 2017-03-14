//
// Created by Victor on 3/11/2017.
//

#ifndef NETWORKSIMULATOR_UNIONFIND_H
#define NETWORKSIMULATOR_UNIONFIND_H

#include "Computer.h"

// path compression and union find
struct UnionFindNode{
    int rank = 1;
    int parent;
    UnionFindNode(int p_parent);
    UnionFindNode() = default;
};

class UnionFind {
    int size;
    UnionFindNode * set;
public:
    UnionFind(int size);
    int join(int a, int b); // do union by rank to point smaller set size to larger set size
    int find(int nodeIndex); // do path compression so that after you find parent, directly set this node to parents
    int rootCount();
};


#endif //NETWORKSIMULATOR_UNIONFIND_H

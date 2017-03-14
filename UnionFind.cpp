//
// Created by Victor on 3/11/2017.
//

#include "UnionFind.h"
#include "Globals.h"

UnionFindNode::UnionFindNode(int p_parent): parent(p_parent){}

UnionFind::UnionFind(int p_size) : size(p_size), set(new UnionFindNode[p_size]){
    for(int i = 0; i < size; i++){
        set[i] = UnionFindNode(i);
    }
}
int UnionFind::join(int a, int b){
    a = find(a);
    b = find(b);
    if(a == b) return a;

    //Union by rank
    if(set[a].rank < set[b].rank){
        set[a].parent = b;
        set[b].rank += set[a].rank;

        return b;
    }
    else {
        set[b].parent = a;
        set[a].rank += set[b].rank;

        return a;
    }
}
int UnionFind::find(int nodeIndex){

    int originalNode = nodeIndex;
    while(set[nodeIndex].parent != nodeIndex)
        nodeIndex = set[nodeIndex].parent;

    //Path compression optimization
    return set[originalNode].parent = nodeIndex;

}

int UnionFind::rootCount(){
    int count = 0;
    for(int i = 0; i < size; i++){
        if(set[i].parent == i && !Globals::computers[i].infected){
            count++;
        }
    }
    return count;
}

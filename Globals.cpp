//
// Created by Victor on 2/10/2017.
//

#include "EventQueue.h"
#include "Globals.h"
#include "UnionFind.h"

namespace Globals {
    bool fixInProgress = false;
    int num_infected = 0;

    //SimState sim_state = Running;

    std::mt19937_64 mt;

    Computer *computers;

    int **edges;

    EventQueue events;

    int** MST;

    int num_attackers, num_sysadmin, num_nodes;

    int num_attacks = 0;

    std::uniform_int_distribution<int> randomFixTime(1000, 2000);
    std::uniform_int_distribution<int> randomAttackTime(100, 1000);

    //O(1) enqueue and dequeue

    std::queue<Computer *> fixes; //queue for broken computers sysadmin needs to fix


    std::unordered_set<int> infected;

    bool isRebuild = false;


    //Edge objects used as objects in rebuildMST
    struct Edge{
        int weight;
        int firstNode;
        int secondNode;
        Edge(int p_firstNode, int p_secondNode, int p_weight){
            firstNode = p_firstNode;
            secondNode = p_secondNode;
            weight = p_weight;
        }
        bool operator < (const Edge& a) const
        {
            return this->weight < a.weight;
        }
        bool operator > (const Edge& a) const
        {
            return this->weight > a.weight;
        }
    };

    template<typename T>
    using min_priority_queue = priority_queue<T, std::vector<T>, std::greater<T>>;

    void buildOriginalMST(int** edges){
//Create priority_queue of Edge objects
        min_priority_queue<Edge> queue;
        for (int i = 0; i < num_nodes; i++) {
            for(int k = 0; k < i; k++){
                if(edges[i][k] != -1 && i != k){
                    queue.push(Edge(i,k,edges[i][k]));
                }
            }
        }
        //Initialize all nodes to -1 and diagonal to 0
        MST = new int*[num_nodes];
        for(int i = 0; i < num_nodes; i++){
            MST[i] = new int[num_nodes];
            MST[i][i] = 0;
            for(int k = 0; k < num_nodes; k++){
                MST[i][k] = -1;
            }
        }

        //Add all edges that will form MST using Kruskal's algorithm

        UnionFind unionFind(num_nodes);
        while(!queue.empty()){
            Edge edge = queue.top();
            queue.pop();

            if((unionFind.find(edge.firstNode) != unionFind.find(edge.secondNode))&& !computers[edge.secondNode].infected && !computers[edge.firstNode].infected ){
                MST[edge.firstNode][edge.secondNode] = MST[edge.secondNode][edge.firstNode] = edge.weight;
                unionFind.join(edge.firstNode, edge.secondNode);
            }
        }
    }


    bool buildAndPrintUnOptimal(int ** edges){
        min_priority_queue<Edge> queue;
        for (int i = 0; i < num_nodes; i++) {
            for(int k = 0; k < i; k++){
                if(MST[i][k] != -1 && i != k){
                    queue.push(Edge(i,k,MST[i][k]));
                }
            }
        }

//Add all edges that will form MST using Kruskal's algorithm


        int sum = 0;
        UnionFind unionFind(num_nodes);
        while(!queue.empty()){
            Edge edge = queue.top();
            queue.pop();

            if((unionFind.find(edge.firstNode) != unionFind.find(edge.secondNode))&& !computers[edge.secondNode].infected && !computers[edge.firstNode].infected ){
                sum += edge.weight;
                unionFind.join(edge.firstNode, edge.secondNode);
            }
        }
        cout << "Total cost of MST with MST - Unoptimal: " << sum << endl;

        if(unionFind.rootCount() > 1){
            return false;
        }
        return true;
    }

//Building Kruskal's algorithm. If true graph is complete, if false graph is partitioned
    bool rebuildMST1(int **edges){
        //Create priority_queue of Edge objects
        min_priority_queue<Edge> queue;
        for (int i = 0; i < num_nodes; i++) {
            for(int k = 0; k < i; k++){
                if(MST[i][k] != -1 && i != k){
                    queue.push(Edge(i,k,MST[i][k]));
                }
            }
        }
        //Initialize all nodes to -1 and diagonal to 0
        MST = new int*[num_nodes];
        for(int i = 0; i < num_nodes; i++){
            MST[i] = new int[num_nodes];
            MST[i][i] = 0;
            for(int k = 0; k < num_nodes; k++){
                MST[i][k] = -1;
            }
        }

        //Add all edges that will form MST using Kruskal's algorithm

        UnionFind unionFind(num_nodes);
        while(!queue.empty()){
            Edge edge = queue.top();
            queue.pop();

            if((unionFind.find(edge.firstNode) != unionFind.find(edge.secondNode))&& !computers[edge.secondNode].infected && !computers[edge.firstNode].infected ){
                MST[edge.firstNode][edge.secondNode] = MST[edge.secondNode][edge.firstNode] = edge.weight;
                unionFind.join(edge.firstNode, edge.secondNode);
            }
        }
        //cout << "Root count in MST: " << unionFind.rootCount() << endl;

        if(unionFind.rootCount() > 1){
            return false;
        }
        return true;

    }
    void printMST1(){
        //Print the missing nodes

        cout << "Missing nodes: ";
        for(auto computerId : infected) {
            cout << computerId << ", ";
        }

        cout << endl;

        if(!rebuildMST1(edges)){
            cout << "Cannot build a Spanning tree from and can't calculate cost because the graph is not connected. (Disjoint sets)" << endl;
        }
        else{
            int sum = 0;
            // Print the Minimum Spanning Tree graph
            //cout << "Minimum Spanning Tree Adjacency Matrix: " << endl;
            for (int i = 0; i < num_nodes; i++) {
                for (int j = 0; j < i; j++) {
                    //if (j > 0) cout << ".";
                    //cout << setw(4) << setfill(' ');
                    if (MST[i][j] >= 0){
                        //  cout << MST[i][j];
                        sum += MST[i][j];
                    }
                    //else cout << " ";
                }
/*
                for (int j = i; j < num_nodes; j++) {
                    cout << " " << setw(4) << setfill(' ');

                    if (MST[i][j] >= 0) cout << MST[i][j];
                    else cout << ".";

                }
                */
                //cout << endl;
            }
            // Print the total cost = sum of all weights in MST
            cout << "Total cost of MST with MST: " << sum << endl;
        }
    }


    bool buildAndPrintOptimal(){
        //Create priority_queue of Edge objects
        min_priority_queue<Edge> queue;
        for (int i = 0; i < num_nodes; i++) {
            for(int k = 0; k < i; k++){
                if(edges[i][k] != -1 && i != k){
                    queue.push(Edge(i,k,edges[i][k]));
                }
            }
        }

        int sum = 0;
        //Add all edges that will form MST using Kruskal's algorithm

        UnionFind unionFind(num_nodes);
        while(!queue.empty()){
            Edge edge = queue.top();
            queue.pop();

            if((unionFind.find(edge.firstNode) != unionFind.find(edge.secondNode))&& !computers[edge.secondNode].infected && !computers[edge.firstNode].infected ){
                sum += edge.weight;
                unionFind.join(edge.firstNode, edge.secondNode);
            }
        }
        cout << "Total cost of MST with original graph - Optimal: " << sum << endl;

        //cout << "Root count in MST: " << unionFind.rootCount() << endl;


        if(unionFind.rootCount() > 1){
            return false;
        }
        return true;

    }

    /*
    void printMST2(){
        int sum = 0;
        // Print the Minimum Spanning Tree graph
        //cout << "Minimum Spanning Tree Adjacency Matrix: " << endl;
        for (int i = 0; i < num_nodes; i++) {
            for (int j = 0; j < i; j++) {
                //if (j > 0) cout << ".";
                //cout << setw(4) << setfill(' ');
                if (edges[i][j] >= 0 ){
                    //  cout << MST[i][j];
                    sum += edges[i][j];
                }
                //else cout << " ";
            }

                for (int j = i; j < num_nodes; j++) {
                    cout << " " << setw(4) << setfill(' ');

                    if (MST[i][j] >= 0) cout << MST[i][j];
                    else cout << ".";

                }

            //cout << endl;
        }
        // Print the total cost = sum of all weights in MST
        cout << "Total cost of MST with original graph: " << sum << endl;
    }
*/
}
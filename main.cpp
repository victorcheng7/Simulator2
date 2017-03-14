#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <random>

#include "Computer.h"
#include "EventQueue.h"
#include "Globals.h"
#include "UnionFind.h"

using namespace Globals;
using namespace std;
/*
template<typename T>
using min_priority_queue = priority_queue<T, std::vector<T>, std::greater<T>>;
 */

int main(int argc, char **argv) {

//O(1) enqueue and dequeue implemented as queue in Globals.h. O(1) membership lookup implemented by including fixpending in Computer.h
//To compile g++ -std=c++11 -o test main.cpp Globals.cpp Computer.cpp EventQueue.cpp UnionFind.cpp


    if (argc < 4) {
        cout << "Usage: " << argv[0]
             << " <num_attackers> <num_sysadmin> <num_nodes> <random_seed>" << endl;

        return 1;
    }

    int random_seed = stoi(argv[4]);
    mt.seed(random_seed);
    num_attackers = stoi(argv[1]);
    num_sysadmin = stoi(argv[2]);
    num_nodes = stoi(argv[3]);

    int sim_time = 0; // time during the simulation

    computers = new Computer[num_nodes]; // List of computers in network
    edges = new int *[num_nodes];
    uniform_int_distribution<int> random1(-120, 100);
    uniform_int_distribution<int> random2(1, 100);

    // creating computers
    for (int j = 0; j < num_nodes; j++)
        computers[j] = Computer(j);

    for (int i = 0; i < num_nodes; i++) {
        edges[i] = new int[num_nodes];
        edges[i][i] = 0;

        for (int k = 0; k < i; k++) {
            edges[i][k] = edges[k][i] = random1(mt);
        }
        int max = edges[i][0], index = 0;
        for (int m = 0; m < i; m++)
            if (edges[i][m] > max) {
                max = edges[i][m];
                index = m;
            }
        if (max < 0) {
            edges[i][0] = random2(mt);
        }
    }
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < i; j++) {
            if (edges[i][j] < 0) {
                edges[i][j] = -1;
            }
        }
    }
// Printing original graph adjacency matrix
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (j > 0) cout << ".";
            cout << setw(4) << setfill(' ');
            if (edges[i][j] >= 0) cout << edges[i][j];
            else cout << " ";
        }
        cout << endl;
    }



//Create the initial MST to reference for future cost calculations
    buildOriginalMST(edges);




// num_attackers attack random computers a random number between 100-1000
    for(int i = 0; i < num_attackers; i++){
        events.addEvent(*new AttackerAttack(sim_time + (randomAttackTime(mt)), &computers[mt() % num_nodes]));
    }

    while (num_attacks < 2000) {
        //If there is something inside of the fix queue, a new sysadmin has been freed up so Launch a new fix event in EventQueue

        if(num_sysadmin > 0 && fixes.size()){
            events.addEvent(*new Fix(sim_time + Globals::randomFixTime(Globals::mt), Globals::fixes.front()));
            fixes.pop();
        }
        sim_time = events.first()->eventTime();

        events.executeEvent(sim_time);
    }

    cout << endl;

    cin.ignore();

    delete computers;
    return 0;
}

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
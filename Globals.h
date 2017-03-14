//
// Created by Victor on 2/10/2017.
//

#ifndef NETWORKSIMULATOR_GLOBALS_H
#define NETWORKSIMULATOR_GLOBALS_H

#include <queue>
#include <random>
#include <unordered_set>
#include <iomanip>


#include "Computer.h"
#include "UnionFind.h"

class EventQueue;

namespace Globals {

    extern bool fixInProgress;

    extern std::mt19937_64 mt;

    extern Computer *computers;

    extern int** edges;

    extern int** MST;

    extern int num_attackers, num_sysadmin, num_nodes;

    extern EventQueue events;

    extern std::queue<Computer *> fixes; //queue for broken computers sysadmin needs to fix - O(1) enqueue and dequeue

    extern int num_attacks;

    extern std::unordered_set<int> infected;

    extern int num_infected;

    extern bool isRebuild;

    void buildOriginalMST(int** edges);

    bool rebuildMST1(int **edges);

    bool buildAndPrintOptimal(); // Rebuild MST with original graph and Print it

    bool buildAndPrintUnOptimal();

    void printMST1();

    extern  std::uniform_int_distribution<int> randomFixTime, randomAttackTime;

}

#endif //NETWORKSIMULATOR_GLOBALS_H

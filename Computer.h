//
// Created by Victor on 2/10/2017.
//

#ifndef NETWORKSIMULATOR_COMPUTER_H
#define NETWORKSIMULATOR_COMPUTER_H

class Computer {
//operator overload for computers
public:
    int id;
    // O(1) MEMBERSHIP LOOKUP. Hashtable to check if computer is already fixpending
    bool infected = false, fixPending = false;

    Computer(int t_id);

    Computer() = default;
};

class ComputerFix {
public:
    ComputerFix(int p_time, Computer *p_computer);

    Computer *computer;
    int time;
};

#endif //NETWORKSIMULATOR_COMPUTER_H

//
// Created by Victor on 2/10/2017.
//

#ifndef NETWORKSIMULATOR_COMPUTER_H
#define NETWORKSIMULATOR_COMPUTER_H

class Computer {
//operator overload for computers
public:
    int id;
    int subnet;
    bool infected, fixPending = false;

    Computer(int t_id, int t_subnet, bool t_infected);

    Computer() = default;

    Computer &randomOther();
};

class ComputerFix {
public:
    ComputerFix(int p_time, Computer *p_computer);

    Computer *computer;
    int time;
};

#endif //NETWORKSIMULATOR_COMPUTER_H

//
// Created by Victor on 2/10/2017.
//

#include <random>

#include "Computer.h"
#include "Globals.h"

ComputerFix::ComputerFix(int p_time, Computer *p_computer) {
    time = p_time;
    computer = p_computer;
}

Computer::Computer(int t_id, int t_subnet, bool t_infected) {
    id = t_id;
    subnet = t_subnet;
    infected = t_infected;
};

Computer &Computer::randomOther() {
    int otherId;

    do {
        otherId = rand() % Globals::num_computers;
    } while (otherId != id);

    return Globals::computers[otherId];
}
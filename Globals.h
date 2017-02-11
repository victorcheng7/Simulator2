//
// Created by Victor on 2/10/2017.
//

#ifndef NETWORKSIMULATOR_GLOBALS_H
#define NETWORKSIMULATOR_GLOBALS_H

#include <queue>
#include <random>

#include "Computer.h"

class EventQueue;

namespace Globals {
    enum SimState {
        Running = 0,
        AttackerWon,
        Draw,
        SysadminWon,
    };

    extern int num_computers,
            num_infected,
            perc_success,
            perc_detect;

    extern bool fixInProgress;

    extern SimState sim_state;

    extern std::mt19937_64 mt;

    extern Computer *computers;

    extern EventQueue events;

    extern std::queue<Computer *> fixes; //queue for broken computers sysadmin needs to fix
}

#endif //NETWORKSIMULATOR_GLOBALS_H

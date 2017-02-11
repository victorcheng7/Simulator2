//
// Created by Victor on 2/10/2017.
//

#include "EventQueue.h"
#include "Globals.h"

namespace Globals {
    int num_computers,
            num_infected = 0,
            perc_success,
            perc_detect;

    bool fixInProgress = false;

    SimState sim_state = Running;

    Computer *computers;

    EventQueue events;

    std::queue<Computer *> fixes; //queue for broken computers sysadmin needs to fix
}
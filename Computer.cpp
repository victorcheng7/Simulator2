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

Computer::Computer(int t_id) {
    id = t_id;
};
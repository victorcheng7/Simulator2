#include <assert.h>

#include <iostream>
#include <list>
#include <cstdio>
#include <cstdlib>
#include<queue>
#include <ctime>

#include "Computer.h"
#include "EventQueue.h"
#include "Globals.h"

using namespace Globals;
using namespace std;

int main() {

//CHANGE THIS TO COMMAND LINE ARGUMENTS

    cout << "Enter the number of computers in simulation" << endl;
    cin >> num_computers;
    cout << "Enter chance of successful attack (0-100)" << endl;
    cin >> perc_success;
    cout << "Enter chance of detection" << endl;
    cin >> perc_detect;

    // rough gauge on optimizing how many events I should already have in my EventQueue, so I can minimize number of resizes.
    EventQueue events(num_computers);

    int time_sim = 0; // time during the simulation

    computers = new Computer[num_computers]; //List of computers in network

    // creating computers
    for (int j = 0; j < num_computers / 2; j++) computers[j] = Computer(j, 1, false);
    for (int k = num_computers / 2; k < num_computers; k++) computers[k] = Computer(k, 2, false);

    // seed rand() with the current time
    srand(static_cast<unsigned int>(time(NULL)));

    // start off simulation by infecting a random computer
    events.addEvent(*new AttackerAttack(time_sim + 11, &computers[rand() % num_computers]));

    while (!sim_state) {
        time_sim = events.array[1]->time;

        if (time_sim >= 8640000)
            sim_state = Draw;
        else
            events.executeEvent(time_sim);

//        //Infected computers and attacker ATTACK EVENTS added onto EventQueue
//        if (time_sim % 10) {
//            int computer_attack_id;
//            if (time_sim != 0) {
//                int attack_chance = rand() % 101;
//                if (attack_chance < perc_success) {
//                    //attacker infects random computer
//                    cout << "Attack( A, " << time_sim << ", " << computer_attack_id << ") SUCCESS" << endl;
//
//                    int computer_attack_id = rand() % computers;
//                    AttackerAttack attack(time_sim + 11, &computers[computer_attack_id]);
//                    events.addEvent(attack);
//                } else {
//                    cout << "Attack( A, " << time_sim << ", " << computer_attack_id << ")" << "FAILED" << endl;
//                }
//            }
//
//            //Loop through all infected computers and infect others
//            for (int i = 0; i < computers; i++) {
//                if (computers[i].infected == true) {
//                    int rollerSuccessAttack = rand() % 101;
//                    if (rollerSuccessAttack < perc_success) {
//                        computer_attack_id = rand() % computers;
//                        while (computer_attack_id == computers[i].id) {
//                            computer_attack_id = rand() % computers;
//                        }
//                        //Add attack onto Eventqueue
//                        ComputerAttack attack(time_sim + 11, &computers[i],
//                                               &computers[computer_attack_id]);
//                        events.addEvent(attack);
//                        cout << "Attack(" << time_sim << ", " << computers[i].id << ", " << computer_attack_id
//                             << ") SUCCESS" << endl;
//                    } else {
//                        cout << "Attack(" << time_sim << ", " << computers[i].id << ")" << "FAILED" << endl;
//                    }
//                }
//            }
//        }
//
//        while (time_sim == events.array[1]->time) {
//            events.executeEvent(time_sim);
//        }
//
//        if (checkFixQueue(time_sim)) {
//            Computer *tempComp = fixes.front().computer;
//            fixes.pop();
//            tempComp->infected = false;
//            cout << "Fix(" << time_sim << ", " << tempComp->id << ")" << endl;
//
//            num_fixes--;
//            num_infected--;
//        }
//
//        time_sim++;
    }

    switch (sim_state) {
        case AttackerWon:
            cout << "Attacker wins";
            break;
        case Draw:
            cout << "Draw";
            break;
        case SysadminWon:
            cout << "Sysadmin wins";
            break;
        default:
            assert(false);
            break;
    }

    cout << endl;

    delete computers;
    return 0;
}
#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <queue>
#include <random>

#include "Computer.h"
#include "EventQueue.h"
#include "Globals.h"

using namespace Globals;
using namespace std;

int main(int argc, char **argv) {
// To compile g++ -std=c++11 -o main main.cpp Globals.cpp Computer.cpp EventQueue.cpp

  random_device dev;

  mt.seed(dev());

  if (argc < 4) {
    cout << "Usage: " << argv[0]
         << " <num_computers> <perc_success> <perc_detect>" << endl;

    return 1;
  }

  num_computers = stoi(argv[1]);
  perc_success = stoi(argv[2]);
  perc_detect = stoi(argv[3]);

  int sim_time = 0; // time during the simulation

  computers = new Computer[num_computers]; // List of computers in network

  // creating computers
  for (int j = 0; j < num_computers / 2; j++)
    computers[j] = Computer(j, 1, false);
  for (int k = num_computers / 2; k < num_computers; k++)
    computers[k] = Computer(k, 2, false);

  // seed z with the current time
  srand(static_cast<unsigned int>(time(NULL)));

  // start off simulation by infecting a random computer
  events.addEvent(
      *new AttackerAttack(sim_time + 11, &computers[mt() % num_computers]));

  while (!sim_state) {
    sim_time = events.first()->eventTime();

    if (sim_time >= 8640000)
      sim_state = Draw;
    else
      events.executeEvent(sim_time);
  }


  switch (sim_state) {
  case AttackerWon: cout << "Attacker wins"; break;
  case Draw: cout << "Draw"; break;
  case SysadminWon: cout << "Sysadmin wins"; break;
  default: assert(false); break;
  }

  cout << endl;

  delete computers;
  return 0;
}
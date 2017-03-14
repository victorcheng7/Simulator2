
// Created by Victor on 2/3/2017.
//

#include <assert.h>

#include <utility>
#include <iomanip>

#include "EventQueue.h"
#include "Globals.h"
#include "UnionFind.h"

using namespace Globals;


// Event Class Superclass
Event::Event(int p_time) : time(p_time) {}

int Event::eventTime() const { return time; }


//Attack class Superclass to all types of attacks
Attack::Attack(int p_time, Computer *p_target)
    : Event(p_time), target(p_target) {}


void Attack::execute(int sim_time) {

    Globals::num_attacks++;

    print();

    if (!target->infected) {
        Globals::num_infected++;
        target->infected = true;
        Globals::infected.insert(target->id);
    }
        relaunch(sim_time + Globals::randomAttackTime(Globals::mt));

    if(num_sysadmin > 0){
        launchFix(sim_time + Globals::randomFixTime(Globals::mt), target);
        num_sysadmin--;
    }
    else{
        fixes.push(target);
    }

    if(!isRebuild){
        launchMSTRebuild(sim_time + 20);
        isRebuild = true;
    }

}


AttackerAttack::AttackerAttack(int p_time, Computer *p_target)
    : Attack(p_time, p_target) {}

void AttackerAttack::print() {
  cout << "Attack(" << eventTime() << ", ATKR, " << target->id << ") " << endl;

  cout << "Number of infected nodes: " << Globals::num_infected << endl;
}

void AttackerAttack::relaunch(int newtime) {
  Globals::events.addEvent(*new AttackerAttack(
      newtime, &Globals::computers[Globals::mt() % Globals::num_nodes]));
}
void AttackerAttack::launchFix(int p_time, Computer *p_target){
    Globals::events.addEvent(*new Fix(p_time + randomFixTime(mt), p_target));
}
void AttackerAttack::launchMSTRebuild(int p_time){
    Globals::events.addEvent(*new buildMST(p_time + 20));
}



//Fix class, used for spawning fixes in Eventqueue
Fix::Fix(int p_time, Computer *p_target) : Event(p_time), target(p_target) {}

void Fix::execute(int sim_time) {
  cout << "Fix(" << sim_time << ", " << target->id << ")" << endl;

    if(target->infected){
        target -> infected = false;
        --Globals::num_infected;
        num_sysadmin++;
        infected.erase(target->id);
    }
    target->fixPending = false;

    if(!isRebuild){
        launchMSTRebuild(sim_time + 20);
        isRebuild = true;
    }

}
void Fix::launchMSTRebuild(int p_time){
    Globals::events.addEvent(*new buildMST(p_time+20));
}





buildMST::buildMST(int p_time) : Event(p_time){}

void buildMST::execute(int sim_time){
    cout << "Rebuild_spanning_tree(" << sim_time << ")" << endl;

    //Check if the graph is partitioned, schedule a fix event.

    bool check1 = buildAndPrintUnOptimal();
    bool check2 = buildAndPrintOptimal();

    if(check2) {
        printMST1();
    }
    isRebuild = false;

}



//EventQueue implementation



EventQueue::EventQueue(int initial_size) {
  size_array = 0;
  max_size = initial_size;

  array = new Event *[initial_size + 1];
  array[0] = nullptr;
};

EventQueue::EventQueue() : EventQueue(4) {}

EventQueue::~EventQueue() {
  for (int i = 0; i < size_array; i++) delete array[i];
  delete[] array;
}

int EventQueue::size() const { return size_array; }

Event *EventQueue::first() const {
  //assert(size_array > 0);
  return array[1];
}

void EventQueue::resize(int size_array) {
  max_size *= 2;

  Event **resize_arr = new Event *[max_size + 1];
  resize_arr[0] = nullptr;
  for (int i = 1; i <= size_array; i++) resize_arr[i] = array[i];

  delete[] array;
  array = resize_arr;
}

void EventQueue::addEvent(Event &event) {
  if (size_array == max_size) resize(max_size);

  ++size_array;

  array[size_array] = &event;

  percolateUp(size_array);
}

void EventQueue::executeEvent(int sim_time) {
  //assert(size_array > 0);

  // execute top node of the array, delete top node, subtract 1 size from heap,
  // remake the heap so it maintains heap structure
  array[1]->execute(sim_time);
  delete array[1];
  std::swap(array[1], array[size_array]);

  --size_array;

  percolateDown(1);
}

// not going to be used
void EventQueue::incrementKey(int index, unsigned int deltaTime) {
  // time is greater, so percolateDown
  array[index]->time += deltaTime;
  percolateDown(index);
}

void EventQueue::decrementKey(int index, unsigned int deltaTime) {
  // time is lesser, so percolateUp
  array[index]->time -= deltaTime;
  percolateUp(index);
}

void EventQueue::percolateUp(int index) {
  //assert(size_array >= 0);

  if (!size_array) return;

  //assert(index >= 1);

  while (true) {
    if (index / 2 < 1) break;

    if (array[index]->eventTime() < array[index / 2]->eventTime())
      std::swap(array[index], array[index / 2]);
    else
      break;

    index /= 2;
  }
}

void EventQueue::percolateDown(int index) {
  //assert(size_array >= 0);

  if (!size_array) return;

  //assert(index <= size_array);

  while (true) {
    int child = index * 2;

    if (child > size_array) break;

    if (child + 1 <= size_array &&
        array[child + 1]->eventTime() < array[child]->eventTime())
      ++child;

    if (array[child]->eventTime() < array[index]->eventTime())
      std::swap(array[index], array[child]);

    index = child;
  }
}

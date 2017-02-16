//
// Created by Victor on 2/3/2017.
//

#include <assert.h>

#include <utility>

#include "EventQueue.h"
#include "Globals.h"

Event::Event(int p_time) : time(p_time) {}

int Event::eventTime() const { return time; }

Attack::Attack(int p_time, Computer *p_target)
    : Event(p_time), target(p_target) {}

bool Attack::canAttack() { return true; }

bool Attack::canDetect() { return true; }

void Attack::notify(int newtime) {
  Globals::events.addEvent(*new Notify(newtime, target));
}

void Attack::execute(int sim_time) {
  if (!canAttack()) return;

  bool success = Globals::mt() % 100 < Globals::perc_success,
       detect = Globals::mt() % 100 < Globals::perc_detect;

  print(success, detect);

  if (success) {
    if (!target->infected) {
      target->infected = true;
      ++Globals::num_infected;

      Globals::events.addEvent(
          *new ComputerAttack(sim_time + 10, target, &target->randomOther()));

      if (Globals::num_infected >= Globals::num_computers / 2)
        Globals::sim_state = Globals::AttackerWon;
    }
  }

  if (detect) notify(sim_time + 1);

  relaunch(eventTime() + 10);
}

ComputerAttack::ComputerAttack(int p_time, Computer *p_source,
                               Computer *p_target)
    : Attack(p_time, p_target), source(p_source) {}

bool ComputerAttack::canAttack() { return source->infected; }

bool ComputerAttack::canDetect() { return source->subnet != target->subnet; }

void ComputerAttack::notify(int newtime) {
  Globals::events.addEvent(*new Notify(newtime, source, target));
}

void ComputerAttack::print(bool success, bool detect) {
  cout << "Attack(" << eventTime() << ", " << source->id << ", " << target->id
       << ") ";

  if (success)
    cout << "SUCCESS";
  else
    cout << "FAIL   ";

  if (detect)
    cout << "DETECT";
  else
    cout << "      ";

  cout << " " << Globals::num_infected << endl;
}

void ComputerAttack::relaunch(int newtime) {
  Globals::events.addEvent(
      *new ComputerAttack(newtime, source, &source->randomOther()));
}

AttackerAttack::AttackerAttack(int p_time, Computer *p_target)
    : Attack(p_time, p_target) {}

void AttackerAttack::print(bool success, bool detect) {
  cout << "Attack(" << eventTime() << ", ATKR, " << target->id << ") ";

  if (success)
    cout << "SUCCESS";
  else
    cout << "FAIL   ";

  if (detect)
    cout << "DETECT";
  else
    cout << "      ";

  cout << " " << Globals::num_infected << endl;
}

void AttackerAttack::relaunch(int newtime) {
  Globals::events.addEvent(*new AttackerAttack(
      newtime, &Globals::computers[Globals::mt() % Globals::num_computers]));
}

Fix::Fix(int p_time, Computer *p_target) : Event(p_time), target(p_target) {}

void Fix::execute(int sim_time) {
  cout << "Fix(" << sim_time << ", " << target->id << ")" << endl;

  if (target->infected) {
    target->infected = false;
    --Globals::num_infected;

    //assert(Globals::num_infected >= 0);
    if (Globals::num_infected == 0) Globals::sim_state = Globals::SysadminWon;
  }

  target->fixPending = false;

  if (Globals::fixes.size()) {
    Globals::events.addEvent(*new Fix(sim_time + 100, Globals::fixes.front()));
    Globals::fixes.pop();
  } else
    Globals::fixInProgress = false;
}

Notify::Notify(int p_time, Computer *p_source, Computer *p_target)
    : Event(p_time) {
  source = p_source;
  target = p_target;
}

Notify::Notify(int p_time, Computer *p_target)
    : Notify(p_time, nullptr, p_target) {}

void Notify::execute(int sim_time) {
  cout << "Notify(" << sim_time << ", ";

  if (source != nullptr) {
    cout << source->id << ", ";

    if (!source->fixPending) {
      Globals::fixes.push(source);
      source->fixPending = true;
    }
  }

  cout << target->id << ")" << endl;

  if (!target->fixPending) {
    Globals::fixes.push(target);
    target->fixPending = true;
  }

  if (!Globals::fixInProgress) {
    Globals::events.addEvent(*new Fix(sim_time + 101, Globals::fixes.front()));
    Globals::fixes.pop();

    Globals::fixInProgress = true;
  }
}

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

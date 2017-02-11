//
// Created by Victor on 2/3/2017.
//

#ifndef NETWORKSIMULATOR_EVENTQUEUE_H
#define NETWORKSIMULATOR_EVENTQUEUE_H

#include <iostream>
#include<string>
#include<queue>
#include<list>

#include "Computer.h"

using namespace std;

//Below is the Computer class that represents a computer, Computer_fix have a computer and a time to be fixed

//Below are all possible events that can be added to the Priority Queue
class Event {
    int time;

public:
    Event(int p_time);

    Event() = default;

    int eventTime() const;

    virtual void execute(int sim_time) = 0;

    friend class EventQueue;
};

class Attack : public Event {
protected:
    virtual bool canAttack();

    virtual bool canDetect();

    virtual void notify(int newtime);

    virtual void print(bool success, bool detect) = 0;

    virtual void relaunch(int newtime) = 0;

public:
    Computer *target;

    Attack(int p_time, Computer *p_target);

    virtual void execute(int sim_time) override;
};

class ComputerAttack : public Attack {
protected:
    virtual bool canAttack() override;

    virtual bool canDetect() override;

    virtual void notify(int newtime) override;

    virtual void print(bool success, bool detect) override;

    virtual void relaunch(int newtime) override;
public:
    ComputerAttack(int p_time, Computer *p_source, Computer *p_target);

    Computer *source;
};

class AttackerAttack : public Attack {
    virtual void print(bool success, bool detect) override;

    virtual void relaunch(int newtime) override;
public:
    AttackerAttack(int p_time, Computer *p_target);
};

class Fix : public Event {
public:
    Computer *target;

    Fix(int p_time, Computer *p_target);

    virtual void execute(int sim_time) override;
};

class Notify : public Event {
public:
    Computer *source, *target;

    Notify(int p_time, Computer *p_source, Computer *p_target);

    Notify(int p_time, Computer *p_target);

    virtual void execute(int sim_time) override;
};

class EventQueue {
    Event **array;
    int max_size;
    int size_array;

public:
    EventQueue(int initial_size);

    EventQueue();

    ~EventQueue();

    int size() const;

    Event *first() const;

    void percolateUp(int index);

    void percolateDown(int index);

    void incrementKey(int index, unsigned int deltaTime);

    void decrementKey(int index, unsigned int deltaTime);

    void addEvent(Event &event);

    void executeEvent(int sim_time);

    void resize(int size_array);
};

#endif //NETWORKSIMULATOR_EVENTQUEUE_H
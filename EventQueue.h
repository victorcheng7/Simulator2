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

    virtual void print() = 0;

    virtual void launchFix(int p_time, Computer* p_target) = 0;

    virtual void relaunch(int newtime) = 0;

    virtual void launchMSTRebuild(int p_time) = 0;


public:
    Computer *target;

    Attack(int p_time, Computer *p_target);

    virtual void execute(int sim_time) override;


};



class AttackerAttack : public Attack {
    virtual void print() override;

    virtual void relaunch(int newtime) override;
    virtual void launchFix(int p_time, Computer* p_target) override;
    virtual void launchMSTRebuild(int p_time);
public:
    AttackerAttack(int p_time, Computer *p_target);
};



class Fix : public Event {
public:
    Computer *target;

    Fix(int p_time, Computer *p_target);

    virtual void execute(int sim_time) override;

    void launchMSTRebuild(int p_time);

};


class buildMST : public Event {

public:
    buildMST(int p_time);

    virtual void execute(int sim_time) override;

};

//EventQueue

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
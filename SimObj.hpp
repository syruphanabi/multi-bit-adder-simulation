//
//  SimObj.hpp
//  6730proj2
//
//  Created by cheng on 18/3/27.
//  Copyright © 2018年 cheng. All rights reserved.
//

#ifndef SimObj_hpp
#define SimObj_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <atomic>
#include "Event.hpp"

using namespace std;

class SimObj{

public:

    //SimObj
    int idCntr = 0;
    long MIN_DELAY = 1l;
    int id;
    int numInputs;
    int numOutputs;
    vector<queue<Event>> inputEvents;
    vector<long> inputTimes;
    long clock = 0;
    queue<Event> readyEvents;
    bool isOnWl;
    bool active;

    vector<atomic_bool> inputEventLock;
    atomic_int numLockedInput;
    bool isRunning;


    SimObj();
    SimObj(int ni, int no);
    void reset(int a, int b);

    //important function:
    void computeClock();//compute local time
    void populateReadyEvents(int epi, bool sepInputLock);//process current msg

    //others
    bool isActive();
    void updateActive();
    bool onwlFlag();
    bool tryLockAllInput();
    void releaseAllInputLock();
    bool hasLockedInput();
    int getId();
    int getIdcntr();

};

#endif /* SimObj_hpp */

//
//  GNode.hpp
//  6730proj2
//
//  Created by cheng on 18/3/27.
//  Copyright © 2018年 cheng. All rights reserved.
//

#ifndef GNode_hpp
#define GNode_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <atomic>
#include <iostream>
#include <unordered_map>
#include "Event.hpp"
#include "SimObj.hpp"


using namespace std;

class GNode{

public:

    vector<GNode> lockList;

    string type;
    int nodeID;
    int graphID;
    SimObj data;
    vector<GNode*> inNeigh;
    vector<GNode*> outNeigh;
    atomic_bool casLock;
    long MIN_DELAY = 1l;

    vector<atomic_int> input;
    atomic_int output;

    GNode();
    GNode(int ID);
    GNode(int ID, int gID);
    GNode(SimObj d, int ID);
    SimObj* getData();
    SimObj setData(SimObj d);
    void setType(string type);
    void reset();
    int getNodeID();
    int getInputIndex(int id);
    vector<GNode*> getOutNeigh();

    //important function
    //handle event
    int simulate(GNode& myNode, int epi, bool sepInputLock);//one bit one timestep
    Event makeEvent(GNode& recvObj, string type, int msg, long sendTime, long delay);
    void execEvent(GNode& myNode, Event& e);
    void recvEvent(int in, Event& e);
};

#endif /* GNode_hpp */

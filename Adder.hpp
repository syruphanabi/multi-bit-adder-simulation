#ifndef Adder_hpp
#define Adder_hpp

#include <iostream>
#include "Graph.hpp"
#include "GNode.hpp"
#include "SimObj.hpp"
#include "Event.hpp"
#include <fstream>
#include <bitset>
#include <vector>
#include <queue>
#include <climits>

class Adder{

public:
  // A, B and C0
  vector<queue<Event*>*> queue_collection1;
  vector<string> queue1_name;
  // C1, C2 ... Ck
  vector<queue<Event*>*> queue_collection2;
  vector<string> queue2_name;
  // S0, S1, .. Sk-1
  vector<queue<Event*>*> queue_collection3;
  vector<string> queue3_name;
  // vector storing all graphs
  vector<Graph*> graph_collection;
  vector<string> graph_name;
  //current result in binary
  vector<int> result;

  //build a multi-bit adder
  Adder(int k);
  //convert calculation result from binary number to decimal number, and print out
  void Read(int a_input, int b_input, long receive_time, int k);
  //covert decimal number to binary number, and push to multi-bit adder
  void finalOutput(vector<int> true_result);
};

#endif

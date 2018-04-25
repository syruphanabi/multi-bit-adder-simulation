#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>

#include "GNode.hpp"

using namespace std;

class Graph{

public:
	  int graphID;
    vector<GNode*> nodes;
    unordered_map<int, GNode&> map;

		queue<Event*> *inputA;
		queue<Event*> *inputB;
		queue<Event*> *inputC;
		queue<Event*> *outputC;
		queue<Event*> *outputS;

		queue<Event*> readyOutC;
		queue<Event*> readyOutS;

		Graph(int ID, queue<Event*> *ia, queue<Event*> *ib, queue<Event*> *ic, queue<Event*> *oc, queue<Event*> *os);
		void set();
		void GraphMaker();
		void Receive();
		void SendOut(int out, long time, string type, char k);
		void Excute(int epi);
};

#endif /* Graph_hpp */

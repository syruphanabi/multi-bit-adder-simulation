#include "Graph.hpp"
#include "Event.hpp"
#include <iostream>

using namespace std;

Graph::Graph(int ID, queue<Event*> *ia, queue<Event*> *ib, queue<Event*> *ic, queue<Event*> *oc, queue<Event*> *os):graphID(ID) {

	//initialize and push all GNodes in to the nodes
	GraphMaker();
	//init
	set();

	inputA = ia;
	inputB = ib;
	inputC = ic;
	outputC = oc;
	outputS = os;
}

void Graph::Excute(int epi){

	queue<GNode*> worklist;
	int val13 = -1, val14 = -1;
	long clock13 = -1, clock14 = -1;

	for (int a = 0; a < 3; a++){
		for (auto node: nodes){
			worklist.push(node);
		}
	}

	while(!worklist.empty()){
		worklist.front()->simulate(*worklist.front(), epi, false);
		for(auto &itr : worklist.front()->getOutNeigh()){
			worklist.push(itr);
		}

		if(worklist.front()->nodeID == 13 && worklist.front()->data.clock != clock13){
      clock13 = worklist.front()->data.clock;
      if(val13 == worklist.front()->output){
          SendOut(-1, 0, "NULL", 'S');
      }
      else {
          val13 = worklist.front()->output;
          SendOut(worklist.front()->output, worklist.front()->data.clock, "REGULAR", 'S');
      }

    } else if(worklist.front()->nodeID == 14 && worklist.front()->data.clock != clock14){
      clock14 = worklist.front()->data.clock;
      if(val14 == worklist.front()->output){
          SendOut(-1, 0, "NULL", 'C');
      }
      else {
          val14 = worklist.front()->output;
          SendOut(worklist.front()->output, worklist.front()->data.clock, "REGULAR", 'C');
      }
    }

		worklist.pop();
	}

	for(int i = 0; i < readyOutS.size(); i++){
    outputS->push(readyOutS.front());
		readyOutS.pop();
  }
  for(int i = 0; i < readyOutC.size(); i++){
    outputC->push(readyOutC.front());
		readyOutC.pop();
  }

  // for (auto node: nodes){
	// 	if(node->nodeID == 2){
	// 		cout << "node " << node->nodeID << " is at time " << node->data.clock << " with value " << node->output << endl;
	// 	}
  // }
}

void Graph::SendOut(int out, long time, string type, char k){
	Event* e = new Event(type,out);
	e->whichIn = 0;
	e->recvTime = time;
	if(k == 'C'){
		readyOutC.push(e);
	}
	else if(k == 'S'){
		readyOutS.push(e);
	}
}

void Graph::Receive() {
 while(!(*inputA).empty()) {
  Event *a = (*inputA).front();
	a->whichIn = 0;
  nodes[0]->data.inputEvents[0].push(*a);
  (*inputA).pop();
 }
 while(!(*inputB).empty()) {
  Event* b = (*inputB).front();
	b->whichIn = 0;
  nodes[1]->data.inputEvents[0].push(*b);
  (*inputB).pop();
 }
 while(!(*inputC).empty()) {
  Event* c = (*inputC).front();
	c->whichIn = 0;
  nodes[2]->data.inputEvents[0].push(*c);
  (*inputC).pop();
 }

 // for(int i = 0; i < 3; i ++){
 //  if (nodes[i]->data.inputEvents[0].empty()){
 // 	 cout << "channel" << i << " is empty." << endl;
 //  }else{
 // 	 cout << "channel " << i << " recv at " << nodes[i]->data.inputEvents[0].front().recvTime << endl;
 //  }
 // }
}

void Graph::set(){

	for(int i = 0; i < 3; i++){
		nodes[i]->data.numInputs = 1;
		nodes[i]->data.numOutputs = nodes[i]->outNeigh.size();

    nodes[i]->data.inputTimes.push_back(0);
    queue<Event> emp;
    nodes[i]->input[0].exchange(0);
    nodes[i]->data.inputEvents.push_back(emp);
	}

	for(int i = 3; i < nodes.size(); i++){
		nodes[i]->data.numInputs = nodes[i]->inNeigh.size();
		nodes[i]->data.numOutputs = nodes[i]->outNeigh.size();

		for(int j = 0; j < nodes[i]->data.numInputs; j++){
			nodes[i]->data.inputTimes.push_back(0);
      queue<Event> emp;
      nodes[i]->data.inputEvents.push_back(emp);
		}
	}
}

void Graph::GraphMaker(){
	GNode* a0 = new GNode(0, graphID);
	GNode* a1 = new GNode(1, graphID);
	GNode* a2 = new GNode(2, graphID);
	GNode* a3 = new GNode(3, graphID);
	GNode* a4 = new GNode(4, graphID);
	GNode* a5 = new GNode(5, graphID);
	GNode* a6 = new GNode(6, graphID);
	GNode* a7 = new GNode(7, graphID);
	GNode* a8 = new GNode(8, graphID);
	GNode* a9 = new GNode(9, graphID);
	GNode* a10 = new GNode(10, graphID);
	GNode* a11 = new GNode(11, graphID);
	GNode* a12 = new GNode(12, graphID);
	GNode* a13 = new GNode(13, graphID);
	GNode* a14 = new GNode(14, graphID);
	nodes.push_back(a0);
	nodes.push_back(a1);
	nodes.push_back(a2);
	nodes.push_back(a3);
	nodes.push_back(a4);
	nodes.push_back(a5);
	nodes.push_back(a6);
	nodes.push_back(a7);
	nodes.push_back(a8);
	nodes.push_back(a9);
	nodes.push_back(a10);
	nodes.push_back(a11);
	nodes.push_back(a12);
	nodes.push_back(a13);
	nodes.push_back(a14);

 	// A(i)
 	nodes[0]->outNeigh.push_back(nodes[3]);
 	nodes[0]->outNeigh.push_back(nodes[9]);
 	nodes[0]->outNeigh.push_back(nodes[10]);
 	nodes[0]->outNeigh.push_back(nodes[11]);
 	nodes[0]->outNeigh.push_back(nodes[12]);

 	nodes[3]->inNeigh.push_back(nodes[0]);
 	nodes[9]->inNeigh.push_back(nodes[0]);
 	nodes[10]->inNeigh.push_back(nodes[0]);
 	nodes[11]->inNeigh.push_back(nodes[0]);
 	nodes[12]->inNeigh.push_back(nodes[0]);

 	// B(i)
 	nodes[1]->outNeigh.push_back(nodes[4]);
 	nodes[1]->outNeigh.push_back(nodes[7]);
 	nodes[1]->outNeigh.push_back(nodes[8]);
 	nodes[1]->outNeigh.push_back(nodes[11]);
 	nodes[1]->outNeigh.push_back(nodes[12]);

 	nodes[4]->inNeigh.push_back(nodes[1]);
 	nodes[7]->inNeigh.push_back(nodes[1]);
 	nodes[8]->inNeigh.push_back(nodes[1]);
 	nodes[11]->inNeigh.push_back(nodes[1]);
 	nodes[12]->inNeigh.push_back(nodes[1]);
 	// C(i)
 	nodes[2]->outNeigh.push_back(nodes[5]);
 	nodes[2]->outNeigh.push_back(nodes[6]);
 	nodes[2]->outNeigh.push_back(nodes[8]);
 	nodes[2]->outNeigh.push_back(nodes[10]);
 	nodes[2]->outNeigh.push_back(nodes[12]);

 	nodes[5]->inNeigh.push_back(nodes[2]);
 	nodes[6]->inNeigh.push_back(nodes[2]);
 	nodes[8]->inNeigh.push_back(nodes[2]);
 	nodes[10]->inNeigh.push_back(nodes[2]);
 	nodes[12]->inNeigh.push_back(nodes[2]);

 	nodes[0]->type = "PORT";
 	nodes[1]->type = "PORT";
 	nodes[2]->type = "PORT";

 	// NOT A(i)
 	nodes[3]->type = "NOT";
 	nodes[3]->outNeigh.push_back(nodes[6]);
 	nodes[3]->outNeigh.push_back(nodes[7]);
 	nodes[3]->outNeigh.push_back(nodes[8]);

 	nodes[6]->inNeigh.push_back(nodes[3]);
 	nodes[7]->inNeigh.push_back(nodes[3]);
 	nodes[8]->inNeigh.push_back(nodes[3]);

 	// NOT B(i)
 	nodes[4]->type = "NOT";
 	nodes[4]->outNeigh.push_back(nodes[6]);
 	nodes[4]->outNeigh.push_back(nodes[9]);
 	nodes[4]->outNeigh.push_back(nodes[10]);

 	nodes[6]->inNeigh.push_back(nodes[4]);
 	nodes[9]->inNeigh.push_back(nodes[4]);
 	nodes[10]->inNeigh.push_back(nodes[4]);

 	// NOT C(i)
 	nodes[5]->type = "NOT";
 	nodes[5]->outNeigh.push_back(nodes[7]);
 	nodes[5]->outNeigh.push_back(nodes[9]);
 	nodes[5]->outNeigh.push_back(nodes[11]);

 	nodes[7]->inNeigh.push_back(nodes[5]);
 	nodes[9]->inNeigh.push_back(nodes[5]);
 	nodes[11]->inNeigh.push_back(nodes[5]);

 	// 1st AND GNode
 	nodes[6]->type = "AND";
 	nodes[6]->outNeigh.push_back(nodes[13]);
 	nodes[13]->inNeigh.push_back(nodes[6]);
 	// 2nd AND GNode
 	nodes[7]->type = "AND";
 	nodes[7]->outNeigh.push_back(nodes[13]);
 	nodes[13]->inNeigh.push_back(nodes[7]);
 	// 3rd AND GNode
 	nodes[8]->type = "AND";
 	nodes[8]->outNeigh.push_back(nodes[14]);
 	nodes[14]->inNeigh.push_back(nodes[8]);
 	// 4th AND GNode
	 nodes[9]->type = "AND";
	 nodes[9]->outNeigh.push_back(nodes[13]);
	 nodes[13]->inNeigh.push_back(nodes[9]);
	 // 5th AND GNode
	 nodes[10]->type = "AND";
	 nodes[10]->outNeigh.push_back(nodes[14]);
	 nodes[14]->inNeigh.push_back(nodes[10]);
	 // 6th AND GNode
	 nodes[11]->type = "AND";
	 nodes[11]->outNeigh.push_back(nodes[14]);
	 nodes[14]->inNeigh.push_back(nodes[11]);
	 // 7th AND GNode
	 nodes[12]->type = "AND";
	 nodes[12]->outNeigh.push_back(nodes[13]);
	 nodes[13]->inNeigh.push_back(nodes[12]);
	 nodes[12]->outNeigh.push_back(nodes[14]);
	 nodes[14]->inNeigh.push_back(nodes[12]);

	 // S(i) OR
	 nodes[13]->type = "OR";

	 // C(i+1) OR
	 nodes[14]->type = "OR";
}

#include <climits>
#include <atomic>
#include "GNode.hpp"
using namespace std;

//constructor
GNode::GNode(): data(8,8), input(8){
}

GNode::GNode(int ID) : nodeID(ID), input(8){
}

GNode::GNode(int ID, int gID) : nodeID(ID), graphID(gID), input(8){
}

GNode::GNode(SimObj d, int ID) : nodeID(ID), input(8){
}




SimObj* GNode::getData(){
	return &data;
}

void GNode::reset(){
	lockList.clear();
	casLock.exchange(false);
	data.reset(0, 0);
}

int GNode::getNodeID() { return nodeID;}

//find all output port
vector<GNode*> GNode::getOutNeigh(){
	return outNeigh;
}

void GNode::setType(string t){
	type = t;
}

//find input
int GNode::getInputIndex(int id){

	for(int i = 0; i < inNeigh.size(); i++){
		if(inNeigh[i]->nodeID == id){
			return i;
		}
	}
	return -1;
}




//simulate
int GNode::simulate(GNode& myNode, int epi, bool sepInputLock){
		myNode.data.computeClock();// update the clock, can do at the end if null msgs are propagated initially

    myNode.data.populateReadyEvents(epi, sepInputLock); // fill up readyEvents,

    int retVal = this->data.readyEvents.size();
    while (!this->data.readyEvents.empty()) {
    	Event e = this->data.readyEvents.front();
    	this->data.readyEvents.pop();

    	// should already own a lock
      execEvent(myNode, e);
    }

    return retVal;
}

//create new event for outNeigh
Event GNode::makeEvent(GNode& recvObj, string label, int msg, long sendTime, long delay){
	Event e(label, msg);
	if(delay <= 0){
		delay = this->MIN_DELAY;
	}
	long recvTime;
	if(sendTime == LONG_MAX){
		recvTime = LONG_MAX;
	}
	else {
		recvTime = sendTime + delay;
	}

	e.recvTime = recvTime;
	e.sendTime = sendTime;
	e.type = label;
	e.val = msg;

    return e;
}

void GNode::execEvent(GNode& sendNode, Event& e){
	int ans = INT_MAX;
	string t;

	//compute output based on node type
  if(e.type == "NULL"){
		cout << "null" << endl;
		for(auto &itr: outNeigh){
				Event ne = makeEvent(*itr, "NULL", output.load(), e.getRecvTime(), MIN_DELAY);
				int in = itr->getInputIndex(nodeID);
				itr->recvEvent(in, ne);
		}
	}else{
		t = "REGULAR";
		input[e.whichIn].exchange(e.val);
		if(this->type == "AND"){
			ans = 0;
    	for(auto &&itr : input){ans += (int)itr.load();}
    	if(ans == data.numInputs) output.exchange(1);
    	else output.exchange(0);
		}
  	else if(this->type == "OR"){
      	ans = 0;
      	for(auto &&itr : input){ans += itr.load();}
      	if(ans >= 1) output.exchange(1);
      	else output.exchange(0);
  	}
  	else if(this->type == "NOT"){
      	ans = input[0].load();
      	if(ans == 1) output.exchange(0);
      	else output.exchange(1);
  	}
    else if(this->type == "PORT"){
        ans = input[0].load();
        output.exchange(ans);
    }

    //makeEvent and fanout
    for(auto &itr: outNeigh){
        Event ne = makeEvent(*itr, t, output.load(), e.getRecvTime(), MIN_DELAY);
        int in = itr->getInputIndex(nodeID);
        itr->recvEvent(in, ne);

    }
  }
}

void GNode::recvEvent(int in, Event& e){
	if(!(in >= 0 && in < data.inputEvents.size() && e.recvTime >= this->data.inputTimes[in])){

    // if(in == -1) cout << "ERROR: GNode not found"<<endl;
		// cout << "ERROR: "<< "inputTimes = " << this->data.inputTimes[in] << endl;
	}
  else{
    e.whichIn = in;

		if(e.type != "NULL"){
			if(!this->data.inputEvents[in].empty()){
				while(this->data.inputEvents[in].front().type == "NULL"){
					this->data.inputEvents[in].pop();
				}
			}
		}
    this->data.inputEvents[in].push(e);
		this->data.inputTimes[in] = this->data.inputEvents[in].front().recvTime;
	}
}

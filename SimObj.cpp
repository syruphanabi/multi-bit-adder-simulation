#include <climits>
#include <iostream>
#include "SimObj.hpp"


SimObj::SimObj() {
    inputTimes.reserve(4);
    inputEvents.reserve(4);
}

SimObj::SimObj(int ni, int no): inputEvents(4), inputTimes(4){
}

void SimObj::reset(int numInputs, int numOutputs){
	this->inputTimes.clear();
	inputEvents.clear();
    inputEventLock.clear();

    queue<Event> empty;
	readyEvents.swap(empty);

	this->numLockedInput.exchange(0);
	this->isRunning = false;
	this->isOnWl = false;

}




//check current clock time
void SimObj::computeClock(){
	long min = LONG_MAX;
  int updateFlag = 1;
    for (int i = 0; i < numInputs; ++i) {
      if(!inputEvents[i].empty()) {
        this->inputTimes[i] = inputEvents[i].front().recvTime;
      }
      if(this->inputTimes[i] == LONG_MAX) {
        cout << "TIME NOT EXIST ERROR" << endl;
        updateFlag = 0;
        break;
      }
      if (min > this->inputTimes[i]) {
        min = this->inputTimes[i];
      }
    }
    if(updateFlag){
      this->clock = min;
    }
}

bool SimObj::isActive(){
	return active;
}

//check if node still active
void SimObj::updateActive(){

    active = false;
    for(int i = 0; i < numInputs; i++){
        if(inputEvents[i].empty() || inputEvents[i].front().type != "NULL"){
            active = true;
        }
    }
}

//pull all events that is less than current clock time
void SimObj::populateReadyEvents(int epi, bool sepInputLock){
	long min;
    long recvTime;
    int minIdx = 0;
    int count = 0;
    bool addEvent = true;

    while(count < epi && addEvent) {
        min = LONG_MAX;
        addEvent = false;
        for(int idx = 0; idx < numInputs; ++idx) {
            if(!inputEvents[idx].empty()) {
                recvTime = inputEvents[idx].front().getRecvTime();
              if(recvTime <= this->clock && recvTime <= min) {

                    min = recvTime;
                    minIdx = idx;
                    addEvent = true;
                }
            }
        }

        if(addEvent) {
            this->readyEvents.push(inputEvents[minIdx].front());
            inputEvents[minIdx].pop();
            if(!inputEvents[minIdx].empty()){
              inputTimes[minIdx] = inputEvents[minIdx].front().recvTime;
            }
            ++count;
        }
    }

    if(sepInputLock)
      this->releaseAllInputLock();
}




bool SimObj::onwlFlag(){
	return this->isOnWl;
}
//not used
bool SimObj::tryLockAllInput(){
    bool ret = true;
    int i, failIdx = 0;

    for(i = 0; i < numInputs; ++i) {
        ret = inputEventLock[i].exchange(true);
        if(ret) {
            failIdx = i;
            break;
        }
    }

    if(!ret) {
        for(i = 0; i < failIdx; ++i)
            inputEventLock[i].exchange(false);
    }else
		numLockedInput.exchange(numInputs);

    return ret;
}
//not used
void SimObj::releaseAllInputLock(){
	for(int i = 0; i < numInputs; ++i){
        inputEventLock[i].exchange(false);
	}

	numLockedInput.exchange(0);
}

bool SimObj::hasLockedInput(){
	return (numLockedInput.load() > 0);
}

int SimObj::getId(){
	return id;
}

int SimObj::getIdcntr(){
	return idCntr;
}

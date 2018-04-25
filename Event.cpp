#include "Event.hpp"

Event::Event(string tp, int v):type(tp), val(v){}

long Event::getRecvTime(){
	return recvTime;
}

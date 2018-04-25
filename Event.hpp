#ifndef Event_hpp
#define Event_hpp

#include <string>
#include <stdio.h>
#include <atomic>

using namespace std;

class Event{

public:
  Event(string tp, int v);
  string type;

  int whichIn;
  long recvTime;
  long sendTime;
  int val;
  long getRecvTime();
};

#endif /* Event_hpp */

Infrastructure:
  In Makefile you can see, this simulation program is written in C++11.
  You should compiler it with g++ and -std=c++11.
  If you want to run in parallel, you should have OpenMP and use -fopenmp.
  If not, you can delete OpenMP flags in main.cpp at line 48 and 87,
  and change OMPFLAGS to empty in Makefile before make.


How to use:

  calculate mode takes 5 arguments, otherwise, you are using statistical mode, which take 4 arguments.

  For example:

    command:
      make
      ./out 8 127 12 200
    stdout and file-out:
      ...
      197,139,139
      198,139,139
      199,139,139
      200,139,0
      201,139,-1073741824
      calculate mode: 127 + 12
      time cost: 0.290214

    this means you want to calculate 127+12 on an 8-bit adder,
    and observe the result for 200 time steps.
    at time step 197, you got 139, and your true value is 139.
    at time step 200, you got 139, and your true value is 0, because your calculation already stoped.

    command:
      make
      ./out 8 200 20
    stdout and file-out:
      198,99,99
      199,99,99
      199,99,99
      200,99,0
      200,99,0
      201,99,-1610612736
      statistical mode:
      time cost: 0.155278

    this means you want to observe an 8-bit adder for 200 time steps,
    while change your a and b for a+b to a new pair of random number every 20 time steps.


How to understand code:

  class Event: the struct of msgs in the system.

  class GNode: mainly describe how a logical gate connect with other logical gates.

      int simulate(GNode& myNode, int epi, bool sepInputLock);//one bit one timestep
      Event makeEvent(GNode& recvObj, string type, int msg, long sendTime, long delay);
      void execEvent(GNode& myNode, Event& e);
      void recvEvent(int in, Event& e);

  class SimObj: mainly describe how to deal with data in a logical gate with time controlling.

      void computeClock();//compute local time
      void populateReadyEvents(int epi, bool sepInputLock);//process current msg

  class Graph: describe how to build a 1-bit adder with logical gates, and how to run a 1-bit adder.

      void GraphMaker();
      void set();
      void Receive();
      void SendOut(int out, long time, string type, char k);
      void Excute(int epi);

  class Adder: describe how to build a multi-bit adder with 1-bit adder, and how to take in and print out decimal input and result.

      Adder(int k); //build a multi-bit adder
      void Read(int a_input, int b_input, long receive_time, int k); //convert calculation result from binary number to decimal number, and print out
      void finalOutput(vector<int> true_result); //covert decimal number to binary number, and push to multi-bit adder

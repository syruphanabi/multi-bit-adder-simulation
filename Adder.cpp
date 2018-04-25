#include "Adder.hpp"
#include <climits>

//build a multi-bit adder
Adder::Adder(int k){
  vector<int> tmp(k, -1);
  result = tmp;
  // for(int i = 0; i < k; i++){
  //   result[i] = -1;
  // }

  //A0,A1,...B0,B1,...,C0
  for(int i = 0; i < 2*k+1; i++) {
    queue<Event*> *q = new queue<Event*>;
    queue_collection1.push_back(q);
    if(i >= 0 && i<k) queue1_name.push_back("A_"+to_string(i));
    else if(i >= k && i < 2*k) queue1_name.push_back("B_"+to_string(i%k));
    else queue1_name.push_back("C_0");
  }
  //C1,C2,...Ck
  for(int i = 0; i < k; i++) {
    queue<Event*> *q = new queue<Event*>;
    queue_collection2.push_back(q);
    int index = i+1;
    queue2_name.push_back("C_"+to_string(index));
  }
  //S0,S1,...,Sk-1
  for(int i = 0; i < k; i++) {
    queue<Event*> *q = new queue<Event*>;
    queue_collection3.push_back(q);
    queue3_name.push_back("S_"+to_string(i));
  }

  //Graph0,Graph1,....,Graphk-1
  for(int i = 0; i < k; i++) {
    if(i == 0) {
      Graph *a = new Graph(i, queue_collection1[i], queue_collection1[i+k], queue_collection1[i+2*k], queue_collection2[i], queue_collection3[i]);
      graph_collection.push_back(a);
    } else {

      Graph *a = new Graph(i, queue_collection1[i], queue_collection1[i+k], queue_collection2[i-1], queue_collection2[i], queue_collection3[i]);
      graph_collection.push_back(a);
    }
    graph_name.push_back("graph_"+to_string(i));
  }

  // for(int i = 0; i < 2*k; i++) {
  //   cout <<"Queue Name"<< queue1_name[i] << "Value" << queue_collection1[i].front() << endl;
  // }
}

//convert calculation result from binary number to decimal number, and print out
void Adder::finalOutput(vector<int> true_result){
    int k = queue_collection3.size();
    vector<int> tmp(k, -1);

    std::ofstream myfile;
    myfile.open("output.txt", ios::out | ios::app);

    int ctr = 0;
    long clock = LONG_MAX;
    long sum = 0;

    for(int n = 0; n < 100; n++){
        ctr = 0;
        clock = 99999;
        for(int i = 0; i < k; i++){
            if(queue_collection3[i]->empty()){
                //cout << "time of collec[" <<i<<"]:" << "empty"<< endl;
                ctr++;
            } else if(queue_collection3[i]->front()->recvTime < clock){
                clock = queue_collection3[i]->front()->recvTime;
                //cout << "time of collec[" <<i<<"]:" << queue_collection3[i]->front()->recvTime <<",clock:"<<clock<< endl;
            }else {
                //cout << "time of collec[" <<i<<"]:" << queue_collection3[i]->front()->recvTime <<",clock:"<<clock<< endl;
            }
        }
        if(ctr >= k) break;

        //populate && exec events
        for(int i = 0; i < k; i++){
            if(!queue_collection3[i]->empty() && queue_collection3[i]->front()->recvTime <= clock){
                tmp[i] = queue_collection3[i]->front()->val;
                result[i] = tmp[i];
                queue_collection3[i]->pop();
                //cout << "pop succeed, clec[" <<i<<"]:"<< queue_collection3[i]->size() << endl;
            }
        }

        //compute sum
        sum = 0;
        for(int i = k-1; i >= 0; i--){
            if(tmp[i] != -1){
                sum = 2*sum + tmp[i];
            }
            else {
                sum = 2*sum + result[i];
            }
        }
        cout << clock << "," << sum << "," << true_result[clock] << endl;
        myfile << clock << "," << sum << "," << true_result[clock] << endl;
        clock++;
    }
    //cout << "final output finished with sum:" << sum <<endl;
    myfile.close();
}

//covert decimal number to binary number, and push to multi-bit adder
void Adder::Read(int a_input, int b_input, long receive_time, int k){

  vector<int> ain_tmp;
  vector<int> bin_tmp;
  vector<int> ain;
  vector<int> bin;
  int i = 0;
  while(a_input > 0) {
    ain_tmp.push_back(a_input % 2);
    a_input = a_input / 2;
    i++;
  }
  while(i < k) {
    ain_tmp.push_back(0);
    i++;
  }
  i = 0;
  while(b_input > 0) {
    bin_tmp.push_back(b_input % 2);
    b_input = b_input / 2;
    i++;
  }
  while(i < k) {
    bin_tmp.push_back(0);
    i++;
  }
  for(int i = k-1; i >= 0; i--) {
    ain.push_back(ain_tmp[i]);
    bin.push_back(bin_tmp[i]);
  }

  int count = 0;

  for(int i = k-1; i >= 0; i--) {
    Event *a = new Event("REGULAR",ain[i]);
    a->recvTime = receive_time;
    queue_collection1[count]->push(a);
    count++;
  }
  for(int i = k-1; i >= 0; i--) {
    Event *b = new Event("REGULAR",bin[i]);
    b->recvTime = receive_time;
    queue_collection1[count]->push(b);
    count++;
  }

  Event *c = new Event("REGULAR",0);
  c->recvTime = receive_time;
  queue_collection1[count]->push(c);

  // for(int i = 0; i < 2*k+1; i++) {
  //   cout <<"Queue Name"<< queue1_name[i] << "Value" << (queue_collection1[i]->front())->val << " size " << queue_collection1[i]->size()<< endl;
  // }
}

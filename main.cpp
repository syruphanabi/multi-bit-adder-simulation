#include <iostream>
#include "Graph.hpp"
#include "GNode.hpp"
#include "SimObj.hpp"
#include "Event.hpp"
#include "Adder.hpp"
#include <queue>
#include <bitset>
#include <sys/time.h>
#include <math.h>
#include <string>

using namespace std;

//tic toc
static double get_walltime(void){
  struct timeval tp;
  gettimeofday(&tp, NULL);

  return (double) (tp.tv_sec + tp.tv_usec/1.e6);
}

int main(int argc, const char * argv[]) {

    int epi = 10;
    double walltime;
    ofstream outfile("output.txt");
    outfile.close();

    int k = atoi(argv[1]);
    Adder a(k);

    //calculate mode: computer a pair of number
    if(argc == 5){

      int a_input = atoi(argv[2]);
      int b_input = atoi(argv[3]);
      int totaltime = atoi(argv[4]);
      vector<int> true_result;
      for (int t = 0; t < totaltime; t++){
        true_result.push_back((a_input+b_input)%(int)(pow(2,k)));
      }

      walltime = 0;
      for(int t = 0; t < totaltime; t++){
        a.Read(a_input,b_input,(long)(t), k);
        walltime -= get_walltime();
        //#pragma omp parallel for schedule(dynamic) num_threads(4)
        for(int i = 0; i < k; i++){
          a.graph_collection[i]->Receive();
          a.graph_collection[i]->Excute(epi);
        }
        walltime += get_walltime();
        a.finalOutput(true_result);
      }

      cout << "calculate mode: " << a_input << " + " << b_input << endl;
      cout << "time cost: " << walltime << endl;
      return 0;
    }

    //statistical mode: computer totaltime/interval pairs of number
    srand (time(NULL));
    int number = pow(2,k);	// upper limit of number
    int totaltime = atoi(argv[2]);
    int interval = atoi(argv[3]);
    int n = totaltime/interval;
    vector<int> a_input;
    vector<int> b_input;
    vector<int> true_result;
    int iteration = 0;
    for(int i = 0; i < n; i++) {
      int number_a = rand()%number;
      int number_b = rand()%number;
      for(int j = 0; j < interval; j++) {
        a_input.push_back(number_a);
          b_input.push_back(number_b);
          if(number_a+number_b >= number) true_result.push_back(number_a+number_b-number);
          else true_result.push_back(number_a+number_b);
      }
    }

    walltime = 0;
    for(int t = 0; t < totaltime; t++){
      a.Read(a_input[t],b_input[t],(long)(t), k);
      walltime -= get_walltime();
      //#pragma omp parallel for schedule(dynamic) num_threads(4)
      for(int i = 0; i < k; i++){
        a.graph_collection[i]->Receive();
        a.graph_collection[i]->Excute(epi);
      }
      walltime += get_walltime();
      a.finalOutput(true_result);
    }

    cout << "statistical mode: " << endl;
    cout << "time cost: " << walltime << endl;

    return 0;
}

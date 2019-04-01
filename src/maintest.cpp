#include "Population.h"
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

int main() {
    vector<double> hist;

    Population * p = new Population();
    p->init(105,40);
    //p->print();
    p->getMatrixFromFile("dataset1_formatted.csv",105);
    p->evaluation();
        for (int i=0; i<300; i++) {
        p->crossover();
        p->evaluation();
        p->mutation();
        p->selection();
        //cout << "---------------------EVAL_SORTED" << endl;
        //p->printEval();
        hist.push_back(p->getOptimumEval());
    }
    cout << "-------------------OPTIMUM: " << endl;
    p->printOptimum();
    cout << "----------------------HIST: " << endl;
    for (auto it=hist.begin(); it!=hist.end(); ++it) {
        cout << *it << ", ";
    }
    cout << endl;
    return 0;
}
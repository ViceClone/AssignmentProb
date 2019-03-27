#include "Population.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iterator>
using namespace std;

bool compare(Chronosome * c1, Chronosome * c2) {
    return c1->getEval()<c2->getEval();
}
Chronosome::Chronosome(int n, int* val) {
    this->n = n;
    this->val = val;
}

double Chronosome::evaluate(Matrix m) {
    double score = 0;
    for (int i=0; i<n; i++) {
        score += m.val[i][this->val[i]];
    }
    this->eval = score;
    return this->eval;
}

double Chronosome::getEval() {
    return eval;
}

Chronosome* Chronosome::PMX(Chronosome* c1, Chronosome* c2) {
    
}

void Population::init(int n, int size) {
    this->n = n;
    this->size = size;
    // TODO: init the vector
    // temp = {1,2,...,n}
    int temp[n];
    for (int i=0; i<n; i++) {
        temp[i] = i;
    }
    for (int i=0; i<size; i++) {
        random_shuffle(&temp[0],&temp[n]);
        int * val = new int[n];
        copy(&temp[0], &temp[n], val);
        Chronosome * c = new Chronosome(n,val);
        list.push_back(c);
    }
}

void Population::evaluation(Matrix m) {
    for (auto it=list.begin(); it!=list.end(); ++it) {
        (*it)->evaluate(m);
    }
}

void Population::selection() {
    sort(list.begin(),list.end(),compare);
}

double Population::getMean() {
    double sum=0;
    for (auto it=list.begin(); it!=list.end(); ++it){
        sum += (*it)->getEval();
    }
    return sum/size;
}

double Population::getStandardDeviation() {
    double mean = getMean();
    double s = 0;
    for (auto it=list.begin(); it!=list.end(); ++it){
        s += ((*it)->getEval()-mean) * ((*it)->getEval()-mean);
    }
    s = s/(size-1);
    return sqrt(s);
}
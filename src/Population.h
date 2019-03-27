#pragma once

#include <string>
#include <vector>
#include "Preprocessing.h"

class Chronosome {
public: 
    Chronosome(int n, int * val);
    ~Chronosome();
    static Chronosome* PMX(Chronosome* c1, Chronosome* c2);
    double evaluate(Matrix m);
    double getEval();
protected:
    int n;
    int * val;
    double eval;
};

class Population {
public:
    Population() {}
    ~Population() {}

    void init(int n, int size);
    void evaluation(Matrix m);
    void selection();
    void crossover();
    void mutation();

    double getMean();
    double getStandardDeviation();
protected:
    int size;
    int n;
    vector<Chronosome*> list;
}
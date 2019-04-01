#pragma once

#include <string>
#include <vector>
#include "Preprocessing.h"

class Chromosome {
public: 
    Chromosome(int n, int * val);
    Chromosome(int n, int * val, int * position);
    ~Chromosome();
    static Chromosome* PMX(Chromosome* c1, Chromosome* c2);
    void inversion(int n1, int n2);
    double evaluate(Matrix* m);
    double getEval();
    int* getVal();
    int* getPosition();
    void print();
protected:
    int n;
    int * val;
    int * position;
    double eval;
};

class Population {
public:
    Population() {}
    ~Population() {}

    void init(int n, int size);
    void evaluation();
    void selection();
    void crossover();
    void mutation();

    double getMean();
    double getStandardDeviation();

    void print();
    void printEval();
    void printOptimum();
    double getOptimumEval();
    void getMatrixFromFile(string filename, int n);
protected:
    int size;
    int n;
    vector<Chromosome*> list;
    Matrix* mat;
};
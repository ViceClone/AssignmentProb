#pragma once

#include <string>
#include "Preprocessing.h"

class Chronosome {
public: 
    Chronosome(int n, int * val);
    ~Chronosome();

    double evaluate(Matrix m);
private:
    int n;
    int * val;
}
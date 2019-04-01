#pragma once

#include <string>
using namespace std;

typedef struct Matrix {
    int m;
    int n;
    double ** val;
} Matrix;

class Preprocessing {
public:
    Preprocessing();
    ~Preprocessing();
    static Matrix getMatrixFromFile(string filename);
};
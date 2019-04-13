#include "Population.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iterator>
using namespace std;

double cube(double x) {
    x-=1;
    return x*x*x;
}

bool compare(Chromosome * c1, Chromosome * c2) {
    return c1->getEval()<c2->getEval();
}
Chromosome::Chromosome(int n, int* val) {
    this->n = n;
    this->val = val;
    position = new int[n];
    for (int i = 0; i<n; i++) {
        position[val[i]] = i;
    }
}

Chromosome::Chromosome(int n, int * val, int * position) {
    this->n = n;
    this->val = val;
    this->position = position;
}

Chromosome::~Chromosome() {
    delete[] val;
    delete[] position;
}

double Chromosome::evaluate(Matrix* m) {
    double score = 0;
    for (int i=0; i<n; i++) {
        score += m->val[i][this->val[i]];
    }
    this->eval = score;
    return this->eval;
}

double Chromosome::getEval() {
    return eval;
}

int* Chromosome::getPosition() {
    return position;
}

int* Chromosome::getVal(){
    return val;
}

Chromosome* Chromosome::PMX(Chromosome* c1, Chromosome* c2) {
    int n = c1->n;
    // choose random values 0 <= n1 <= n2 <= n-1
    int n1 = rand() % (n-1);
    int n2 = rand() % (n-n1-1) + n1+1;
    // Copy all the consecutive allenes from c1 to new Chromosome
    int * childVal = new int[n];
    for (int i=0; i<n; i++) {
        childVal[i] = c2->val[i];
    }
    for (int i=n1; i<=n2; i++) {
        childVal[i] = c1->val[i];
        if (c1->position[c2->val[i]] > n2 || c1->position[c2->val[i]] < n1) {
            int pos = i;
            while (pos >= n1 && pos <= n2) {
                int tempVal = c1->val[pos];
                pos = c2->position[tempVal];
            }
            childVal[pos] = c2->val[i];
        }
    }

    Chromosome* child = new Chromosome(n,childVal);
    return child;
}

void Chromosome::inversion(int n1, int n2) {
    int temp = val[n1];
    val[n1] = val[n2];
    val[n2] = temp;
    position[val[n1]] = n1;
    position[val[n2]] = n2;
}

void Chromosome::print() {
    for (int i = 0; i<n; i++) {
        cout << "  " << val[i];
    }
    cout << endl;
}

void Chromosome::printAssignment(Matrix* mat) {
    for (int i = 0; i<n; i++) {
        cout << "  " << mat->val[i][val[i]];
    }
    cout << endl;
}

void Population::init(int n, int size) {
    srand (time(NULL));
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
        Chromosome * c = new Chromosome(n,val);
        list.push_back(c);
    }
}

void Population::evaluation() {
    for (auto it=list.begin(); it!=list.end(); ++it) {
        (*it)->evaluate(mat);
    }
}

void Population::selection() {
    sort(list.begin(),list.end(),compare);
    int exceed = list.size()-size;
    for (int i=0; i<exceed; i++){
        Chromosome* c = list.back();
        list.pop_back();
        delete c;
    }
}

void Population::crossover() {
    for (int i=0; i<size/2; i++) {
        int n1 = rand() % size;
        int n2 = rand() % (size-n1) + n1;
        Chromosome* c1 = list[n1];
        Chromosome* c2 = list[n2];
        Chromosome* child = Chromosome::PMX(c1,c2);
        list.push_back(child);
    }
}

void Population::mutation() {
    for (auto it=list.begin(); it!=list.end(); ++it){
        int n1 = rand() % (n-1);
        int n2 = rand() % (n-n1-1) + n1+1;
        Chromosome* c1 = *it;
        int * val1 = c1->getVal();
        int * position1 = c1->getPosition();
        int* val2 = new int[n];
        int* position2 = new int[n];
        for (int i=0; i<n; i++) {
            val2[i] = val1[i];
            position2[i] = position1[i];
        }
        Chromosome* c2 = new Chromosome(n,val2,position2);
        c2->inversion(n1,n2);
        c2->evaluate(mat);
        if (c1->getEval()>c2->getEval()) {
            *it = c2;
            delete c1;
        }
    }
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

void Population::print() {
    for (auto it=list.begin();it!=list.end();++it) {
        (*it)->print();
    }
}

void Population::printEval() {
    for (auto it=list.begin();it!=list.end();++it) {
        cout << (*it)->getEval() << " ";
    }
    cout << endl;
}

void Population::printOptimum() {
    list[0]->print();
    list[0]->printAssignment(mat);
}

void Population::getMatrixFromFile(string filename, int n, double (*f)(double)) {
    mat = (Matrix*)malloc(sizeof(Matrix));
    mat->n = n;
    mat->val = new double*[n];
    ifstream ifs;
    ifs.open(filename);
    string line;
    if (!ifs) {
        cout << "unable to open file!" << endl;
        return;
    }
    int row = 0;
    for (;;) {
        if (!getline(ifs, line))
            break;
        mat->val[row] = new double[n];
        istringstream iss(line);
        for (int i=0; i<n; i++) {
            if (!(iss >> mat->val[row][i])){
                break;
            }
            mat->val[row][i] = f(mat->val[row][i]);
        }
        row++;
    }
}

double Population::getOptimumEval() {
    return list[0]->getEval();
}
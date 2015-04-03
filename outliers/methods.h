#ifndef METHODS_H_
#define METHODS_H_
#include<vector>
#include<cmath>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

int vectorToIntCellID(vector<int> id, int base);
vector<int> intToVectorCellID(int id, int base, int dimension);
vector<int> getLayerCellIdListByRecursive(vector<int> vId ,int base, int width);
int gaussRand(double e, double v);
void resultOutput(string path, vector<int> outliers,long cost_time);
#endif

#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;


class gaussSolver
{
public:
	void solveForGauss(int nRows, vector<double> eqMatrix, vector<double> &eqRoots);
	gaussSolver() {};
};





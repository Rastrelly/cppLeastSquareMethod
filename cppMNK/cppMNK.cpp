#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <ctime>
#include <fstream>
#include <locale>
#include "gaussSolver.h"

enum funcType 
{
	FT_LINEAR,
	FT_QUADRATIC
};

struct dPoint { double x, y; };
std::vector<dPoint> vData;
std::vector<dPoint> fData;
dPoint pMin = { 0,0 }, pMax = {0,0};
double a=0, b=0, c=0;
double A, B, C, D, E, F, G, N;

//set function type
bool useFullRandom = false;
funcType useFT = FT_QUADRATIC;
int useDataVolume = 20;

//random
std::default_random_engine re;

void calcEquationCoefficients()
{
	A = 0;
	B = 0;
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	N = vData.size();

	switch (useFT)
	{
	default:
	case FT_LINEAR:
	{
		for (dPoint cp : vData)
		{
			A += cp.x;
			B += pow(cp.x, 2);
			C += cp.y;
			D += cp.x*cp.y;
		}
		std::cout << "Linear set: A=" << A << "; B=" << B << "; C=" << C << "D=" << D << "\n";
		break;
	}
	case FT_QUADRATIC:
	{
		for (dPoint cp : vData)
		{
			A += 2*pow(cp.x, 4);
			B += 2*pow(cp.x, 3);
			C += 2*pow(cp.x, 2);
			D += 2*cp.x;
			E += 2 * pow(cp.x, 2)*cp.y;
			F += 2 * cp.x*cp.y;
			G += 2 * cp.y;
		}
		std::cout << "Quadratic set: A=" << A << "; B=" << B << "; C=" << C << "; D=" << D << "; E=" << E << "; F=" << F << "; G=" << G << "\n";
		break;
	}
		
	}
	
}

void solveEquation(double &ra, double &rb, double &rc)
{
	switch (useFT)
	{
	default:
	case FT_LINEAR:
	{
		rc = 0;
		rb = (D - (B*C / A)) / (A - (B*N / A));
		ra = (C - N * rb) / A;
		break;
	}
	case FT_QUADRATIC:
	{
		std::vector<double> inputEq =
		{
			A, B, C, E,
			B, C, D, F,
			C, D, 2 * N, G
		};
		std::vector<double> outputEq = {};
		gaussSolver GS;
		GS.solveForGauss(3, inputEq, outputEq);
		if (outputEq.size() >= 3)
		{
			ra = outputEq[0];
			rb = outputEq[1];
			rc = outputEq[2];
		}
		else
		{
			ra = 0; rb = 0; rc = 0;
		}
		break;
	}
	}
	std::cout << "a = " << ra << "; b = " << rb << "; c = " << rc << "\n";

}

double calcFuncDirect(double x)
{
	switch (useFT)
	{
	default:
	case FT_LINEAR:
	{
		return a * x + b;
		break;
	}
		
	case FT_QUADRATIC:
	{
		return a * pow(x, 2) + b * x + c;
		break;
	}
}
}



void calcFunc(int n)
{
	fData.clear();
	double d = (pMax.x - pMin.x) / (double)n;
	double cx = pMin.x;
	while (cx < pMax.x + d)
	{
		fData.push_back({cx,calcFuncDirect(cx)});
		cx += d;
	}
}

int randInt(int min, int max)
{
	return (rand() % (abs(max - min)+1)) + min;
}

double randDouble(double min, double max)
{	
	std::uniform_real_distribution<double> unif(min, max);
	return unif(re);
}

void makeData(bool fullRandom)
{
	int l = useDataVolume;
	vData.resize(l);
	if (fullRandom)
	{
		for (int i = 0; i < l; i++) vData[i] = { randDouble(-100,100), randDouble(-100,100) };
	}
	else
	{
		for (int i = 0; i < l; i++)
		{
			if (i == 0)
			{
				vData[i] = { randDouble(-100, 100),randDouble(-100, 100) };
			}
			else
			{
				if (rand() % 100 < 50)
				{
					vData[i] = { vData[i - 1].x + randDouble(-10, 10), vData[i - 1].y + randDouble(-10, 10) };
				}
				else
				{
					vData[i] = { vData[i - 1].x + randDouble(-1, 1), vData[i - 1].y + randDouble(-1, 1) };
				}
			}
		}
	}
}

void minMax(dPoint &min, dPoint &max)
{
	min = vData[0];
	max = vData[0];
	for (dPoint cp : vData)
	{
		if (cp.x < min.x) min.x = cp.x;
		if (cp.x > max.x) max.x = cp.x;
		if (cp.y < min.y) min.y = cp.y;
		if (cp.y > max.y) max.y = cp.y;
	}
}

void printPointsVectorToFile(std::vector<dPoint> vecToPrint, std::string fn)
{
	std::ofstream dataWriter(fn.c_str());
	for (dPoint cp : vecToPrint)
	{
		dataWriter << cp.x << ";" << cp.y << "\n";
	}
	dataWriter.close();
}

int main()
{
	std::locale::global(std::locale(""));
	srand(time(NULL));
	//1
	makeData(useFullRandom);
	printPointsVectorToFile(vData, "initial.csv");
	//2
	calcEquationCoefficients();
	//3
	solveEquation(a, b, c);
	//4
	minMax(pMin, pMax);
	calcFunc(100);
	printPointsVectorToFile(fData, "final.csv");
}

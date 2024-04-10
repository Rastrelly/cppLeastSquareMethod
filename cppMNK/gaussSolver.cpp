#include "gaussSolver.h"

void gaussSolver::solveForGauss(int nRows, vector<double> eqMatrix, vector<double> &eqRoots) {
	
	eqRoots.clear();
	int cnt = 0;
	
	int n, m;

	n = nRows;
	m = n + 1;

	double **A = new double *[n + 1]; //Memory for rows
	for (int i = 1; i <= n; i++)
		A[i] = new double[m + 1];    //Memory for columns
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			A[i][j] = eqMatrix[cnt]; cnt++;
			cout << A[i][j] << " ";
		}
		cout << "\n";
	}

	/*GAUSS*/
	/*---------------------------------Direct----------------------------------*/

	double tmp;
	double *x = new double[n + 1];
	for (int k = 1; k < n; k++)
	{
		for (int i = k + 1; i <= n; i++)
		{
			tmp = A[i][k] / A[k][k];
			for (int j = m; j >= k; j--) //Count from last element in a row
				A[i][j] -= tmp * A[k][j]; //
		}
	}

	//cout << "Matrix after direct pass: " << endl;

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (abs(A[i][j]) < 0.000001) A[i][j] = 0.0f;


	/*-------------------------------Reverse-------------------------------*/


	x[n] = A[n][m] / A[n][n];
	for (int i = n - 1; i >= 1; i--) {
		x[i] = A[i][m];
		for (int j = i + 1; j <= n; j++)
			x[i] -= A[i][j] * x[j];
		x[i] = x[i] / A[i][i];
	}

	for (int i = 1; i <= n; i++)
		if (abs(x[i]) < 0.00000001) x[i] = 0.0f;


	/*-------------------------------Solution output-------------------------------*/
	cout << "Solver output: \n";
	for (int i = 1; i <= n; i++)
	{
		cout << "x[" << i << "] = " << x[i] << "; ";
		eqRoots.push_back(x[i]);
	}
	cout << "\n";

	delete[] A;
}
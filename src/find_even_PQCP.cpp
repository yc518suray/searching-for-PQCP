// This program prints even length PQCPs
//
// Author: Raymond Su, q36141155@gs.ncku.edu.tw

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstdio>

using namespace std;

/* ----- declarations ----- */

bool isPQCP(vector<int> a, vector<int> b, int L);
vector<int> toSeq(vector<int> x, int L);

/* ----- main ----- */

int main(int argc, char ** argv)
{
	/* ---- input index sets ----- */
	ifstream infile(argv[1]);
	char outfileName[50];
	sprintf();
	ofstream outfile();;

	int L = stoi(argv[2]);
	int XSize = stoi(argv[3]);
	int YSize = stoi(argv[3]);

	vector<int> X(XSize, 0);
	vector<int> Y(YSize, 0);
	vector<int> a(L, 0);
	vector<int> b(L, 0);

	/* ----- output found pairs ----- */
	char ch;
	while(infile >> X[0])
	{
		for(int i = 1; i < XSize; i++) infile >> X[i];
		infile >> ch;
		for(int i = 0; i < YSize; i++) infile >> Y[i];

		a = toSeq(X, L);
		b = toSeq(Y, L);

		if(isPQCP(a, b, L))
		{
			for(int i = 0; i < L; i++) outfile << a[i] << " ";
			outfile << ", ";
			for(int i = 0; i < L; i++) outfile << b[i] << "";
			outfile << endl;
		}
	}

	/* ----- finish program ----- */
	outfile.close();
	infile.close();
	
	return 0;
}

/* ----- function definitions ----- */

bool isPQCP(vector<int> a, vector<int> b, int L)
{
	int corr_a[L - 1][L - 1] = {0};
	int corr_b[L - 1][L - 1] = {0};
}

vector<int> toSeq(vector<int> x, int L)
{
	int M = static_cast<int>(x.size());
	vector<int> tmp(L, 1);
	for(int i = 0; i < M; i++)
	{
		tmp[x[i]] = -1;
	}

	return tmp;
}

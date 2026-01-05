// Usage: ./find_even_PQCP [ADF input file] [L] [XSize] [YSize]
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
	
	int L = stoi(argv[2]);
	int XSize = stoi(argv[3]);
	int YSize = stoi(argv[4]);

	char outfileName[50];
	sprintf(outfileName, "results/%d-%d-%d-PQCP.out", L, XSize, YSize);
	ofstream outfile(outfileName);
	
	vector<int> X(XSize, 0);
	vector<int> Y(YSize, 0);
	vector<int> a(L, 0);
	vector<int> b(L, 0);

	/* ----- output found pairs ----- */
	char ch;
	bool file_empty = true;

	while(infile >> X[0])
	{
		file_empty = false;
		for(int i = 1; i < XSize; i++) infile >> X[i];
		infile >> ch;
		for(int i = 0; i < YSize; i++) infile >> Y[i];

		a = toSeq(X, L);
		b = toSeq(Y, L);

		if(isPQCP(a, b, L))
		{
			for(int i = 0; i < L; i++) outfile << a[i] << " ";
			outfile << ", ";
			for(int i = 0; i < L; i++) outfile << b[i] << " ";
			outfile << endl;
		}
	}

	if(file_empty) cout << "no ADF found" << endl;
	else cout << "ADF (therefore PQCP) found!" << endl;

	/* ----- finish program ----- */
	outfile.close();
	infile.close();
	
	return 0;
}

/* ----- function definitions ----- */

bool isPQCP(vector<int> a, vector<int> b, int L)
{
	int corr_a[L - 1] = {0};
	int corr_b[L - 1] = {0};
	int corr_sum[L - 1] = {0};

	int mercy = 0;

	for(int i = 0; i < L - 1; i++)
	{
		for(int j = 0; j < L; j++)
		{
			corr_a[i] += a[j] * a[(j + i + 1) % L];
			corr_b[i] += b[j] * b[(j + i + 1) % L];
		}
		corr_sum[i] = corr_a[i] + corr_b[i];
		if(corr_sum[i] == 4 || corr_sum[i] == -4) mercy++;
		else if(corr_sum[i] != 0) mercy += 5;
	}
	
	if(mercy == 2) return true;
	else return false;
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

// This program generates ADF candidates with specified parameters.
// ADF = Almost Difference Family
//
// Author: Raymond Su q36141155@gs.ncku.edu.tw
// Note: first part of each line is difference values (difference table)
// and second part of each line is the elements of index set.

#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<string>
#include<algorithm>

using namespace std;

/* ----- declarations ----- */

set<int> Orbit(int a, int H [], int size, int mod);
void fillZ_L(int (& arr) [], int M);
void cal_diff(vector<int> A, int (& diff) [], int M, int N);
void expand_orbit(vector<int> & v, vector<set<int>> a, vector<bool> sel);
int modulo(int x, int N);

/* ----- main ----- */

int main(int argc, char ** argv)
{
	/* ----- parameter settings ----- */
	// Note: K is always designated as the set with odd size
	int L = 46;
	int HSize = 2;
	int H[HSize] = {1, 45}; 	// we are using this H today!
	int JSize = 23;				// size of index set J, g0
	int KSize = 18;				// size of index set K, g1
	int type = 2;				// type 1 -> +4, type 2 -> -4
	int lambda_choice[2] = {JSize + KSize - L / 2, JSize + KSize - L / 2 - 1};
	int lambda = lambda_choice[type - 1];
	int singleton_choice = 2;
	int orbit_choice = 1;		// 1 -> normal choice 2 -> specified choice

	int Z_L[L] = {0};	
	fillZ_L(Z_L, L);

	set<set<int>> orbit;
	set<set<int>> singleton = {{0}, {23}};
	int singleton_arr[2][2];
	switch(singleton_choice)
	{
		case 0:
			singleton_arr[0][0] = 0;
			singleton_arr[0][1] = 23;
			singleton_arr[1][0] = 0;
			singleton_arr[1][1] = 23;
			break;
		case 1:
			singleton_arr[0][0] = 0;
			singleton_arr[0][1] = 23;
			singleton_arr[1][0] = 23;
			singleton_arr[1][1] = 0;
			break;
		case 2:
			singleton_arr[0][0] = 23;
			singleton_arr[0][1] = 0;
			singleton_arr[1][0] = 0;
			singleton_arr[1][1] = 23;
			break;
		case 3:
			singleton_arr[0][0] = 23;
			singleton_arr[0][1] = 0;
			singleton_arr[1][0] = 23;
			singleton_arr[1][1] = 0;
			break;
		default:
			cerr << "wrong singtleton choice!" << endl;
			break;
	};

	/* ----- file I/O settings ----- */
	char outFnameCanJ[50]; // to store candidates of J
	char outFnameCanK[50]; // to store candidates of K
	sprintf(outFnameCanJ, "results/%d-%d-%d-CAN-J.out", L, JSize, type);
	sprintf(outFnameCanK, "results/%d-%d-%d-CAN-K.out", L, KSize, type);

	/* ----- generate orbits ----- */
	for(int i = 0; i < L; i++) orbit.insert(Orbit(Z_L[i], H, HSize, L));

	/* ----- select candidates index set ----- */
	for(set<int> x: singleton) orbit.erase(x);
	vector<set<int>> orbit_vec;
	for(set<int> x: orbit) orbit_vec.push_back(x);
	
	ofstream outfileCJ(outFnameCanJ);
	ofstream outfileCK(outFnameCanK);
	
	/* --- some useful variables --- */
	vector<int> vJ;
	vector<int> vK;
	int diff_table[L - 1] = {0};
	vector<bool> selectionJ(orbit_vec.size(), false);
	vector<bool> selectionK(orbit_vec.size(), false);

	/* --- some size to be clarified --- */
	int JSize_remainder = JSize % HSize;
	int KSize_remainder = KSize % HSize;
	int numOrbitJ = (JSize - JSize_remainder) / HSize;
	int numOrbitK = (KSize - KSize_remainder) / HSize;
	
	if(orbit_choice == 2)
	{
		// customized orbit choice -> to include all singletons
		--numOrbitK;
		KSize_remainder += 2;
	}

	/* --- initialize selection vector --- */
	fill(selectionJ.end() - numOrbitJ, selectionJ.end(), true);
	fill(selectionK.end() - numOrbitK, selectionK.end(), true);

	/* ----- output candidates ----- */
	/* ----- J ----- */
	do
	{
		expand_orbit(vJ, orbit_vec, selectionJ);
		for(int i = 0; i < JSize_remainder; i++)
		{
			vJ.insert(vJ.end(), singleton_arr[0][i]);
		}
		cal_diff(vJ, diff_table, L - 1, JSize);
		sort(vJ.begin(), vJ.end());
		for(int i = 0; i < L / 2; i++)
		{
			// due to symmetry of difference table
			outfileCJ << diff_table[i] << " ";
			diff_table[i] = 0;
		}
		outfileCJ << ", ";
		for(int v: vJ) outfileCJ << v << " ";
		outfileCJ << endl;

		vJ.clear();
	}while(next_permutation(selectionJ.begin(), selectionJ.end()));
	cout << "finish generating candidates of J\n";
	/* ----- K ----- */
	do
	{
		expand_orbit(vK, orbit_vec, selectionK);
		for(int i = 0; i < KSize_remainder; i++)
		{
			vK.insert(vK.end(), singleton_arr[1][i]);
		}
		sort(vK.begin(), vK.end());
		cal_diff(vK, diff_table, L - 1, KSize);
		for(int i = 0; i < L / 2; i++)
		{
			// due to symmetry of difference table
			outfileCK << (lambda - diff_table[i]) << " ";
			diff_table[i] = 0;
		}
		outfileCK << ", ";
		for(int v: vK) outfileCK << v << " ";
		outfileCK << endl;

		vK.clear();
	}while(next_permutation(selectionK.begin(), selectionK.end()));
	
	outfileCJ.close();
	outfileCK.close();
	cout << "finish generating candidates of K\n";
	
	return 0;
}


/* ----- function definitions ----- */

set<int> Orbit(int a, int H [], int size, int mod)
{
	// generate orbits of a number a in Z_L

	set<int> temp_set;
	for(int i = 0; i < size; i++)
	{
		temp_set.insert((a * H[i]) % mod);
	}

	return temp_set;
}

void fillZ_L(int (& arr) [], int M)
{
	// fill the Z_L array

	for(int i = 0; i < M; i++) arr[i] = i;
}

void cal_diff(vector<int> A, int (& diff) [], int M, int N)
{
	// M -> length of difference table, M = L - 1
	// N -> size of the index set

	vector<int> B;
	B.resize(A.size());
	copy(A.begin(), A.end(), B.begin());

	for(int k = 0; k < N - 1; k++)
	{
		rotate(A.begin(), A.begin() + 1, A.end());
		for(int i = 0; i < N; i++) ++diff[modulo(A[i] - B[i], M + 1) - 1];
	}
}

void expand_orbit(vector<int> & v, vector<set<int>> a, vector<bool> sel)
{
	// sel -> selection of the orbits
	
	int M = a.size();
	for(int i = 0; i < M; i++)
	{
		if(sel[i])
		{
			for(int n: a[i]) v.insert(v.end(), n);
		}
	}
}

int modulo(int x, int N)
{
	// assume -N < x < N
	return (x < 0)? x + N: x;
}

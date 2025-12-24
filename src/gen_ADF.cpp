// ADF = Almost Difference Family

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

/* ----- main ----- */

int main(int argc, char ** argv)
{
	/* ----- parameter settings ----- */
	int L = 46;
	int HSize = 2;
	int singleSize = 2;
	int H[orbitSize] = {1, 45};
	int singleton[singleSize] = {0, 23};
	int JSize = ; // size of index set J
	int KSize = ; // size of index set K
	int Z_L[L] = {0};
	fillZ_L(Z_L, L);

	set<set<int>> orbit;

	/* ----- file I/O settings ----- */
	string outFnameCanJ; // to store candidates of J
	string outFnameCanK;
	string outFnameJ;	 // to store ADF J
	string outFnameK;
	sprintf(outFnameCanJ, "results/%d-CAN-J.out", L);
	sprintf(outFnameJ, "results/%d-ADF-J.out", L);
	sprintf(outFnameCanK, "results/%d-cAN-K.out", L);
	sprintf(outFnameK, "results/%d-ADF-K.out", L);

	/* ----- generate orbits ----- */
	for(int i = 0; i < L; i++) orbit.insert(orbit(Z_L[i], H, HSize, L));

	/* ----- select candidates index set ----- */
	for(int i = 0; i < singleSize; i++) orbit.erase({singleton[i]});
	vector<set<int>> = orbit_vec;
	for(set<int> x: orbit) orbit_vec.push_back(x);
	do
	{
	}while(next_permutation(orbit_vec.begin(), orbit_vec.end()));

	/* ----- output to candidate files ----- */


	cout << "finish generating candidates of J and K\n";

	/* ----- find ADFs  ----- */

	/* ----- output to found ADF files ----- */
	
	return 0;
}

/* ----- function definitions ----- */

set<int> Orbit(int a, int H [], int size, int mod)
{
	for(int i = 0; i < size; i++)
	{
		set<int> temp_set;
		temp_set.insert((a * H[i]) % mod);
	}

	return temp_set;
}

void fillZ_L(int (& arr) [], int M)
{
	for(int i = 0; i < M; i++) arr[i] = i;
}

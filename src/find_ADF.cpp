// Usage: ./find_ADF.cpp [# of lines of J candidates file]
// This program matches ADF candidates and find ADFs.
// Hopefully any ADF will be found.
//
// Author: Raymond Su. q36141155@gs.ncku.edu.tw

#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<limits>

using namespace std;

/* ----- declarations ----- */
void clearTag(int (& A) [], int N);
bool to_next_group(ifstream & infile, int (& tag) [], int N);
bool build_group(ifstream & in, vector<vector<int>> & vg, vector<int> & v,\
				 vector<vector<int>> & dT, int (& tag) [], int N, int M);
bool tag_not_matched(int A [], int B [], int N);
bool advance_J(int A [], int B [], int N);
int match_to_K(vector<int> x, vector<vector<int>> B, int N);

/* ----- some parameters ----- */
int L = 46;
int JSize = 20;
int KSize = 19;
int type = 1;

/* ----- global variables ----- */

/* ----- main ----- */
int main(int argc, char ** argv)
{
	/* ----- file I/O settings ----- */
	char inFnameJ[50];
	char inFnameK[50];
	char outFname[50];
	sprintf(inFnameJ, "results/%d-%d-%d-CAN-J-sort.out", L, JSize, type);
	sprintf(inFnameK, "results/%d-%d-%d-CAN-K-sort.out", L, KSize, type);
	sprintf(outFname, "results/%d-%d-ADF.out", L, type);
	ifstream infileJ(inFnameJ);
	ifstream infileK(inFnameK);
	ofstream outfile(outFname);
	
	/* ----- variables ----- */
	vector<vector<int>> diffTableJ;	// vector to hold difference table of J
	vector<vector<int>> diffTableK; // vector to hold difference table of K
	vector<int> vJ(JSize);			// vector to hold a J set
	vector<int> vK(KSize);			// vector to hold a K set
	vector<vector<int>> vJs;		// vector to hold a group of J sets
	vector<vector<int>> vKs;		// vector to hold a group of K sets

	int groupTagJ[3] = {0};			// group tag of a group of J sets
	int groupTagK[3] = {0};			// group tag of a group of K sets

	/* ----- the GREAT MATCH ----- */
	bool stop = false;
	int index_K = -1;
	for(int i = 0; i < 3; i++) infileJ >> groupTagJ[i];
	for(int i = 0; i < 3; i++) infileK >> groupTagK[i];
	
	while(!stop)
	{
		if(tag_not_matched(groupTagJ, groupTagK, 3))
		{
			/*
			cout << "tag not matched" << endl;
			cout << "tag of J: ";
			for(int i = 0; i < 3; i++) cout << groupTagJ[i] << " ";
			cout << "tag of K: ";
			for(int i = 0; i < 3; i++) cout << groupTagK[i] << " ";
			cout << endl << endl;
			*/

			// advance to next J or K candidate group
			if(advance_J(groupTagJ, groupTagK, 3))
			{
				stop = to_next_group(infileJ, groupTagJ, 3);
			}
			else
			{
				stop = to_next_group(infileK, groupTagK, 3);
			}
		}
		else
		{
			/*
			cout << "tag matched" << endl;
			cout << "tag of J: ";
			for(int i = 0; i < 3; i++) cout << groupTagJ[i] << " ";
			cout << "tag of K: ";
			for(int i = 0; i < 3; i++) cout << groupTagK[i] << " ";
			cout << endl << endl;
			*/

			// build J and K candidates group
			bool stopJ = build_group(infileJ, vJs, vJ, diffTableJ, groupTagJ, 3, L / 2);
			bool stopK = build_group(infileK, vKs, vK, diffTableK, groupTagK, 3, L / 2);
			stop = stopJ || stopK;
			
			// match difference tables to find ADFs
			int index_J = 0;
			for(vector<int> diffJx: diffTableJ)
			{
				index_K = match_to_K(diffJx, diffTableK, 3);
				if(index_K >= 0)
				{
					// found ADFs
					for(int x: vJs[index_J]) outfile << x << " ";
					outfile << ", ";
					for(int x: vKs[index_K]) outfile << x << " ";
					outfile << endl;
					index_K = -1;
				}
				++index_J;
			}

			vJs.clear();
			vKs.clear();
		}
	}

	/* ----- finish everything ----- */
	infileJ.close();
	infileK.close();
	outfile.close();
	
	return 0;
}


/* ----- function definitions ----- */

void clearTag(int (& A) [], int N)
{
	// A -> group tag
	// N -> size of group tag
	
	for(int i = 0; i < N; i++) A[i] = 0;
}

bool tag_not_matched(int A [], int B [], int N)
{
	// A -> tag of group of J candidate sets
	// B -> tag of group of K candidate sets
	// N -> size of group tag

	int cnt = 0;
	for(int i = 0; i < N; i++)
	{
		if(A[i] == B[i])
		{
			cnt += 3;
		}
		else if(A[i] == B[i] + 1)
		{
			cnt += 1;
		}
	}
	if(cnt < (3 * N - 2)) return true;
	else return false;
}

bool advance_J(int A [], int B [], int N)
{
	// A -> tag of group of J candidate sets
	// B -> tag of group of K candidate sets
	// N -> size of group tag

	for(int i = 0; i < N; i++)
	{
		if(A[i] > B[i])
		{
			return false;
		}
		else if(A[i] < B[i])
		{
			return true;
		}
		else;
	}

	cerr << "error with advance_J\n";
	return false;
}

bool to_next_group(ifstream & infile, int (& tag) [], int N)
{
	// tag -> tag of group to advance
	// N   -> size of group tag
	
	bool reached = false;
	int tmp_tag[N] = {0};

	infile.ignore(numeric_limits<streamsize>::max(), '\n');
	if(infile.peek() != EOF)
	{
		while(!reached)
		{
			for(int i = 0; i < N; i++)
			{
				infile >> tmp_tag[i];
				if(tmp_tag[i] != tag[i]) reached = true;
			}
			if(reached)
			{
				for(int i = 0; i < N; i++) tag[i] = tmp_tag[i];
			}
			else
			{
				infile.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}

bool build_group(ifstream & in, vector<vector<int>> & vg, vector<int> & v,\
				 vector<vector<int>> & dT, int (& tag) [], int N, int M)
{
	// N -> size of group tag
	// M -> size of difference table = L / 2

	/* --- process the first one in the group --- */
	vector<int> diff(M);
	for(int i = 0; i < N; i++) diff[i] = tag[i];
	for(int i = N; i < M; i++) in >> diff[i];
	dT.insert(dT.end(), diff);

	char ch;
	in >> ch; // the separating comma

	int vsize = static_cast<int>(v.size());
	for(int i = 0; i < vsize; i++) in >> v[i];
	vg.insert(vg.end(), v);

	/* --- process the rest of the group --- */
	bool reached = false;
	while(!reached)
	{
		if(in >> diff[0])
		{
			for(int i = 1; i < N; i++)
			{
				in >> diff[i];
				if(diff[i] != tag[i])
				{
					tag[i] = diff[i]; // update tag to succeeding group
					reached = true;
				}
			}

			if(reached) continue;
			for(int i = N; i < M; i++) in >> diff[i];
			dT.insert(dT.end(), diff);
			
			in >> ch; // the separating comma

			for(int i = 0; i < vsize; i++) in >> v[i];
			vg.insert(vg.end(), v);
		}
		else
		{
			return true; // EOF, stop the program
		}
	}
	return false; // not EOF, finish building group
}

int match_to_K(vector<int> x, vector<vector<int>> B, int N)
{
	// x -> single difference table of J candidates
	// B -> difference tables of K candidates
	// N -> size of group tag

	/* --- check if a mercy already exists --- */
	int mercy = 0;
	for(int i = 0; i < N; i++)
	{
		if(x[i] != B[0][i]) ++mercy;
	}

	/* --- match with B --- */
	int M = static_cast<int>(x.size()); // size of difference table
	int idx = 0;
	for(vector<int> y: B)
	{
		for(int i = N; i < M; i++)
		{
			if(x[i] == y[i] + 1)
			{
				++mercy;
			}
			else if(x[i] != y[i] + 1)
			{
				mercy += 5;
			}
			else;
			if(mercy > 1) break;
		}
		if(mercy == 1)
		{
			return idx;
		}
		++idx;
	}
	return -1;
}

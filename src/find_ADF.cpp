// Usage: ./find_ADF
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

/* ----- typedef ----- */
typedef vector<vector<int>> vv;

/* ----- declarations ----- */
void clearTag(int (& A) [], int N);
void swap(int (& A) [], int (& B) [], int N);
bool toofar(int a [], int b [], int N);
bool included(int a [], int b [], int N, int type);
bool to_next_group(ifstream & in, int (& tag) [], int N);
bool update_flock(ifstream & in, vv & vg, vv & dT, vv & taglist, int tag [], int N, int K, int L, int type);
bool build_group(ifstream & in, vv & vg, vv & dT, int (& tag) [], int N, int jsize, int L);
bool tag_matched(int A [], int B [], int N, int type);
int match_to_K(vector<int> x, vv B, int N, int type);

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
	
	if(!infileJ || !infileK)
	{
		cerr << "Error: CANNOT open input files!" << endl;
		return 1;
	}

	/* ----- variables ----- */
	vv diffTableJ;	// vector to hold difference tables of J
	vv diffTableK;	// vector to hold difference tables of K
	vv vJs;			// vector to hold groups of J sets
	vv vKs;			// vector to hold groups of K sets

	int tagJ[3] = {0};		// group tag of a group of J sets
	int tmp_tagK[3] = {0};	// group tag of a group of K sets
	int tagJnext[3] = {0};	// group tag of next group of J sets
	vv listTagK;			// list of group tags of K sets in a flock

	/* ----- the GREAT MATCH ----- */
	bool stop = false;
	bool stopJ, stopK;
	for(int i = 0; i < 3; i++)
	{
		infileJ >> tagJ[i];
		tagJnext[i] = tagJ[i];
	}
	
	/* ----- examine each group in J set ----- */
	while(!stop)
	{
		/* === debug === */
		//cout << "now J tag = [ ";
		//for(int i = 0; i < 3; i++) cout << tagJ[i] << " ";
		//cout << "], K tag = [ ";
		//for(int i = 0; i < 3; i++) cout << tmp_tagK[i] << " ";
		//cout << "]" << endl;
		//if(toofar(tmp_tagK, tagJ, 3)) cout << "tagK toofar from tagJ" << endl;
		/* === debug === */

		/* ----- update J group and K flock ----- */
		if(!toofar(tmp_tagK, tagJ, 3))
		{
			stopK = update_flock(infileK, vKs, diffTableK, listTagK, tagJ, 3, KSize, L, type);
			for(int i = 0; i < 3; i++) tmp_tagK[i] = listTagK[0][i];
		}
		stopJ = build_group(infileJ, vJs, diffTableJ, tagJ, 3, JSize, L);
		stop = stopJ || stopK;

		/* --- tagJnext -> tag of next group	--- */
		/* --- tagJ		-> tag of current group --- */
		swap(tagJnext, tagJ, 3);

		/* ----- examine the current J group ----- */
		if(listTagK.size() > 1)
		{
			/* === debug === */
			cout << "J tag = [";
			for(int i = 0; i < 3; i++) cout << setw(3) << tagJ[i];
			cout << "], K flock = [";
			for(int i = 0; i < 3; i++) cout << setw(3) << listTagK[0][i];
			cout << ",";
			for(int i = 0; i < 3; i++) cout << setw(3) << listTagK[listTagK.size() - 2][i];
			cout << "], flock size = " << setw(6) << diffTableK.size();
			cout << ", " << setw(3) << listTagK.size() - 1 << " tags in flock" << endl;
			/* === debug === */
	
			int pos1 = 0;
			clearTag(tmp_tagK, 3);
			for(vector<int> tagK: listTagK)
			{
				int pos2 = pos1 + tagK[3];
				
				/* --- match with each group in the K flock --- */
				int index_J = 0, index_K = 0;
				for(int i = 0; i < 3; i++) tmp_tagK[i] = tagK[i];
				if(tag_matched(tagJ, tmp_tagK, 3, type))
				{
					/* === debug === */
					//cout << "tag matched, J tag = [ ";
					//for(int i = 0; i < 3; i++) cout << tagJ[i] << " ";
					//cout << "], K tag = [ ";
					//for(int i = 0; i < 3; i++) cout << tagK[i] << " ";
					//cout << "], diffTableJ.size() = " << diffTableJ.size() << endl;
					/* === debug === */

					vv diffKs(diffTableK.begin() + pos1, diffTableK.begin() + pos2);
					
					for(vector<int> diffJ: diffTableJ)
					{
						index_K = pos1 + match_to_K(diffJ, diffKs, 3, type);
						if(index_K >= pos1)
						{
							/* --- Hurray! ADFs found! --- */
							for(int x: vJs[index_J]) outfile << x << " ";
							outfile << ", ";
							for(int x: vKs[index_K]) outfile << x << " ";
							outfile << endl;
						}
						++index_J;
					}
				}

				/* --- update starting position of K group --- */
				pos1 = pos2;
			}
		}

		/* --- to next J group --- */
		vJs.clear();
		diffTableJ.clear();

		/* --- tagJ -> tag of next group --- */
		for(int i = 0; i < 3; i++) tagJ[i] = tagJnext[i];
		
		/* --- tmp_tagK -> tag of first group in flock --- */
		for(int i = 0; i < 3; i++) tmp_tagK[i] = listTagK[0][i];
	}

	/* ----- finish program ----- */
	cout << endl << "finish finding ADFs" << endl;
	
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

void swap(int (& A) [], int (& B) [], int N)
{
	// N -> size of group tag

	int tmp;
	for(int i = 0; i < N; i++)
	{
		tmp = A[i];
		A[i] = B[i];
		B[i] = tmp;
	}
}

bool tag_matched(int A [], int B [], int N, int type)
{
	// A -> tag of group of J candidate sets
	// B -> tag of group of K candidate sets
	// N -> size of group tag

	int cnt = 0;
	int d = type - 1;
	for(int i = 0; i < N; i++)
	{
		if(A[i] == B[i] + d)
		{
			cnt += 3;
		}
		else if(A[i] == B[i] + 1 - d)
		{
			cnt += 1;
		}
	}
	if(cnt < (3 * N - 2)) return false;
	else return true;
}

bool included(int a [], int b [], int N, int type)
{
	// a -> tag of some group of K candidate sets
	// b -> tag of the corresponding group of J candidate sets
	// N -> size of tag

	int d = type - 1;

	if(a[0] != b[0] - d) return false; // note: this will not give exhaustive matching
	else if((a[1] < b[1] - 1) || (a[1] > b[1])) return false;
	else return true;
}

bool toofar(int a [], int b [], int N)
{
	// a -> K tag
	// b -> J tag
	// N -> size of tag

	int cnt = 0;
	for(int i = 0; i < N - 1; i++)
	{
		if(a[i] == b[i]) ++cnt;
		else if(a[i] > b[i])
		{
			if(cnt == i) return true;
		}
		else --cnt;
	}
	return false;
}

bool to_next_group(ifstream & in, int (& tag) [], int N)
{
	// tag -> tag of group to advance
	// N   -> size of group tag
	
	bool reached = false;
	int tmp_tag[N] = {0};

	in.ignore(numeric_limits<streamsize>::max(), '\n');
	while(true)
	{
		for(int i = 0; i < N; i++)
		{
			if(!(in >> tmp_tag[i])) return true;
			if(tmp_tag[i] != tag[i]) reached = true;
		}
		if(reached)
		{
			for(int i = 0; i < N; i++) tag[i] = tmp_tag[i];
			return false;
		}
		else
		{
			in.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

bool update_flock(ifstream & in, vv & vg, vv & dT, vv & taglist, int tag [], int N, int K, int L, int type)
{
	// tag -> tag of corresponding group of J set
	// N   -> size of group tag
	// L   -> sequence length
	
	int tmp_tag[N];					// temporary tag of K group
	bool stop = false;				// stop reading input file
	bool ready_to_build = false;	// ready to build the flock
	if(taglist.empty())
	{
		/* --- empty only at the very beginning --- */
		for(int i = 0; i < N; i++) in >> tmp_tag[i];
	}
	else
	{
		for(int i = 0; i < N; i++) tmp_tag[i] = taglist[0][i];
	}

	while(!ready_to_build)
	{
		if((taglist.size() == 1) || taglist.empty())
		{
			if(taglist.empty())
			{
				vector<int> t(N + 1, 0);
				taglist.insert(taglist.end(), t);
			}
			for(int i = 0; i < N; i++) taglist[0][i] = tmp_tag[i];
			
			/* --- move to starting group of flock --- */
			while(!included(tmp_tag, tag, N, type))
			{
				stop = to_next_group(in, tmp_tag, N);
				for(int i = 0; i < N; i++) taglist[0][i] = tmp_tag[i];
				if(toofar(tmp_tag, tag, N)) break;
				
				/* --- EOF, stop the program --- */
				if(stop) return true;
			}
			ready_to_build = true;
		}
		else
		{
			/* --- remove groups from flock --- */
			for(int i = 0; i < N; i++) tmp_tag[i] = taglist[0][i];
			
			int pos = 0;
			while(!included(tmp_tag, tag, N, type))
			{
				/* --- remove the associated vectors --- */
				pos = taglist[0][N];
				taglist.erase(taglist.begin());
				vg.erase(vg.begin(), vg.begin() + pos);
				dT.erase(dT.begin(), dT.begin() + pos);

				/* --- update tag to next group --- */
				for(int i = 0; i < N; i++) tmp_tag[i] = taglist[0][i];
				
				if(taglist.size() == 1) break;
			}
			
			if(taglist.size() > 1)
			{
				ready_to_build = true;
				for(int i = 0; i < N; i++) tmp_tag[i] = taglist[taglist.size() - 1][i];
			}
		}
	}

	/* --- no valid tags, taglist empty --- */
	if(toofar(tmp_tag, tag, N)) return false;

	/* --- add new groups to flock --- */
	vector<int> tg(N + 1, 0);	// taglist element
	int count = dT.size();		// count of K sets in the flock

	while(included(tmp_tag, tag, N, type))
	{
		/* --- add groups to flock --- */		
		stop = build_group(in, vg, dT, tmp_tag, N, K, L);
		
		/* --- update tag of next group & size of last element of taglist --- */
		if(!taglist.empty()) taglist[taglist.size() - 1][N] = dT.size() - count;
		count = dT.size();		

		for(int i = 0; i < N; i++) tg[i] = tmp_tag[i];
		taglist.insert(taglist.end(), tg);

		/* --- EOF, stop the program --- */
		if(stop) return true;
	}

	/* --- not EOF, finishing updating flock --- */
	return false;
}

bool build_group(ifstream & in, vv & vg, vv & dT, int (& tag) [], int N, int jsize, int L)
{
	// N	  -> size of group tag
	// jsize  -> size of J candidate sets
	// L	  -> sequence length

	int M = L / 2;
	vector<int> v(jsize, 0);

	/* --- process the first one in the group --- */
	vector<int> diff(M);
	for(int i = 0; i < N; i++) diff[i] = tag[i];
	for(int i = N; i < M; i++) in >> diff[i];
	dT.insert(dT.end(), diff);

	char ch;
	in >> ch; // the separating comma

	for(int i = 0; i < jsize; i++) in >> v[i];
	vg.insert(vg.end(), v);

	/* --- process the rest of the group --- */
	bool reached = false;
	while(!reached)
	{
		if(in >> diff[0])
		{
			/* --- examine tag --- */
			if(diff[0] != tag[0])
			{
				tag[0] = diff[0];
				reached = true;
			}
			for(int i = 1; i < N; i++)
			{
				in >> diff[i];
				if(diff[i] != tag[i])
				{
					tag[i] = diff[i]; // update tag to next group
					reached = true;
				}
			}

			/* --- add vectors to dT * vg --- */
			if(reached) continue;
			for(int i = N; i < M; i++) in >> diff[i];
			dT.insert(dT.end(), diff);
			
			in >> ch; // the separating comma

			for(int i = 0; i < jsize; i++) in >> v[i];
			vg.insert(vg.end(), v);
		}
		else
		{
			return true; // EOF, stop the program
		}
	}
	return false; // not EOF, finish building group
}

int match_to_K(vector<int> x, vv B, int N, int type)
{
	// x -> one difference table of J candidates
	// B -> difference tables of K candidates
	// N -> size of group tag

	int d = type - 1;

	/* --- check if a mercy already exists --- */
	int tag_mercy = 0;
	for(int i = 0; i < N; i++)
	{
		if(x[i] != B[0][i] + d) ++tag_mercy;
	}

	/* --- match with B --- */
	int M = static_cast<int>(x.size());
	int idx = 0;
	
	for(vector<int> y: B)
	{
		int mercy = tag_mercy;
		for(int i = N; i < M; i++)
		{
			/* --- counts of special points --- */
			if(x[i] == y[i] + 1 - d)
			{
				++mercy;
			}
			/* --- counts of invalid points --- */
			else if(x[i] != y[i] + d)
			{
				mercy += 5;
			}
			else;
			if(mercy > 1) break;
		}
		/* --- found --- */
		if(mercy == 1)
		{
			return idx;
		}

		++idx;
	}

	/* --- NOT found --- */
	return -1;
}

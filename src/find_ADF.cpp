// This program matches ADF candidates and find ADFs.
// Hopefully any will be found.
//
// Author: Raymond Su. q36141155@gs.ncku.edu.tw

#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;

/* ----- declarations ----- */

/* ----- some parameters ----- */
int L = 46;
int JSize = 20;
int KSize = 19;

/* ----- main ----- */
int main(int argc, char ** argv)
{
	/* ----- file I/O settings ----- */
	char inFnameJ[50];
	char inFnameK[50];
	char outFname[50];
	sprintf(inFnameJ, "results/%d-%d-%d-CAN-J.out", L, JSize, type);
	sprintf(inFnameK, "results/%d-%d-%d-CAN-K.out", L, KSize, type);
	sprintf(outFname, "results/%d-%d-ADF.out", L, type);
	ifstream infileJ(inFnameJ);
	ifstream infileK(inFnameK);
	ofstream outfile(outFname);

	/* ----- the MATCH and CHECK  ----- */


	/* ----- finish everything ----- */
	infileJ.close();
	infileK.close();
	outfile.close();
	
	return 0;
}


/* ----- function definitions ----- */

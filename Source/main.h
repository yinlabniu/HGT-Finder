#ifndef MAIN_H
#define MAIN_H 

#include "Blast.h"
#include "get_parms.h"
#include "StatFile.h"
#include "GFF.h"
#include <map>
#include <string>
using namespace std;

bool readArgs(int argc,
			  char * argv[],
			  string &dbBlastFile, 
			  string &selfBlastFile, 
			  int &taxID,
			  string &outputFile,
			  vector<float> &Rs,
			  string &statsFile,
			  string &gffFile,
			  float &pVal,
			  float &qVal,
			  int &holeSize);
vector<float> getRs(string s);
bool matches(string a, string b);
bool matchesCS(string a, string b);
string toLower(string a);

bool getStaticFiles(string exeLoc,
					string &namesFile,
					string &nodesFile,
					string &nrDBFile,
					string &percR,
					string &clustR);
string getExeDir(string exeLoc);

void runPercentile(string outFile, string exeLoc);
void combineStatFiles(vector<string> files, string output);

vector<vector<string> > cluster(unordered_set<string> candidates, 
								GFF &g, 
								int holeSize);
void appendVect(vector<string> &to, vector<string> from);

void printClusters(vector<vector<string> > clusters, string outFile);

#endif
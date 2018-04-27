#ifndef STATFILE_H
#define STATFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

struct valNode {
	float pVal;
	float qVal;

	valNode(float p, float q) : pVal(p), qVal(q) {};
	valNode() : pVal(-1), qVal(-1) {};
};

class StatFile
{
public:
	StatFile(string fileName, float cutoff, int type);
	unordered_set<string> getHGTCandidates();

private:
	string fileName;
	float cutoff;
	//0 for pvalue 1 for qvalue
	int type;

	string procLine(string line);
};

#endif
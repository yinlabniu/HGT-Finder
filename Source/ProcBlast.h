#include "Query.h"
#include "Subject.h"
#include "TaxTrace.h"
#ifndef PROCBLAST_H
#define PROCBLAST_H

#include "TaxTrace.h"
#include "Subject.h"
#include "Query.h"
#include <map>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

class ProcBlast
{
public:
	ProcBlast(Query &q, vector<Subject> &s, ofstream &o, TaxTrace *t);
	void procBlast();

private:
	int N;
	vector<int> qLineage;

	Query query;
	vector<Subject> subjects;
	ofstream *fout;
	TaxTrace *tTrace;

	unordered_set<int> subTaxes;

	// vector<Subject> uniqSubjects;

	void printData(float x);
	string getCommonNode(vector<string> a, vector<string> b);
	string printTaxonomy(vector<string> a, string common);
	vector<string> convTaxIDs(vector<int> taxIDs);
};

bool sortSubjects(Subject a, Subject b);

#endif


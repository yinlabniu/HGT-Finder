#ifndef BLAST_H
#define BLAST_H 

#include "BitScore.h"
#include "ProcBlast.h"
#include "Query.h"
#include "Subject.h"
#include "TaxTrace.h"
#include "DBContainer.h"
#include <string>
#include <unordered_set>
using namespace std;

class Blast
{
public:
	Blast(string bF);
	Blast(string bF, int t);
	
	void sBFile(string bF);
	int gTID();

	BitScore readSelfBlast(DBContainer &db);
	void readDBBlast(DBContainer &db, 
					 TaxTrace &tTrace, 
					 BitScore &bScore, 
					 ofstream &fout,
					 float r);

private:
	struct BlastLine {
		string query;
		string subject;
		float perID;
		int alignLength;
		int mismatches;
		int gapOpenings;
		int qStart;
		int qEnd;
		int sStart;
		int sEnd;
		float eVal;
		float bitScore;
	
		BlastLine(){};
		BlastLine(string q, string s, float pID, int aL, int m, int gO, int qS, 
			int qE, int sS, int sE, float e, int bs): query(q), subject(s), 
			perID(pID), alignLength(aL), mismatches(m), gapOpenings(gO), qStart(qS), 
			qEnd(qE), sStart(sS), sEnd(sE), eVal(e), bitScore(bs) {};
	};

	string bFile;
	vector<Subject> cSubjects;

	vector<int> qLineage;
	int N;

	int tID;

	BlastLine parseLine(string line);

	int numCommon(vector<int> a, vector<int> b, int &comTax);
};

#endif


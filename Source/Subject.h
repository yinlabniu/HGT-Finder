#ifndef SUBJECT_H
#define SUBJECT_H 

#include <string>
#include <vector>
using namespace std;

class Subject
{
public:
	Subject(string na, float r, float d, float n, vector<int> lineage, string co);

	string gName();
	float gR();
	float gD();
	int gNp();
	vector<int> gLineage();
	string gCommon();

private:
	string name;
	float R;
	float D;
	int np;
	vector<int> lineage;
	string commonNode;
};

#endif


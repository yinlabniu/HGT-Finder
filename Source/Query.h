#ifndef QUERY_H
#define QUERY_H 

#include <vector>
#include <string>
using namespace std;

class Query
{
public:
	Query(string na, int b, int kay, int n, vector<int> l);
	Query();

	string gName();
	int gBitScore();
	int gk();
	vector<int> gLineage();

	bool operator==(Query q);
	// Query operator=(Query q);

private:
	string name;
	int bitScore, k, N;
	vector<int> lineage;
};

#endif


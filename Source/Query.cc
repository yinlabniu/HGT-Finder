#include "Query.h"

Query::Query(string na, int b, int kay, int n, vector<int> l) {
	name = na;
	bitScore = b;
	k = kay;
	N = n;
	lineage = l;
}

Query::Query() {
	name = "";
	bitScore = -1;
	k = -1;
	N = -1;
}

string Query::gName() {
	return name;
}

int Query::gBitScore() {
	return bitScore;
}

int Query::gk() {
	return k;
}

vector<int> Query::gLineage() {
	return lineage;
}

bool Query::operator==(Query q) {
	if(this -> name == q.name)
		return true;
	else
		return false;
}


// Query Query::operator=(Query q) {
// 	Query rtrn(q.gName(), q.gBitScore(), q.gk(), q.gLineage());

// 	return rtrn;
// }
#include "Subject.h"

Subject::Subject(string na, float r, float d, float n, vector<int> l, string co) {
	name = na;
	R = r;
	D = d;
	np = n;
	lineage = l;
	commonNode = co;
}

string Subject::gName() {
	return name;
}

float Subject::gR() {
	return R;
}

float Subject::gD() {
	return D;
}

int Subject::gNp() {
	return np;
}

vector<int> Subject::gLineage() {
	return lineage;
}

string Subject::gCommon() {
	return commonNode;
}
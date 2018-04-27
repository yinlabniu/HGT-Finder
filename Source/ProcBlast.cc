#include "ProcBlast.h"

ProcBlast::ProcBlast(Query &q, vector<Subject> &s, ofstream &o, TaxTrace *t){
	query = q;
	qLineage = q.gLineage();
	N = qLineage.size();

	subjects = s;

	fout = &o;

	tTrace = t;

	// procBlast();
}

void ProcBlast::procBlast() {
	if(subjects.size() < 1) {
		cout << "No X for query: " << query.gName() << endl;
		return;
	}

	float x = 0;
	int count = 1;

	sort(subjects.begin(), subjects.end(), sortSubjects);

	for(auto i : subjects) {
		x += i.gR() * i.gD() / (float) count;
		count++;
	}

	x /= sqrt((float) subjects.size());

	printData(x);
}

void ProcBlast::printData(float x) {
	vector<string> qTaxonomy = convTaxIDs(query.gLineage());

	string commonNode;
	vector<int> sTaxIDs;
	vector<string> sTaxonomy;
	for(auto i : subjects) {
		sTaxIDs = i.gLineage();
		int sTax = sTaxIDs[sTaxIDs.size() - 1];
		sTaxonomy = convTaxIDs(sTaxIDs);

		commonNode = i.gCommon();

		*fout << query.gName() << "\t" << x << "\t" << subjects.size() << "\t";
		*fout << i.gName() << "\t" << sTax << "\t" << i.gD() << "\t";
		*fout << i.gR() << "\t" << N << "\t" << printTaxonomy(qTaxonomy, commonNode) << "\t";
		*fout << printTaxonomy(sTaxonomy, commonNode) << endl;
	}
}

string ProcBlast::getCommonNode(vector<string> a, vector<string> b) {
	string prevNode = "";

	int max;
	if(a.size() < b.size())
		max = a.size();
	else
		max = b.size();

	for(int i = 0; i < max; i++) {
		if(a[i] != b[i])
			return prevNode;

		prevNode = a[i];
	}

	return "";
}

string ProcBlast::printTaxonomy(vector<string> a, string common) {
	string tax;

	for(int i = 0; i < a.size(); i++) {
		if(a[i] == common)
			tax += "*" + a[i] + "*";
		else
			tax += a[i];

		if(i != a.size() - 1)
			tax += ";";
	}

	return tax;
}

vector<string> ProcBlast::convTaxIDs(vector<int> taxIDs) {
	vector<string> taxonomy;

	for(auto i : taxIDs) {
		taxonomy.push_back(tTrace -> getName(i));
	}

	return taxonomy;
}

bool sortSubjects(Subject a, Subject b) {
	return a.gR() > b.gR();
}
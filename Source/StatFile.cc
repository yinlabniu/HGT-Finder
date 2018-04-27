#include "StatFile.h"

StatFile::StatFile(string f, float c, int t) {
	fileName = f;
	cutoff = c;
	type = t;

	if(type != 0 && type != 1) {
		cerr << "Warning: StatFile Type invalid...";
	}
}

unordered_set<string> StatFile::getHGTCandidates() {
	unordered_set<string> candidates;

	ifstream fin(fileName.c_str());
	if(!fin.good()) {
		cerr << "Could not open file: " << fileName << endl;
		return candidates;
	}

	string line, gene;
	while(fin.good()) {
		getline(fin, line);
		gene = procLine(line);

		if(gene.size() > 0)
			candidates.insert(gene);
	}

	return candidates;
}

string StatFile::procLine(string l) {
	stringstream line(l);

	string name, col;
	float pVal, qVal;

	getline(line, col, '\t');
	name = col;

	getline(line, col, '\t');

	getline(line, col, '\t');
	pVal = atof(col.c_str());
	if(type == 0 && pVal < cutoff) {
		return name;
	}

	getline(line, col, '\t');
	qVal = atof(col.c_str());
	if(type == 1 && qVal < cutoff) {
		return name;
	}

	return "";
}
#include "Blast.h"

Blast::Blast(string bF) {
	bFile = bF;
	tID = -1;
}

Blast::Blast(string bF, int t) {
	bFile = bF;
	tID = t;
}

void Blast::sBFile(string bF) {
	bFile = bF;
}

int Blast::gTID() {
	return tID;
}

BitScore Blast::readSelfBlast(DBContainer &db) {
	BitScore bs;

	ifstream fin(bFile.c_str());
	if(!fin.good()) {
		cerr << "Could not open file: " << bFile << endl;
		return bs;
	}

	string line;
	BlastLine bLine;
	while(fin.good()) {
		getline(fin, line);
		bLine = parseLine(line);

		if(bs.exists(bLine.query) && bs[bLine.query] > bLine.bitScore)
			continue;

		bs.insert(bLine.query, bLine.bitScore);

		if(tID == -1) 
			tID = db[bLine.query];
	}

	return bs;
}

void Blast::readDBBlast(DBContainer &db, 
						TaxTrace &tTrace, 
						BitScore &bScore, 
						ofstream &fout,
						float r) {
	ifstream fin(bFile.c_str());
	if(!fin.good()) {
		cerr << "Could not open file: " << bFile << endl;
		return;
	}

	qLineage = tTrace.getTaxLineage(tID);
	N = qLineage.size();

	if(N < 1) {
		cerr << "Can't find query taxonomy\nTerminating\n";
		return;
	}

	string line;
	string currQ = "";
	string currS = "";
	string comNode = "";
	float currQBitScore = 0;
	unordered_set<int> sTIDs;

	int count = 0;
	float inc = bScore.size() / 50.0;
	cerr << "\t";

	while(fin.good()) {
		getline(fin, line);
		BlastLine bLine = parseLine(line);

		currS = bLine.subject;
		int sTaxID = db[currS];

		if(sTaxID == tID) {
			continue;
		}

		if(currQ != bLine.query) {
			if(currQ.size() > 0) {
				if(count ++ > inc) {
					cerr << "=";
					count = 0;
				}

				Query query(currQ, bScore[currQ], cSubjects.size(), N, qLineage);

				ProcBlast procBlast(query, cSubjects, fout, &tTrace);
				procBlast.procBlast();
			}

			cSubjects.clear();
			currQ = bLine.query;
			currQBitScore = bScore[currQ];
			sTIDs.clear();
		}

		float R = (float) bLine.bitScore / (float) currQBitScore;

		if(R < 0 || bLine.bitScore < 0) {
			cout << "Could not find bit score for subject: " << currS << endl;
			continue;
		} else if(R < r) {
			continue;
		} else if(R > 1) {
			cout << "Invalid R value for subject: " << currS << endl;
			// continue;
		}

		if(sTIDs.find(sTaxID) == sTIDs.end()) {
			sTIDs.insert(sTaxID);
		} else {
			continue;
		}

		vector<int> sTax = tTrace.getTaxLineage(sTaxID);

		if(sTax.size() < 2) {
			cout << "Could not find tax lineage for taxID: " << sTaxID << endl;
			continue;
		}

		int comTax = 0;
		int np = numCommon(sTax, qLineage, comTax);
		comNode = tTrace.getName(comTax);
		np = qLineage.size() - np;

		//if np < 0 -> subject is a subspecies of the query
		if(np <= 0) {
			cout << "subject " << sTaxID << " is subspecies of " << tID << endl;
			continue;
		}

		float D = (float) np / (float) N;

		cSubjects.push_back(Subject(currS, R, D, np, sTax, comNode));
	}

	cerr << endl;
}

int Blast::numCommon(vector<int> a, vector<int> b, int &comTax) {
	int max;
	if(a.size() > b.size())
		max = a.size();
	else
		max = b.size();

	int i = 0;
	for(i = 0; i < max; i++) {
		if(a[i] != b[i])
			break;

		comTax = a[i];
	}

	return i;
}

Blast::BlastLine Blast::parseLine(string l) {
	stringstream line(l);

	string col;
	int count = 0;
	string query, subject;
	float perID, eVal;
	int alignLength, mismatches, gapOpenings;
	int qStart, qEnd, sStart, sEnd;
	float bitScore;

	while(line.good()) {
		getline(line, col, '\t');

		if(count == 0) {
			stringstream ss(col);
			getline(ss, col, ' ');
			query = col;
		} else if(count == 1) {
			stringstream ss(col);
			getline(ss, col, ' ');
			subject = col;
		} else if(count == 2) {
			perID = atof(col.c_str());
		} else if(count == 3) {
			alignLength = atoi(col.c_str());
		} else if(count == 4) {
			mismatches = atoi(col.c_str());
		} else if(count == 5) {
			gapOpenings = atoi(col.c_str());
		} else if(count == 6) {
			qStart = atoi(col.c_str());
		} else if(count == 7) {
			qEnd = atoi(col.c_str());
		} else if(count == 8) {
			sStart = atoi(col.c_str());
		} else if(count == 9) {
			sEnd = atoi(col.c_str());
		} else if(count == 10) {
			eVal = atof(col.c_str());
		} else if(count == 11) {
			bitScore = atof(col.c_str());
		}

		count++;
	}

	BlastLine bLine(query, 
					subject, 
					perID, 
					alignLength, 
					mismatches,
					gapOpenings,
					qStart,
					qEnd,
					sStart,
					sEnd,
					eVal,
					bitScore);

	return bLine;
}
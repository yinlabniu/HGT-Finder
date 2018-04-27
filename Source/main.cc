#include "main.h"

int main(int argc, char * argv[]) {
	string dbBlastFile, selfBlastFile, outputFile;
	int taxID;
	vector<float> rVals;
	string statsFile, gffFile;
	float pVal, qVal;
	int holeSize;

	if(!readArgs(argc, 
				 argv, 
				 dbBlastFile, 
				 selfBlastFile, 
				 taxID, 
				 outputFile, 
				 rVals,
				 statsFile,
				 gffFile,
				 pVal,
				 qVal,
				 holeSize)) {
		cerr << "Improper arguments (Please see readme)\n";
		return 1;
	}

	cout << "dbBlast:    " << dbBlastFile << endl;
	cout << "selfBlast:  " << selfBlastFile << endl;
	cout << "taxID:      " << taxID << endl;
	cout << "outputFile: " << outputFile << endl;
	cout << "R cutoffs:  ";
	for(auto i : rVals) {
		cout << i << " ";
	}
	cout << endl;
	cout << "stats File: " << statsFile << endl;
	cout << "GFF File:   " << gffFile << endl;
	cout << "P-Value:    " << pVal << endl;
	cout << "Q-Value:    " << qVal << endl;
	cout << "Hole Size:  " << holeSize << endl;

	string exeLoc = argv[0];

	if(dbBlastFile != "") {
		string namesFile, nodesFile, taxDBFile, percR, clustR;

		if(!getStaticFiles(exeLoc, namesFile, nodesFile, taxDBFile, percR, clustR)) {
			return 2;
		}

		cout << "names:      " << namesFile << endl;
		cout << "nodes:      " << nodesFile << endl;
		cout << "tax DB:      " << taxDBFile << endl;
		cout << "percentil:  " << percR << endl;
		cout << "clustR:     " << clustR << endl;

		cerr << "Reading in tax DB...";
		DBContainer taxDB(taxDBFile);
		cerr << "Done\nReading in tax trace...";
		TaxTrace tTrace(namesFile, nodesFile);
		cerr << "Done\n";

		cerr << "Parsing self blast file...";
		Blast selfBlast(selfBlastFile);
		BitScore bScores = selfBlast.readSelfBlast(taxDB);
		cerr << "Done\n";

		cerr << "Parsing db blast file...\n";
		Blast dbBlast(dbBlastFile, taxID);

		string fileOut;
		string rStr;

		vector<string> files;

		for(auto i : rVals) {
			rStr = to_string(i);
			fileOut = outputFile + "." + rStr + ".dist";
			ofstream fout(fileOut.c_str());

			cerr << "\t" << fileOut << endl;

			if(!fout.good()) {
				cerr << "Could not open file: " << fileOut << endl;
			}

			dbBlast.readDBBlast(taxDB, tTrace, bScores, fout, i);

			fout.close();

			runPercentile(fileOut, exeLoc);

			files.push_back(fileOut + ".stats");
		}
		cerr << "Done\n";

		combineStatFiles(files, outputFile);

		statsFile = outputFile + ".stats.merged";
	}

	if(gffFile != "") {
		float cutoff = 0;
		int cutoffType = -1;
		if(pVal > 0) {
			cutoff = pVal;
			cutoffType = 0;
		} else if(qVal > 0) {
			cutoff = qVal;
			cutoffType = 1;
		}

		StatFile sFile(statsFile, cutoff, cutoffType);
		unordered_set<string> HGTCandidates = sFile.getHGTCandidates();

		GFF gff(gffFile, "gene");
		vector<vector<string> > clusters = cluster(HGTCandidates, gff, holeSize);

		printClusters(clusters, outputFile);
	}

	return 0;
}

bool readArgs(int argc,
			  char * argv[],
			  string &dbBlastFile, 
			  string &selfBlastFile, 
			  int &taxID,
			  string &outputFile,
			  vector<float> &Rs,
			  string &statsFile,
			  string &gffFile,
			  float &pVal,
			  float &qVal,
			  int &holeSize) {
	taxID = -1;
	pVal = -1;
	qVal = -1;
	holeSize = -1;

	map<string, string> parms = get_parms(argc, argv);

	string key, val;
	for(auto i : parms) {
		key = i.first;
		val = i.second;

		if(matchesCS(key, "-d") || matches(key, "--dbBlast")) {
			dbBlastFile = val;
		} else if(matchesCS(key, "-s") || matches(key, "--selfBlast")) {
			selfBlastFile = val;
		} else if(matchesCS(key, "-t") || 
				  matches(key, "--tax") || 
				  matches(key, "--taxID")) {
			taxID = atoi(val.c_str());
		} else if(matchesCS(key, "-o") || 
				  matches(key, "--out") || 
				  matches(key, "--output")) {
			outputFile = val;
		} else if(matches(key, "-r") || 
				  matches(key, "--rVals") || 
				  matches(key, "--rVal")) {
			Rs = getRs(val);
		} else if(matchesCS(key, "-S") || 
				  matchesCS(key, "-a") || 
				  matches(key, "--stats")) {
			statsFile = val;
		} else if(matchesCS(key, "-p") || matches(key, "--pval")) {
			pVal = atof(val.c_str());
		} else if(matchesCS(key, "-q") || matches(key, "--qval")) {
			qVal = atof(val.c_str());
		} else if(matchesCS(key, "-h") || matches(key, "--holeSize")) {
			holeSize = atoi(val.c_str());
		} else if(matchesCS(key, "-g") || 
				  matches(key, "--gff") || 
				  matches(key, "--gffFile")) {
			gffFile = val;
		} else {
			cerr << "Improper argument: " << key << " " << val << endl;
			return false;
		}
	}

	if( (dbBlastFile == "" || selfBlastFile == "" || Rs.size() == 0) &&
		statsFile == "") {
		cerr << "Improper parameters\n";
		return false;
	} 

	if(gffFile != "" || holeSize != -1 || (statsFile != "")) {
		if(gffFile == "" || 
		   holeSize < 1 || 
		   (statsFile == "" && dbBlastFile == "") || 
		   (pVal < 0 && qVal < 0) ) {
			cerr << "Improper parameters\n";
			return false;
		}
	}

	return true;
}

vector<float> getRs(string s) {
	vector<float> Rs;

	stringstream line(s);

	string col;
	float r;
	while(line.good()) {
		getline(line, col, ',');
		r = atof(col.c_str());

		Rs.push_back(r);
	}

	return Rs;
}

bool matches(string a, string b) {
	return toLower(a) == toLower(b);
}

bool matchesCS(string a, string b) {
	return a == b;
}

string toLower(string a) {
	string tLow;

	for(auto i : a) {
		tLow += tolower(i);
	}

	return tLow;
}

bool getStaticFiles(string exeLoc,
					string &namesFile,
					string &nodesFile,
					string &taxDBFile,
					string &percR,
					string &clustR) {
	string path = getExeDir(exeLoc);
	string fileLocsFile = path + "fileLocations";

	ifstream fin(fileLocsFile.c_str());
	if(!fin.good()) {
		cerr << "Could not open file: " << fileLocsFile << endl;
		return false;
	}

	string key, value;
	while(fin.good()) {
		getline(fin, key, '\t');
		getline(fin, value);

		if(key == "names")
			namesFile = path + value;
		else if(key == "nodes")
			nodesFile = path + value;
		else if(key == "taxDB")
			taxDBFile = path + value;
		else if(key == "percentile")
			percR = path + value;
		else if(key == "cluster")
			clustR = path + value;
	}	

	if(namesFile == "" || 
	   nodesFile == "" || 
	   taxDBFile == "" || 
	   percR == "" || 
	   clustR == "") {
		cerr << "fileLocations file incomplete or corrupt\n";
		return false;
	}

	return true;
}

string getExeDir(string exeLoc) {
	vector<string> dirs;
	string exeDir;

	stringstream path(exeLoc);

	string dir;
	while(path.good()) {
		getline(path, dir, '/');
		dirs.push_back(dir);
	}

	for(int i = 0; i < dirs.size() - 1; i++) {
		exeDir += dirs[i] + "/";
	}

	return exeDir;
}

void runPercentile(string outFile, string exeLoc) {
	string cutFile = outFile + ".cut";
	string cutCmd = "cut -f1,2 "  + outFile + " | uniq > " + cutFile;

	system(cutCmd.c_str());

	string dir = getExeDir(exeLoc);

	string RCmd = "Rscript " + dir + "FindPercentile.R " + cutFile + " " + outFile;

	system(RCmd.c_str());
}

void combineStatFiles(vector<string> files, string output) {
	string catCmd = "cat ";
	for(auto i : files) {
		catCmd += i + " ";
	}

	catCmd += "> " + output + ".stats.merged";

	system(catCmd.c_str());

	// string rmCmd = "rm " + output + "*.cut";

	// system(rmCmd.c_str());
}

vector<vector<string> > cluster(unordered_set<string> candidates, 
								GFF &g, 
								int holeSize) {
	vector<vector<string> > clusters;

	string gName;
	vector<string> hole;
	vector<string> currClust;

	for(auto i = g.begin(); i != g.end(); i++) {
		gName = i -> attributes["Name"];
		if(gName == "")
			gName = i -> attributes["ID"];
		if(gName == "")
			continue;

		auto find = candidates.find(gName);

		if(find != candidates.end()) {
			appendVect(currClust, hole);
			currClust.push_back("*" + gName + "*");
			hole.clear();
		} else {
			if(currClust.size() == 0) {
				hole.clear();
				continue;
			}

			hole.push_back(gName);

			if(hole.size() > holeSize) {
				if(currClust.size() > 1)
					clusters.push_back(currClust);
				currClust.clear();
				hole.clear();
			}
		}
	}

	return clusters;
}

void appendVect(vector<string> &to, vector<string> from) {
	for(auto i : from) {
		to.push_back(i);
	}
}

void printClusters(vector<vector<string> > clusters, string outFile) {
	outFile += ".clusters";

	ofstream fout(outFile.c_str());
	if(!fout.good()) {
		cerr << "Could not open file: " << outFile << endl;
		return;
	}

	fout << "query\tX\tk\tsubject\tsubject_taxonomy_id\tDistance\tR\tquery_lineage_length\tquery_taxonomy\tsubject_taxonomy\n";

	for(auto i : clusters) {
		fout << i.size() << "\t";

		for(auto j : i) {
			fout << j << " ";
		}

		fout << endl;
	}
}
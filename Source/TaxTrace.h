#ifndef TAXTRACE_H
#define TAXTRACE_H 

#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

//class to deal with Taxonomy trace
class TaxTrace
{
public:
	//initializes with names and nodes file
	TaxTrace(string namesFile, string nodesFile);
	TaxTrace();

	//initializes with names and nodes file
	void initWithFiles(string namesFile, string nodesFile);

	//given a taxID, returns the taxonomy of the taxID
	vector<string> getTaxonomy(string taxID);
	vector<string> getTaxonomy(int taxID);

	//given two tax IDs, calculates the distances between them
	vector<int> getTaxonomyDistance(string tax1, string tax2);
	vector<int> getTaxonomyDistance(int tax1, int tax2);

	//given two tax IDs, calculates the distances between them and fills the 
	//taxes vector with taxonomy for the taxIDs
	//If you want to do both, get distance and taxonomy, this is more efficient
	vector<int> getTaxonomyDistance(string tax1, 
									string tax2, 
									vector<vector<string> > &taxes);
	vector<int> getTaxonomyDistance(int tax1, int tax2, vector<vector<string> > &taxes);

	//calculates the Yin taxonomy distance
	float getYinTaxDistance(string tax1, string tax2);
	float getYinTaxDistance(int tax1, int tax2);

	//calculates the Yin taxonomy distance and fills taxonomy lineage information
	float getYinTaxDistance(string tax1, string tax2, vector<vector<string> > &taxes);
	float getYinTaxDistance(int tax1, int tax2, vector<vector<string> > &taxes);

	//averages the Yin Taxonomy distances
	vector<float> getAvgYinTaxDistance(int tax1, 
									   int tax2, 
									   int &NPrime, 
									   int &M, 
									   int &MPrime);

	//returns a name given a taxID
	string getName(int taxID);

	//gets the taxonomic lineage given a taxID.  Taxonomic lineage is
	//not of strings, rather of taxIDs.  
	//this is used for quicker comparisons when getting the deepest 
	//common node.
	vector<int> getTaxLineage(int taxID);

private:
	//hash maps for names, parents, and ranks
	unordered_map<int, string> names;
	unordered_map<int, int> parents;
	unordered_map<int, string> ranks;

	//reads a line from the names file
	void readNamesLine(string l);
	//reads a line from the nodes file
	void readNodesLine(string l);
	//chomps the leading and trailing whitespace from an entry.
	void chomp(string &s);
};

#endif
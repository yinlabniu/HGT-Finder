#include "TaxTrace.h"

/*
PUBLIC FUNCTIONS
*/

TaxTrace::TaxTrace(string namesFile, string nodesFile)
{
	//init the TaxTrace with two files
	initWithFiles(namesFile, nodesFile);
}

TaxTrace::TaxTrace() {
	
}

void TaxTrace::initWithFiles(string namesFile, string nodesFile)
{
	//open up the files
	ifstream namein(namesFile.c_str(), ios::in);
	ifstream nodein(nodesFile.c_str(), ios::in);

	//if either aren't good, output the error and return
	if(!namein.good() || !nodein.good())
	{
		cerr << "Could not initialize Tax Trace\n";
		return;
	}

	//string to hold a line
	string line;

	//grab a line and read it in
	while(namein.good())
	{
		getline(namein, line);
		readNamesLine(line);
	}

	while(nodein.good())
	{
		getline(nodein, line);
		readNodesLine(line);
	}

	return;
}

vector<string> TaxTrace::getTaxonomy(string taxID)
{
	//convert the taxID to an int and find it's Tax info
	return getTaxonomy(atoi(taxID.c_str()));
}
vector<string> TaxTrace::getTaxonomy(int taxID)
{
	//stack for taxLineage and vector for taxonomy
	stack<string> taxLineage;
	vector<string> taxonomy;

	//search taxID
	//if not found, return empy taxonomy
	//otherwise, push the name to the stack
	unordered_map<int, string>::iterator find = names.find(taxID);
	if(find == names.end())
	{
		return taxonomy;
	}
	else
		taxLineage.push(find -> second);

	//name to read in
	string name = "";
	//while our taxID isn't root (1)
	while(taxID != 1)
	{
		//search for that taxID's parent
		//if it can't be found, break and exit
		unordered_map<int, int>::iterator findPar = parents.find(taxID);
		if(findPar == parents.end())
			break;

		//find the taxID in the names
		//if we can't find it, enter in a "???"
		unordered_map<int, string>::iterator findName = names.find(findPar -> second);
		if(findName == names.end())
			name = "???";

		//set the taxID to the parent and set the name
		taxID = findPar -> second;
		name = findName -> second;

		//push the name to the stack
		taxLineage.push(name);
	}

	//go through the taxLineage stack and push each one into the vector
	//in reverse order
	while(taxLineage.size() > 0)
	{
		taxonomy.push_back(taxLineage.top());
		taxLineage.pop();
	}

	//return the taxonomy
	return taxonomy;
}

vector<int> TaxTrace::getTaxonomyDistance(string tax1, string tax2)
{
	//convert the taxIDs into ints and return the taxDistance
	return getTaxonomyDistance(atoi(tax1.c_str()), atoi(tax2.c_str()));
}
vector<int> TaxTrace::getTaxonomyDistance(int tax1, int tax2)
{
	//get the taxLineage for both taxIDs
	vector<int> taxLineage1 = getTaxLineage(tax1);
	vector<int> taxLineage2 = getTaxLineage(tax2);

	if(taxLineage1.size() < 1 || taxLineage2.size() < 1)
	{
		vector<int> tmpRtrn = { -1, -1, -1, -1, -1 };
		return tmpRtrn;
	}

	//set the max to the smaller lineage
	int max = taxLineage1.size();
	if(taxLineage1.size() > taxLineage2.size())
		max = taxLineage2.size();

	//integer to hold the number of elements they have in common in their lineages
	int commonLineage = 0;
	//traverse through the lineages in order
	//if we have a mismatch, break
	//otherwise, add one to the commonLineage count
	for(int i = 0; i < max; i++)
	{
		if(taxLineage1[i] != taxLineage2[i])
			break;

		commonLineage++;
	}

	//create a vector of distances to return
	vector<int> distances;
	//distances[0] = absolute distance
	//distances[1] = query side branch distance (N')
	//distances[2] = subject side branch distance (M')
	//distances[3] = query total distance to root (N)
	//distances[4] = subject total distance to root (M)
	distances.push_back((taxLineage1.size() - commonLineage) + 
						(taxLineage2.size() - commonLineage));
	distances.push_back(taxLineage1.size() - commonLineage);
	distances.push_back(taxLineage2.size() - commonLineage);
	distances.push_back(taxLineage1.size());
	distances.push_back(taxLineage2.size());

// if(tax1 == 331117 || tax2 == 331117){
// 	for(auto i : distances)
// 		cerr << i << " ";
// 	cerr << endl;
// }

	//return the distances
	return distances;
}

vector<int> TaxTrace::getTaxonomyDistance(string tax1, 
										  string tax2, 
										  vector<vector<string> > &taxes)
{
	//convert the taxIDs to ints and retur nthe tax distances
	return getTaxonomyDistance(atoi(tax1.c_str()), atoi(tax2.c_str()), taxes);
}
vector<int> TaxTrace::getTaxonomyDistance(int tax1, int tax2, vector<vector<string> > &taxes)
{
	//clear the taxes vector if not done already
	taxes.clear();

	//get the taxonomy for both
	taxes.push_back(getTaxonomy(tax1));
	taxes.push_back(getTaxonomy(tax2));

	//return the taxonomy distance between them
	return getTaxonomyDistance(tax1, tax2);
}

float TaxTrace::getYinTaxDistance(string tax1, string tax2)
{
	//convert the taxIDs and return the distance
	return getYinTaxDistance(atoi(tax1.c_str()), atoi(tax2.c_str()));
}
float TaxTrace::getYinTaxDistance(int tax1, int tax2)
{
	//grab the tax distances and return N'/N
	vector<int> distances = getTaxonomyDistance(tax1, tax2);

	if(distances[1] < 0 || distances[3] < 0)
		return -1;

	return float (distances[1]) / float (distances[3]);
}

//logic is similar to above

float TaxTrace::getYinTaxDistance(string tax1, string tax2, vector<vector<string> > &taxes)
{
	return getYinTaxDistance(atoi(tax1.c_str()), atoi(tax2.c_str()), taxes);
}
float TaxTrace::getYinTaxDistance(int tax1, int tax2, vector<vector<string> > &taxes)
{
	vector<int> distances = getTaxonomyDistance(tax1, tax2, taxes);
	if(distances[1] < 0 || distances[3] < 0)
		return -1;

	return float (distances[1]) / float (distances[3]);
}

vector<float> TaxTrace::getAvgYinTaxDistance(int tax1, 
								   			 int tax2, 
								   			 int &NPrime, 
								   			 int &M, 
								   			 int &MPrime)
{
	//these are the distances to return (2 total distances will be returned)
	vector<float> rtrnDistances;
	//grab the taxonomic distance information
	vector<int> distances = getTaxonomyDistance(tax1, tax2);

	if(distances[1] < 0 || distances[3] < 0)
	{
		vector<float> tmpRtrn = { -1, -1 };
		return tmpRtrn;
	}

	//calculate N'/N and M'/M
	float yinDistance1 = float (distances[1]) / float (distances[3]);
	float yinDistance2 = float (distances[2]) / float (distances[4]);

	//push back N'/N and (N'/N + M'/M) / 2
	rtrnDistances.push_back(yinDistance1);
	rtrnDistances.push_back((yinDistance1 + yinDistance2) / 2.0);

	//set the NPrime, M, and MPrime variables
	NPrime = distances[1];
	M = distances[4];
	MPrime = distances[2];

	//return the distances
	return rtrnDistances;
}

string TaxTrace::getName(int taxID)
{
	//name to return
	string name;
	//search for the name
	auto find = names.find(taxID);

	//if not found, reutrn empty name
	if(find == names.end())
		return name;

	//otherwise, return the name
	return find -> second;
}

/*
PRIVATE FUNCTIONS
*/

void TaxTrace::readNamesLine(string l)
{
	//int to hold column numbers and tax ID
	//strings to hold column data and name
	int colNum = 0;
	string column;
	int taxID;
	string name;

	//place string into stream
	stringstream line(l);

	//while the stream is good
	while(line.good())
	{
		//grab a column, delimate on |
		getline(line, column, '|');

		//chomp the column information
		chomp(column);

		//set the appropriate data based on column number
		switch(colNum)
		{
			case 0:
				taxID = atoi(column.c_str());
				break;
			case 1:
				name = column;
				break;
			default:
				break;
		}

		//increment column number
		colNum++;

		//if column says scientific name, set the data in the hash map and break
		if(column == "scientific name")
		{
			names[taxID] = name;
			break;
		}
	}

	return;
}
void TaxTrace::readNodesLine(string l)
{
	//integers to hold the column number, taxID, and parents
	//strings to hold the column and rank
	int colNum = 0;
	string column;
	int taxID;
	int parent;
	string rank;

	//place line in string stream
	stringstream line(l);

	while(line.good())
	{
		//grab column with delimiter of |
		getline(line, column, '|');
		//chomp the column
		chomp(column);

		//set data based on column number
		switch(colNum)
		{
			case 0:
				taxID = atoi(column.c_str());
				break;
			case 1:
				parent = atoi(column.c_str());
				break;
			case 2:
				rank = column;
				break;
			default:
				break;
		}

		//increment column number
		colNum++;
	}

	//set the parents and ranks
	parents[taxID] = parent;
	ranks[taxID] = rank;
}
void TaxTrace::chomp(string &s)
{
	//if the first character is a tab, clear it out
	if(s[0] == '\t')
		s = s.substr(1, s.size() - 1);
	//if the last character is a tab, clear it out
	if(s[s.size() - 1] == '\t')
		s = s.substr(0, s.size() - 1);
}

vector<int> TaxTrace::getTaxLineage(int taxID)
{
	//runs similar to the getTaxonomy...  Except this one grabs IDs
	//instead of names
	stack<int> taxLineage;
	vector<int> taxonomy;

	if(taxID <= 0)
		return taxonomy;

	taxLineage.push(taxID);

	while(taxID != 1)
	{
		unordered_map<int, int>::iterator findPar = parents.find(taxID);
		if(findPar == parents.end())
			break;

		taxID = findPar -> second;

		taxLineage.push(taxID);
	}

	while(taxLineage.size() > 0)
	{
		taxonomy.push_back(taxLineage.top());
		taxLineage.pop();
	}

	return taxonomy;
}
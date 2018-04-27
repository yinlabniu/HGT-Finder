#include "NRTax.h"

/*
Public methods
*/

const int defNRSize = 53512115;

//default constructor sets the NRSize
NRTax::NRTax()
{
	NRSize = defNRSize;
}
NRTax::NRTax(string f)
{
	//set the NRSize
	NRSize = defNRSize;
	
	//initialize the NRTax with a file
	initWithFile(f);
}
NRTax::NRTax(string f, int n)
{
	//set the NRSize
	NRSize = n;

dups = 0;

	//initialize NRTax with file
	initWithFile(f);

cerr << "NR dups: " << dups << endl;
cerr << "NR size: " << fileData.size() << endl;
}

void NRTax::initWithFile(string f)
{
	//reserve space in the hash map
	fileData.reserve(NRSize);

	//open up the file
	//if can't open, output error and return
	ifstream file(f.c_str(), ios::in);
	if(!file.good())
	{
		cerr << "Could not open file: " << f << endl;
		return;
	}

	//string to hold a line
	string line;
	//grab a line and read it in
	while(file.good())
	{
		getline(file, line);
		readLine(line);
	}
}

// int NRTax::getTaxNum(string search)
int NRTax::getTaxNum(string search)
{
	//search for search term
	auto find = fileData.find(search);

	//if not found, return 0
	if(find == fileData.end())
		return -1;

	//otherwise return the taxonomic ID
	return find -> second;
}
// int NRTax::getTaxNum(int search)
// {
// 	//init taxNum
// 	int taxNum = 0;

// 	//search for search term
// 	auto find = fileData.find(search);

// 	//if not found, return 0
// 	if(find == fileData.end())
// 		return taxNum;

// 	//otherwise return the taxonomic ID
// 	return find -> second;
// }

void NRTax::clear()
{
	//clear the hash table
	fileData.clear();
}

int NRTax::size()
{
	//return the size of the hash table
	return fileData.size();
}

/*
Private methods
*/

void NRTax::readLine(string l)
{
	//vector of strings to represent the columns
	vector<string> columns;
	stringstream line(l);

	//string to hold a read in column
	string column;
	//while we can read into column
	while(line >> column)
		//push back the column to the vector
		columns.push_back(column);

	//make sure there are enough columns to insert into hash map
	if(columns.size() < 2)
	{
		return;
	}

	//insert the columns into the hash map for lookup later
	int iKey = convertKeyToInt(columns[0]);

// if(fileData.find(iKey) != fileData.end())
// 	dups++;

	// fileData[iKey] = atoi(columns[1].c_str());
	fileData[columns[0]] = atoi(columns[1].c_str());
}

int NRTax::convertKeyToInt(string s)
{
	string sub = s.substr(3, 9);
	int gi = strhash(sub);

	return gi;
}
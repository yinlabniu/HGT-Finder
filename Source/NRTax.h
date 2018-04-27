#ifndef NRTAX_h
#define NRTAX_h 

#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <functional>
// #include "boost/date_time/posix_time/posix_time.hpp"
using namespace std;

//NRTax class
class NRTax
{
public:
	//initializes blank NR
	NRTax();
	//initiates NR with file f
	NRTax(string f);
	//initiates NR with file and sets NRSize
	NRTax(string f, int n);

	//initiates NR with file f
	void initWithFile(string f);

	//given a search term, returns the taxonomy number
	//returns value <= 0 on failure
	int getTaxNum(string search);
	// int getTaxNum(int search);

	//clears the fileData
	void clear();
	//returns the size of fileData
	int size();

private:
	//hashmap to hold file data
	unordered_map<string, int> fileData;
	int NRSize;

	hash<string> strhash;
int dups;

	//reads a line from the input file
	void readLine(string l);

	int convertKeyToInt(string s);
};

#endif
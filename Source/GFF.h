#ifndef GFF_H
#define GFF_H 

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;

struct GFFNode
{
	string seqid;
	string source;
	string type;
	int start;
	int end;
	string score;
	string strand;
	string phase;
	map<string, string> attributes;

	string getAttribute(string key);
};

class GFF
{
public:
	GFF(string f);
	GFF(string f, string type);

	void initWithFile();

	GFFNode operator[](int i);

	void setFile(string f);
	string getFile();

	vector<GFFNode>::iterator begin();
	vector<GFFNode>::iterator last();
	vector<GFFNode>::iterator end();

	int size();

	void printGFF();

private:
	vector<GFFNode> data;
	string file;
	string type;

	void parseLine(string l);
	map<string, string> getAttributes(string fullAttributes);

	string editName_p(string name);
};

#endif
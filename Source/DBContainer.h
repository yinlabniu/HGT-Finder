#ifndef _DBCONTAINER_H_
#define _DBCONTAINER_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

class DBContainer{

private:
	vector<int> seeds;
	vector<int> values;
	char checkBits;
	short taxBits;
	bool avail;

	unsigned int hashString(unsigned int d, string& key);
	unsigned int foldHash(unsigned int hash, int size);
	void init(const char* dbFile);
public:
	DBContainer(const char* dbFile);
	DBContainer(string& dbFile);
	~DBContainer();

	int operator[](string& key);
	int operator[](const char* key);
	bool good();
};

#endif

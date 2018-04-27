#ifndef BITSCORE_H
#define BITSCORE_H

#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

class BitScore
{
public:
	BitScore();
	void insert(string name, int bScore);
	int operator[](string name);
	bool exists(string name);

	void print();

	int size();

private:
	unordered_map<string, int> bScores;
};

#endif


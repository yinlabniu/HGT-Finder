#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <map>
#include <cstring>

using std::ifstream; using std::ofstream;
using std::ios;
using std::cout; using std::cerr;
using std::endl;
using std::map; using std::unordered_map;
using std::vector; using std::pair;
using std::string;

typedef pair<char*, int> ProteinToTaxId;
typedef vector<const ProteinToTaxId*> ProteinToTaxIdBucket;

unsigned int hashString( unsigned int d, const char* str );
unsigned int foldHash(unsigned int hash, int size);
int nlz10b(unsigned x);
pair<vector<int>,vector<int>> CreateMinimalPerfectHash( const vector<ProteinToTaxId>& dict );
void printHelp();

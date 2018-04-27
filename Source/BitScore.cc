#include "BitScore.h"

BitScore::BitScore() {

}

void BitScore::insert(string name, int bScore) {
	bScores[name] = bScore;
}

int BitScore::operator[](string name) {
	auto find = bScores.find(name);

	if(find == bScores.end())
		return -1;
	else
		return find -> second;
}

bool BitScore::exists(string name) {
	auto find = bScores.find(name);

	if(find == bScores.end())
		return false;
	else
		return true;
}

void BitScore::print() {
	for(auto i : bScores) {
		cout << i.first << "\t" << i.second << endl;
	}
}

int BitScore::size() {
	return bScores.size();
}
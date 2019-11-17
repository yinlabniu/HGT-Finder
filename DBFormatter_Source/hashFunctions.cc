#include "hashFunctions.h"


// Calculates a distinct hash function for a given string. Each value of the
// integer d results in a different hash value.
unsigned int hashString( unsigned int d, const char* str ){
    if (d == 0) d = 0x01000193;

    // Use the FNV algorithm
    for(int i = 0; str[i] != 0; i++){
        d = ( (d * 0x01000193) ^ str[i] ) & 0xffffffff;
    }
    return d;
}

// XOR Fold a hash to a smaller bit value
unsigned int foldHash(unsigned int hash, int size){
	int fold = 16;
	while(size < fold){
		hash = (((hash>>fold) ^ (hash & ((u_int32_t)1<<fold)-1)));
		fold /= 2;
	}
	size = fold*2 - size;
	return (((hash>>size) ^ (hash & ((u_int32_t)1<<size)-1)));
}

// Count the leading 0 bits using NLZ10b algorithm
int nlz10b(unsigned x) {

   static char table[64] =
     {32,20,19, 0, 0,18, 0, 7,  10,17, 0, 0,14, 0, 6, 0,
       0, 9, 0,16, 0, 0, 1,26,   0,13, 0, 0,24, 5, 0, 0,
       0,21, 0, 8,11, 0,15, 0,   0, 0, 0, 2,27, 0,25, 0,
      22, 0,12, 0, 0, 3,28, 0,  23, 0, 4,29, 0, 0,30,31};

   x = x | (x >> 1);    // Propagate leftmost
   x = x | (x >> 2);    // 1-bit to the right.
   x = x | (x >> 4);
   x = x | (x >> 8);
   x = x & ~(x >> 16);
   x = x*0xFD7049FF;
   return table[x >> 26];
}


pair<vector<int>,vector<int>> CreateMinimalPerfectHash( const vector<ProteinToTaxId>& dict ){
    int size = dict.size();

    // Step 1: Place all of the keys into buckets
    vector<ProteinToTaxIdBucket> buckets;
    buckets.resize(size);
    vector<int> G;
    G.resize(size);
    vector<int> values;
    values.resize(size);
    vector<bool> filled;
    filled.resize(size);
    fill(filled.begin(),filled.end(),0);

    for(auto itr = dict.begin(); itr != dict.end(); itr++){
        buckets[hashString(0, itr->first) % size].push_back( &*itr );
    }

    // Step 2: Sort the buckets and process the ones with the most items first.
    sort( buckets.begin(), buckets.end(), [](const ProteinToTaxIdBucket& a, const ProteinToTaxIdBucket& b) {
        return a.size() > b.size();
    });

    int b = 0;
    for(; b < size; b++){
        ProteinToTaxIdBucket& bucket = buckets[b];
        if (bucket.size() <= 1) break;

        int d = 1;
        int item = 0;
        vector<int> slots;

        // Repeatedly try different values of d until we find a hash function
        // that places all items in the bucket into free slots
        while( item < bucket.size()){
            int slot = hashString( d, bucket[item]->first ) % size;
            if (filled[slot] == true || find(slots.begin(),slots.end(),slot) != slots.end()){
                d += 1;
                item = 0;
                slots.clear();
	    }
            else{
                slots.push_back( slot );
                item += 1;
	    }
        }

        G[hashString(0, bucket[0]->first) % size] = d;
        for(int i = 0; i < bucket.size(); i++){
            values[slots[i]] = bucket[i]->second;
	    filled[slots[i]] = true;
	}
    }

    // Only buckets with 1 item remain. Process them more quickly by directly
    // placing them into a free slot. Use a negative value of d to indicate
    // this.
    vector<int> freelist;
    for (int i = 0; i < size; i++){
        if (filled[i] == false) freelist.push_back( i );
    }

    for (; b < size; b++ ){
        ProteinToTaxIdBucket& bucket = buckets[b];
        if( bucket.size() == 0) break;
        int slot = freelist.back();
	freelist.pop_back();
        // We subtract one to ensure it's negative even if the zeroeth slot was
        // used.
        G[hashString(0, bucket[0]->first) % size] = -slot-1;
        values[slot] = bucket[0]->second;
    }
    return make_pair(G, values);
}

void printHelp(){
	cerr << "Arguments [-i input file, tab delimited geneID->taxID]" << endl
	     << "          <-o output file. Defaults to [input].hdb>" << endl;
}


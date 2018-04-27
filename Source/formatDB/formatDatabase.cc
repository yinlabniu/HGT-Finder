#include "hashFunctions.h"

int main(int argc, char* argv[]){
        map<string,string> arguments;
        for(int i=1;i<argc;i+=2){
                if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
                        printHelp();
                        return 0;
                }
                else {
                        if(argv[i][0] != '-'){
                                cerr << "Invalid argument " << argv[i] << endl;
                                printHelp();
                                return 0;
                        }
                        else{
                                arguments[argv[i]] = argv[i+1];
                        }
                }
        }

	if(arguments["-i"] == ""){
		cerr << "Input file required." << endl;
		printHelp();
	}

	unordered_map<string,int> dict;
	int tax = 0;
	ifstream file; file.open(arguments["-i"],ios::in);
	string key;
	char zeroes = 32;
	while(file >> key){
	    file >> tax;
	    dict[key] = tax;
	    int lZ = nlz10b(tax);
            if(lZ < zeroes) zeroes = lZ;
	}

	for(auto itr = dict.begin(); itr != dict.end(); itr++){
		unsigned int hash = hashString(0,itr->first);
                itr->second = foldHash(hash,zeroes)<<(sizeof(int)*8-zeroes) | itr->second;
	}

	pair<vector<int>,vector<int>> result = CreateMinimalPerfectHash( dict );
	file.close();

	string outFile;
	if(arguments["-o"] != ""){outFile = arguments["-o"];}
	else{outFile = arguments["-i"]+".hdb";}
	ofstream ofile;
	ofile.open(outFile.c_str(),ios::out|ios::binary);
	ofile.write(&zeroes,sizeof(zeroes));
	ofile.write((char*)result.first.data(),sizeof(int)*result.first.size());
        ofile.write((char*)result.second.data(),sizeof(int)*result.first.size());
        ofile.close();
}

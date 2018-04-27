#ifndef GET_PARMS_H
#define GET_PARMS_H

#include <iostream>
#include <map>
#include <vector>
#include <stdlib.h>
using namespace std;

//Overloaded functions get parameters into a map
map<string,string> get_parms(int argc, 
			   char * argv[]);
map<string,string> get_parms(int argc, 
			   char * argv[], 
			   vector<string> good_opts,
			   void (*usage)());
map<string,string> get_parms(int argc, 
			   char * argv[], 
			   void (*usage)(),
			   vector<string> good_opts,
			   vector<vector<string> > required_opts);
//checks to ensure that a read in option is an accepted option if the option list is
//supplied
bool check_opt(string opt, vector<string> good_opts);

#endif
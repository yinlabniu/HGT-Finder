#include "get_parms.h"

//1 for each argument
//	1a if reading in opt, read in option
//	1b else read in value for option
//	1c set option, value pair in map
//2 return map
map<string,string> get_parms(int argc, 
			   char * argv[])
{
	map<string, string> parms_read;

	string in ="";
	//1 for each argument
	for(int i = 1; i < argc; i++)
	{
		//1a if reading in option, do it
		if(in == "")
		{
			in = argv[i];
		}
		//1b else read in value for option
		else
		{
			//1c set the option, value pair in map
			parms_read[in] = argv[i];
			in = "";
		}
	}

	//return map
	return parms_read;
}
//1 for each argument
//	1a if reading in option, do it
//	1b check to make sure option is good
//	1c else read in value for option
//	1d set the option, value pair in map
//2 return map
map<string,string> get_parms(int argc, 
			   char * argv[], 
			   vector<string> good_opts,
			   void (*usage)())
{
	map<string, string> parms_read;

	string in ="";
	//1 for each argument
	for(int i = 1; i < argc; i++)
	{
		//1a if reading in option, do it
		if(in == "")
		{
			in = argv[i];
			//1b check to make sure option is good
			bool good = check_opt(in, good_opts);
			if(good)
				continue;
			else
				usage();
		}
		//1c else read in value for option
		else
		{
			//1d set the option, value pair in map
			parms_read[in] = argv[i];
			in = "";
		}
	}

	//2 return map
	return parms_read;
}
//1 read in options
//2 for each required option (set)
//3 for each option in that set
//	3a check and see if option is used
//	3b if so, set true and break;
//4 if option not used
//	4a output usage
//5 return map
map<string,string> get_parms(int argc, 
			   char * argv[], 
			   void (*usage)(),
			   vector<string> good_opts,
			   vector<vector<string> > required_opts)
{
	//1 read in options
	map<string, string> parms_read = get_parms(argc, argv, good_opts, usage);
	vector<bool> required_opts_found;

	//2 for each option set
	for(int i = 0; i < required_opts.size(); i++)
	{
		required_opts_found.push_back(false);

		//for each option in that set
		for(int j = 0; j < required_opts[i].size(); j++)
		{
			//3a check and see if option is used
			if(parms_read.find(required_opts[i][j]) != parms_read.end())
			{
				//3b if so, set true and break
				required_opts_found[i] = true;
				break;
			}
		}

		//4 if option not used, output usage
		if(required_opts_found[i] == false)
			usage();
	}

	//return map
	return parms_read;
}
//1 for each valid option
//	1a check to see if opt matches up, if so, return true
//2 return false if no matches
bool check_opt(string opt, vector<string> good_opts)
{
	//1 for each valid option
	for(int i = 0; i < good_opts.size(); i++)
	{
		//1a chec kto see if opt matches up, if so, return true
		if(opt == good_opts[i])
			return true;
	}

	//2 return false if nothing matches
	return false;
}
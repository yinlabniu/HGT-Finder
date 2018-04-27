#include "GFF.h"

string GFFNode::getAttribute(string key)
{
	string value;
	auto find = attributes.find(key);

	if(find == attributes.end())
		return value;

	return find -> second;
}

GFF::GFF(string f)
{
	file = f;

	initWithFile();
}
GFF::GFF(string f, string t)
{
	file = f;
	type = t;

	initWithFile();
}

void GFF::initWithFile()
{
	ifstream fin(file.c_str(), ios::in);
	if(!fin.good())
	{
		cerr << "Could not open file: " << file << endl;
		return;
	}

	string line;
	while(fin.good())
	{
		getline(fin, line);
		parseLine(line);
	}
}

GFFNode GFF::operator[](int i)
{
	return data[i];
}

void GFF::setFile(string f)
{
	file = f;
}
string GFF::getFile()
{
	return file;
}

vector<GFFNode>::iterator GFF::begin()
{
	return data.begin();
}
vector<GFFNode>::iterator GFF::last()
{
	auto l = data.end();
	l--;

	return l;
}
vector<GFFNode>::iterator GFF::end()
{
	return data.end();
}

int GFF::size()
{
	return data.size();
}

void GFF::printGFF()
{
	for(auto i : data)
	{
		cout << i.seqid << "\t" << i.source << "\t" << i.type << "\t";
		cout << i.start << "\t" << i.end << "\t" << i.score << "\t";
		cout << i.strand << "\t" << i.phase << "\t";

		auto lastAttr = i.attributes.end();
		lastAttr--;

		for(auto j : i.attributes)
		{
			cout << j.first << "=";

			if(j.first == "Name")
				cout << editName_p(j.second);
			else
				cout << j.second;

			if(j.first != lastAttr -> first)
				cout << ";";
		}
		cout << endl;
	}
}

void GFF::parseLine(string l)
{
	if(l.size() < 1 || l[0] == '#')
		return;

	stringstream line(l);

	GFFNode gNode;

	int count = 0;
	string temp;
	while(line.good())
	{
		getline(line, temp, '\t');

		switch(count)
		{
			case 0:
				gNode.seqid = temp;
				break;
			case 1:
				gNode.source = temp;
				break;
			case 2:
				if(temp != type && type != "")
					return;

				gNode.type = temp;
				break;
			case 3:
				gNode.start = atoi(temp.c_str());
				break;
			case 4:
				gNode.end = atoi(temp.c_str());
				break;
			case 5:
				gNode.score = temp;
				break;
			case 6:
				gNode.strand = temp;
				break;
			case 7:
				gNode.phase = temp;
				break;
			case 8:
				gNode.attributes = getAttributes(temp);
				break;
			default:
				break;
		}

		count++;
	}

	data.push_back(gNode);
}
map<string, string> GFF::getAttributes(string fullAttributes)
{
	map<string, string> attributes;

	stringstream ss(fullAttributes);

	string attribute, key, value;
	while(ss.good())
	{
		getline(ss, attribute, ';');

		int count = 0;
		stringstream ss1(attribute);

		while(ss1)
		{
			if(count++ == 0)
				getline(ss1, key, '=');
			else
				getline(ss1, value);
		}

		attributes[key] = value;
	}

	return attributes;
}

string GFF::editName_p(string name)
{
	string editedName;

	stringstream ss(name);

	string column, temp;
	int count = 0;
	while(ss.good())
	{
		getline(ss, column, '|');
		if(count++ == 0)
		{
			stringstream ss2(column);
			getline(ss2, column, '.');

			while(ss2.good())
				getline(ss2, temp);
		}

		editedName += column + "|";
	}

	return editedName;
}
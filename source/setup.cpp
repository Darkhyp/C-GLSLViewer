#include "setup.h"


vector<string> split(string& str, char delim) {
	vector<string> result;
	stringstream ss(str);
	string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

string trim(string& line) {

	auto start = line.find_first_not_of(" \n\r\t");
	if (start == string::npos)
		return string();

	auto end = line.find_last_not_of(" \n\r\t");

	return line.substr(start, end - start + 1);
}


map<string, string> ReadINIValues(string iniFileName)
{
	map<string, string> optionMap;
	string line;

	ifstream file(iniFileName);

	cout << "Reading " << iniFileName << "..." << endl;
	if (file)
	{
		stringstream buffer;

		buffer << file.rdbuf();

		while (getline(buffer, line)) {
			if (line[0] == '#') continue;
			vector<string> tmp = split(line, '=');
			if (tmp.size() == 2)
				optionMap[trim(tmp[0])] = trim(tmp[1]);
			else
				cout << "line '" << line << "' does not have single '='! Skipping this line..." << endl;
		}

		file.close();

	}
	else
		cout << "file (" << iniFileName << ") not found!" << endl;

	return optionMap;
}


Setup::Setup(string iniFileName) : iniFileName(iniFileName) {}


void Setup::load()
{
	// reading setup file
	map<string, string> optionMap = ReadINIValues(iniFileName);

	// ******************** treating values from setup file ************************
	// check width option
	try
	{
		width = stoi(optionMap["width"]);
	}
	catch (...)
	{
		cout << "window width is not defined. Use default 800." << endl;
		width = 800;
	}
	cout << "width = " << width << endl;
	
	// check height option
	try
	{
		height = stoi(optionMap["height"]);
	}
	catch (...)
	{
		cout << "window height is not defined. Use default 600." << endl;
		height = 600;
	}
	cout << "height = " << height << endl;
	
	// check vertexSource option
	try
	{
		vertexSource = optionMap["vertexSource"];
		cout << "Using vertexSource from '" << vertexSource << "'" << endl;
	}
	catch (...)
	{
		cout << "vertexSource is not defined. Exit..." << endl;
		exit(0);
	}
	
	// check fragmentSource option
	try
	{
		fragmentSource = optionMap["fragmentSource"];
		cout << "Using fragmentSource from '" << fragmentSource << "'" << endl;
	}
	catch (...)
	{
		cout << "fragmentSource is not defined. Exit..." << endl;
		exit(0);
	}

	// check textureSource option
	try
	{
		textureSource = optionMap["textureSource"];
		cout << "Using textureSource from '" << textureSource << "'" << endl;
	}
	catch (...)
	{
		cout << "textureSource is not defined. Exit..." << endl;
		exit(0);
	}

	// check textureUniformName option
	try
	{
		textureUniformName = optionMap["textureUniformName"];
		cout << "Using textureUniformName = '" << textureUniformName << "'" << endl;
	}
	catch (...)
	{
		cout << "textureUniformName is not defined. Exit..." << endl;
		exit(0);
	}


}

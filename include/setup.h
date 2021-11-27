#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#pragma once
using namespace std;

class Setup
{
public:
	// ini file
	string iniFileName;

	// window size
	int width, height;
	string vertexSource, fragmentSource;

	Setup(string iniFileName);

	void load();
};


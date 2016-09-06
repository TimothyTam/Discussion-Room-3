#pragma once

#include <string>
#include <vector>

#include "validation.h"
using namespace std;

class MainQuery {

public:
	MainQuery();
	vector<string> processQuery(string query);
};


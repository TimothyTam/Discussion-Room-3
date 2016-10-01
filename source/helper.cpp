#pragma once
#include <iostream>
#include "helper.h"

using namespace std;

void Helper::printVi(vi v) {

	for (size_t i = 0; i < v.size(); i++) {
		cout << v[i] << ", ";
	}
	cout << "\n";
}

Helper::Helper()
{
}

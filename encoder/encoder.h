#pragma once
#include<vector>
#include <iostream>
using namespace std;
class additonalInfo {
public:
	additonalInfo() {
		for (int i = 0; i < 256; i++) {
			this->mask.push_back(0);
		}
	}
	vector<pair<int, int>> pairs;
	vector<int> mask;
	vector<pair<unsigned short, unsigned short>> bitmap1;
	vector<pair<unsigned short, unsigned short>> bitmap2;
};
#pragma once
#include<vector>
#include <iostream>
using namespace std;
class additonalInfo {
public:
	additonalInfo(int length) {
		for (int i = 0; i < 256; i++) {
			this->mask.push_back(0);
		}
		toembedLength = length;
	};
	vector<pair<int, int>> pairs;

	//以下两个成员变量是需要传递的附加信息
	vector<int> mask;
	int toembedLength;
	//直方图平移的方法中是不需要的
	//vector<pair<unsigned short, unsigned short>> bitmap1;
	//vector<pair<unsigned short, unsigned short>> bitmap2;
};
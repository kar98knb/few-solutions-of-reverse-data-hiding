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

	//����������Ա��������Ҫ���ݵĸ�����Ϣ
	vector<int> mask;
	int toembedLength;
	//ֱ��ͼƽ�Ƶķ������ǲ���Ҫ��
	//vector<pair<unsigned short, unsigned short>> bitmap1;
	//vector<pair<unsigned short, unsigned short>> bitmap2;
};
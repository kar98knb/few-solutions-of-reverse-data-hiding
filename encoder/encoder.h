#pragma once
#include<vector>
#include <iostream>
using namespace std;
class additonalInfo {
public:
	additonalInfo(){}
	additonalInfo(int length, int imageGrayscale_0_0, int imageGrayscale_0_1) {
		for (int i = 0; i < 256; i++) {
			this->mask.push_back(0);
		}
		toembedLength = length;
		imageGrayscaleAt_0_0 = imageGrayscale_0_0;
		imageGrayscaleAt_0_1 = imageGrayscale_0_1;
	};

	vector<int> mask;  //已废弃

	int imageGrayscaleAt_0_0 = -1, imageGrayscaleAt_0_1 = -1;  //固定16bits
	vector<pair<int, int>> pairs;  //每对8+8bits
	int toembedLength = -1;  //  固定16bits
	vector<pair<int,int>> bitmap1;   //每个位置16bits
	vector<int> bitmapMinLength;     //最多128对，即最多128个非零零值点，每个零值点的最大值为107，所以不能为这个数组分配固定长度
	int lengthForBitmapMinLength = 0;    //上述数组的长度，也即非零零值点的个数，固定8bits
	//vector<pair<unsigned short, unsigned short>> bitmap2;

	int totalAddLength = -1;

	//这个函数在转换成员变量时会将新变量添加到字符前，因此decoder端先读到的内容在这个函数里写在最后
	string AddInfo() {
		string extendInfo;

		//转换bitmap1
		for (int i = bitmap1.size() - 1; i >= 0; i--) {
			int row = pairs[i].first, col = pairs[i].second;
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(col & 1) + extendInfo;
				col >>= 1;
			}
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(row & 1) + extendInfo;
				row >>= 1;
			}
		}

		//转换bitmapMinLength
		for (int i = bitmapMinLength.size() -1; i >=0 ; i--) {
			int temp = bitmapMinLength[i];
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(temp & 1) + extendInfo;
				temp >>= 1;
			}
		}

		//转换pairs
		for(int i = pairs.size() - 1; i >= 0; i--){
			int max = pairs[i].first, min = pairs[i].second;
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(min & 1) + extendInfo;
				min >>= 1;
			}
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(max & 1) + extendInfo;
				max >>= 1;
			}
		}

		//转换lengthForBitmapMinLength(8bits)
		for (int i = 0, lengthForBitmapMinLength = bitmapMinLength.size(); i < 8; i++) {
			extendInfo = to_string(lengthForBitmapMinLength & 1) + extendInfo;
			lengthForBitmapMinLength >>= 1;
		}

		//转换toembedLength(16bits)
		for (int toEmbedLength = toembedLength, i = 0; i < 16; i++) {
			extendInfo = to_string(toEmbedLength & 1) + extendInfo;
			toEmbedLength >>= 1;
		}

		//转换imageGrayscaleAt_0_0,imageGrayscaleAt_0_1(8+8bits)
		int temp = imageGrayscaleAt_0_0;
		for (int i = 0; i < 8; i++) {
			extendInfo = to_string(temp & 1) + extendInfo;
			temp >>= 1;
		}

		temp = imageGrayscaleAt_0_1;
		for (int i = 0; i < 8; i++) {
			extendInfo = to_string(temp & 1) + extendInfo;
			temp >>= 1;
		}

		//添加整体addinfo的长度(16bits)，包含自身
		totalAddLength = extendInfo.length() + 16;
		temp = totalAddLength;
		for (int i = 0; i < 16; i++) {
			extendInfo = to_string(temp & 1) + extendInfo;
			temp >>= 1;
		}

		return extendInfo;
	}
};
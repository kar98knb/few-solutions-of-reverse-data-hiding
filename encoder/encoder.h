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

	vector<int> mask;  //�ѷ���

	int imageGrayscaleAt_0_0 = -1, imageGrayscaleAt_0_1 = -1;  //�̶�16bits
	vector<pair<int, int>> pairs;  //ÿ��8+8bits
	int toembedLength = -1;  //  �̶�16bits
	vector<pair<int,int>> bitmap1;   //ÿ��λ��16bits
	vector<int> bitmapMinLength;     //���128�ԣ������128��������ֵ�㣬ÿ����ֵ������ֵΪ107�����Բ���Ϊ����������̶�����
	int lengthForBitmapMinLength = 0;    //��������ĳ��ȣ�Ҳ��������ֵ��ĸ������̶�8bits
	//vector<pair<unsigned short, unsigned short>> bitmap2;

	int totalAddLength = -1;

	//���������ת����Ա����ʱ�Ὣ�±�����ӵ��ַ�ǰ�����decoder���ȶ��������������������д�����
	string AddInfo() {
		string extendInfo;

		//ת��bitmap1
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

		//ת��bitmapMinLength
		for (int i = bitmapMinLength.size() -1; i >=0 ; i--) {
			int temp = bitmapMinLength[i];
			for (int j = 0; j < 8; j++) {
				extendInfo = to_string(temp & 1) + extendInfo;
				temp >>= 1;
			}
		}

		//ת��pairs
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

		//ת��lengthForBitmapMinLength(8bits)
		for (int i = 0, lengthForBitmapMinLength = bitmapMinLength.size(); i < 8; i++) {
			extendInfo = to_string(lengthForBitmapMinLength & 1) + extendInfo;
			lengthForBitmapMinLength >>= 1;
		}

		//ת��toembedLength(16bits)
		for (int toEmbedLength = toembedLength, i = 0; i < 16; i++) {
			extendInfo = to_string(toEmbedLength & 1) + extendInfo;
			toEmbedLength >>= 1;
		}

		//ת��imageGrayscaleAt_0_0,imageGrayscaleAt_0_1(8+8bits)
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

		//�������addinfo�ĳ���(16bits)����������
		totalAddLength = extendInfo.length() + 16;
		temp = totalAddLength;
		for (int i = 0; i < 16; i++) {
			extendInfo = to_string(temp & 1) + extendInfo;
			temp >>= 1;
		}

		return extendInfo;
	}
};
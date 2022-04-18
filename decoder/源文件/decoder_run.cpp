#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<fstream>
#include <cassert>
#include<json/json.h>
#include "../../encoder/encoder.h"
using namespace cv;
using namespace std;


void usingHis(Mat image);
void usingDE(Mat image);

bool isChangeable(int mean, int dif) {
	int min = (2 * (255 - mean) < 2 * mean + 1) ? 2 * (255 - mean) : 2 * mean + 1;
	if (abs(2 * (dif >> 1) + 1) < min && abs(2 * (dif >> 1)) < min) {
		return true;
	}
	else {
		return false;
	}
}

void Ini(string embedInfo,addInfoHis &addinfo) {
	string totalAddLength = embedInfo.substr(0, 16);
	string toembedLength = embedInfo.substr(32, 16);
	for (int i = 15,mask = 1; i >= 0; i--) {
		if (totalAddLength[i] == '1') {
			addinfo.totalAddLength += mask;
		}
		if (toembedLength[i] == '1') {
			addinfo.toembedLength += mask;
		}
		mask <<= 1;
	}

	string imageGrayscaleAt_0_0 = embedInfo.substr(16, 8);
	string imageGrayscaleAt_0_1 = embedInfo.substr(24, 8);
	string lengthForBitmapMinLength = embedInfo.substr(48, 8);
	for (int i = 7, mask = 1; i >= 0; i--) {
		if (imageGrayscaleAt_0_0[i] == '1') {
			addinfo.imageGrayscaleAt_0_0 += mask;
		}
		if (imageGrayscaleAt_0_1[i] == '1') {
			addinfo.imageGrayscaleAt_0_1 += mask;
		}
		if (lengthForBitmapMinLength[i] == '1') {
			addinfo.lengthForBitmapMinLength += mask;
		}
		mask <<= 1;
	}
}

int str_to_int(string str) {
	int ans = 0;
	int mask = 1;
	for (int i = str.size() - 1; i >= 0; i--) {
		if (str[i] == '1') {
			ans += mask;
		}
		mask <<= 1;
	}
	return ans;
}

string recoverBitmap(string &str,int limit) {
	string ans;
	bool compressed = false, isone = false;
	int index = 0;
	while (1) {
		string temp = str.substr(index, runlengthcode + 2);
		compressed = str[index] == '1' ? true : false;
		if (compressed) {
			isone = str[index+1] == '1' ? true : false;
			int count = str_to_int(temp.substr(2))+1;
			if (isone) {
				for (int i = 0; i < count; i++) {
					ans += '1';
				}
			}
			else {
				for (int i = 0; i < count; i++) {
					ans += '0';
				}
			}
		}
		else {
			ans += temp.substr(2);
		}
		index += (runlengthcode+2);
		if (ans.size() >= limit) {
			ans.substr(0, limit);
			str = str.substr(index);
			break;
		}
	}
	return ans;
}


int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "请输入图片路径和提取方式" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	if (argv[2][1] == 'h') {
		usingHis(image);
		cout << "成功使用直方图提取"<<endl;
		return 0;
	}
	else if (argv[2][1] == 'd') {
		usingDE(image);
		cout << "成功使用差分扩展提取" << endl;
		return 0;
	}
	else {
		cout << "请输入正确参数";
		return -1;
	}
	return -1;

}

void usingHis(Mat image) {
	addInfoHis addinfo(image.rows, image.cols);
	pair<int, int> firstpair = make_pair(image.at<uchar>(0, 0), image.at<uchar>(0, 1));
	image.at<uchar>(0, 0) = 0; image.at<uchar>(0, 1) = 0;


	addinfo.pairs.push_back(firstpair);
	string embedInfo;

	int times = 0;
	pair<int, int> thispair = firstpair;
	bool isIni = false;
	if (firstpair.first > firstpair.second) {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			else if (image.at<uchar>(0, j) == firstpair.first - 1) {
				embedInfo += '1';
				image.at<uchar>(0, j)++;
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}

		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
					if (embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
				else if (image.at<uchar>(i, j) == firstpair.first - 1) {
					embedInfo += '1';
					image.at<uchar>(i, j)++;
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}

	}
	else {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			else if (image.at<uchar>(0, j) == firstpair.first + 1) {
				embedInfo += '1';
				image.at<uchar>(0, j)--;
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
				else if (image.at<uchar>(i, j) == firstpair.first + 1) {
					embedInfo += '1';
					image.at<uchar>(i, j)--;
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}

			}
		}

	}





	//提取全部的信息（如果提取了第一对点的信息就完成了全部信息的提取，那么以下代码就不会执行）
	while (1) {
		if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
			goto here;
		}
		times++;
		string max = embedInfo.substr(56 + 16 * times, 8);
		string min = embedInfo.substr(56 + 16 * times + 8, 8);
		int thisPairMax = 0, thisPairMin = 0;
		for (int i = 0, mask = 1; i < 8; i++) {
			if (max[7 - i] == '1') {
				thisPairMax += mask;
			}
			if (min[7 - i] == '1') {
				thisPairMin += mask;
			}
			mask <<= 1;
		}

		thispair.first = thisPairMax;
		thispair.second = thisPairMin;
		addinfo.pairs.push_back(thispair);
		if (thispair.first > thispair.second) {
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(0, j) == thispair.first - 1) {
					embedInfo += '1';
					image.at<uchar>(0, j)++;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						embedInfo += '0';
					}
					else if (image.at<uchar>(i, j) == thispair.first - 1) {
						embedInfo += '1';
						image.at<uchar>(i, j)++;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}
		else {
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(0, j) == thispair.first + 1) {
					embedInfo += '1';
					image.at<uchar>(0, j)--;
				}

				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}

			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						embedInfo += '0';
					}
					else if (image.at<uchar>(i, j) == thispair.first + 1) {
						embedInfo += '1';
						image.at<uchar>(i, j)--;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}

	}

here:
	ofstream outfile("out.txt");
	outfile << embedInfo.substr(addinfo.totalAddLength);
	//cout << "嵌入的信息为：" << embedInfo.substr(addinfo.totalAddLength) << endl;

	//构造附加信息类的实例
	int row = 0;
	int col = 0;
	for (int i = 1; i < row; i <<= 1) {
		row++;
	}
	for (int i = 1; i < col; i <<= 1) {
		col++;
	}
	times++;
	int timeFor_bitmapMinLength = 0;
	int timeFor_bitmap1 = 0;
	int temp_lengthForBitmapMinLength = addinfo.lengthForBitmapMinLength;

	while (temp_lengthForBitmapMinLength != 0) {
		string bitmapMinLength = embedInfo.substr(56 + 16 * times + 8 * timeFor_bitmapMinLength, 8);
		int thisbitmapMinLength = 0;
		for (int i = 7, mask = 1; i >= 0; i--) {
			if (bitmapMinLength[7 - i] == '1') {
				thisbitmapMinLength += mask;
			}
			mask <<= 1;
		}
		addinfo.bitmapMinLength.push_back(thisbitmapMinLength);
		temp_lengthForBitmapMinLength--;
		timeFor_bitmapMinLength++;
		continue;
	}

	for (int i = 0; i < addinfo.bitmapMinLength.size(); i++) {
		int temp = addinfo.bitmapMinLength[i];
		string bitmap1 = embedInfo.substr(56 + 16 * times + 8 * timeFor_bitmapMinLength + (row + col) * timeFor_bitmap1, (row + col));
		string rows = bitmap1.substr(0, row), cols = bitmap1.substr(row);
		int thisrow = 0, thiscol = 0;
		for (int rowsIndex = rows.length() - 1, mask = 1; rowsIndex >= 0; rowsIndex--) {
			if (rows[rowsIndex] == '1') {
				thisrow += mask;
			}
			mask <<= 1;
		}
		for (int colsIndex = cols.length() - 1, mask = 1; colsIndex >= 0; colsIndex--) {
			if (cols[colsIndex] == '1') {
				thisrow += mask;
			}
			mask <<= 1;
		}
		addinfo.bitmap1.push_back(make_pair(thisrow, thiscol));
	}

	//进行图像复原

	for (int pairIndex = 0; pairIndex < addinfo.pairs.size(); pairIndex++) {
		pair<int, int>thispair = addinfo.pairs[pairIndex];
		if (thispair.first > thispair.second) {

			//第一趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) >= thispair.second && image.at<uchar>(0, j) < thispair.first) {
					image.at<uchar>(0, j)++;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) >= thispair.second && image.at<uchar>(i, j) < thispair.first) {
						image.at<uchar>(i, j)++;
					}
				}
			}
		}
		else {
			//第一趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) <= thispair.second && image.at<uchar>(0, j) > thispair.first) {
					image.at<uchar>(0, j)--;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) <= thispair.second && image.at<uchar>(i, j) > thispair.first) {
						image.at<uchar>(i, j)--;
					}
				}
			}
		}

		int count = 0;
		for (int i = 0; i < addinfo.bitmapMinLength.size(); i++) {
			for (int j = 0; j < addinfo.bitmapMinLength[i]; j++) {
				image.at<uchar>(addinfo.bitmap1[count + j].first, addinfo.bitmap1[count + j].second) = thispair.second;
			}
			count += i;
		}
	}

	image.at<uchar>(0, 0) = addinfo.imageGrayscaleAt_0_0;
	image.at<uchar>(0, 1) = addinfo.imageGrayscaleAt_0_1;

there:
	imwrite("output.bmp", image);

}

void usingDE(Mat image)
{
	string allEmbed;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j += 2) {
			int mean = (image.at<uchar>(i, j) + image.at<uchar>(i, j + 1)) >> 1;
			int dif = image.at<uchar>(i, j) - image.at<uchar>(i, j + 1);
			if (isChangeable(mean, dif)) {
				allEmbed += to_string(dif & 1);
				//cout << i << ' ' << j << ' ' << dif << endl;
			}
		}
	}

	int allLength = str_to_int(allEmbed.substr(0, 32));
	int offset = str_to_int(allEmbed.substr(32, 32));
	string realEmbed = allEmbed.substr(64);

	string bitmap = recoverBitmap(realEmbed, image.cols * image.rows / 2);
	int LSBcount = 0;
	for (int i = 0; i < bitmap.size(); i++) {
		if (bitmap[i] == '0') {
			LSBcount++;
		}
	}

	string LSB = realEmbed.substr(0,LSBcount);
	string embed = realEmbed.substr(LSBcount, offset);

	int indexforLSB = 0;
	int countforAllLength = 0;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j += 2) {
			if (i == 67 && j == 156){
				int a=0;
			}
			int mean = (image.at<uchar>(i, j) + image.at<uchar>(i, j + 1)) >> 1;
			int dif = image.at<uchar>(i, j) - image.at<uchar>(i, j + 1);
			if (isChangeable(mean, dif)) {
				int olddif;
				if (bitmap[(i * image.rows + j)/2] == '1') {
					olddif = dif >>1;
				}
				else {
					if (dif >= 0 && dif <= 1) {
						olddif = 1;
					}
					else if (dif >= -2 && dif <= -1) {
						olddif = -2;
					} 
					else {
						olddif = 2 * (dif>>1) + (LSB[indexforLSB] == '1' ? 1 : 0);
						indexforLSB++;
					}

				}
				int oldx = 0, oldy = 0;
				if (olddif > 0) {
					if ((olddif & 1) == 0) {
						oldx = mean + (olddif + 1) / 2;
						oldy = mean - (olddif) / 2;
					}
					else {
						oldx = mean + (olddif + 1) / 2;
						oldy = mean - (olddif) / 2;
					}
				}
				else {
					if ((olddif & 1) == 0) {
						oldx = mean + (olddif) / 2;
						oldy = mean - (olddif) / 2;
					}
					else {
						oldx = mean + (olddif) / 2;
						oldy = mean - (olddif-1) / 2;
					}
				}
				image.at<uchar>(i, j) = oldx; image.at<uchar>(i, j + 1) = oldy;
				countforAllLength++;
				if (countforAllLength == allLength) {
					goto there1;
				}
			}
			
		}
	}

there1:
	imwrite("output.bmp", image);
	ofstream outfile("out.txt");
	outfile << embed;
}

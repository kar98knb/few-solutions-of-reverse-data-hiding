#include<iostream>
#include"bmp.h"
#include <direct.h>


BitMapFileHeader strHead;
RGBQUAD strPla[256];//256色调色板
BitMapInfoHeader strInfo;

int main() {
	char* currentPath = _getcwd(NULL, 256);
	string resPath = currentPath;
	for (size_t i = resPath.length() - 1; i > 0; i--) {
		if (resPath[i] != '\\') { continue; }
		else { 
			resPath = resPath.substr(0,i); 
			resPath += "\\res";
			break;
		}
	}

	string testPath = resPath + "\\lena.bmp";

	int width, height;//图片的宽度和高度
	const char* c_s = testPath.c_str();
	FILE* fpi = fopen(c_s, "rb"); //请一定指定参数为'rb'，在'r'模式下读调色板的时候会发生文件内指针跳转错误

	WORD bfType;
	fread(&bfType, 1, sizeof(WORD), fpi);
	if (0x4d42 != bfType)
	{
		cout << "the file is not a bmp file!" << endl;
		return NULL;
	}


	//读取bmp文件的文件头和信息头
	fread(&strHead, 1, sizeof(tagBitMapFileHeader), fpi);
	fread(&strInfo, 1, sizeof(tagBitMapInfoHeader), fpi);

	for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++)
	{
		fread((char*)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
		fread((char*)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
		fread((char*)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
		fread((char*)&(strPla[nCounti].rgbReserved), 1, sizeof(BYTE), fpi);
	}

	width = strInfo.biWidth;
	height = strInfo.biHeight;
	//图像每一行的字节数必须是4的整数倍
	width = (width * sizeof(ImageData) + 3) / 4 * 4;
	ImageData* imagedata = (ImageData*)malloc(sizeof(ImageData) * width * height);
	if (imagedata == NULL) { return -1; }
	else {
		//读出图片的像素数据
		fread(imagedata, sizeof(ImageData) * width, height, fpi);
		fclose(fpi);
	}
	

	//TODO:编码



	return 0;
}
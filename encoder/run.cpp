#include<iostream>
#include"bmp.h"
#include <direct.h>


BitMapFileHeader strHead;
RGBQUAD strPla[256];//256ɫ��ɫ��
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

	int width, height;//ͼƬ�Ŀ�Ⱥ͸߶�
	const char* c_s = testPath.c_str();
	FILE* fpi = fopen(c_s, "rb"); //��һ��ָ������Ϊ'rb'����'r'ģʽ�¶���ɫ���ʱ��ᷢ���ļ���ָ����ת����

	WORD bfType;
	fread(&bfType, 1, sizeof(WORD), fpi);
	if (0x4d42 != bfType)
	{
		cout << "the file is not a bmp file!" << endl;
		return NULL;
	}


	//��ȡbmp�ļ����ļ�ͷ����Ϣͷ
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
	//ͼ��ÿһ�е��ֽ���������4��������
	width = (width * sizeof(ImageData) + 3) / 4 * 4;
	ImageData* imagedata = (ImageData*)malloc(sizeof(ImageData) * width * height);
	if (imagedata == NULL) { return -1; }
	else {
		//����ͼƬ����������
		fread(imagedata, sizeof(ImageData) * width, height, fpi);
		fclose(fpi);
	}
	

	//TODO:����



	return 0;
}
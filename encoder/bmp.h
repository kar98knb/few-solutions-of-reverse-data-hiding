#pragma once
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

using namespace std;

//λͼ�ļ�ͷ����;
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������
//Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct  tagBitMapFileHeader {
	//WORD bfType;//�ļ����ͣ�������0x424D�����ַ���BM��
	DWORD bfSize;//�ļ���С
	WORD bfReserved1;//������
	WORD bfReserved2;//������
	DWORD bfOffBits;//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}BitMapFileHeader;

typedef struct tagBitMapInfoHeader {
	DWORD biSize;//��Ϣͷ��С
	LONG biWidth;//ͼ����
	LONG biHeight;//ͼ��߶�
	WORD biPlanes;//λƽ����������Ϊ1
	WORD biBitCount;//ÿ����λ��
	DWORD  biCompression; //ѹ������
	DWORD  biSizeImage; //ѹ��ͼ���С�ֽ���
	LONG  biXPelsPerMeter; //ˮƽ�ֱ���
	LONG  biYPelsPerMeter; //��ֱ�ֱ���
	DWORD  biClrUsed; //λͼʵ���õ���ɫ����
	DWORD  biClrImportant; //��λͼ����Ҫ��ɫ����
}BitMapInfoHeader; //λͼ��Ϣͷ����

typedef struct tagRGBQUAD {
	BYTE rgbBlue; //����ɫ����ɫ����
	BYTE rgbGreen; //����ɫ����ɫ����
	BYTE rgbRed; //����ɫ�ĺ�ɫ����
	BYTE rgbReserved; //����ֵ
}RGBQUAD;//��ɫ�嶨��

//������Ϣ
typedef struct tagImageData
{
	BYTE blue;
	//BYTE green;
	//BYTE red;
}ImageData;

//void showBmpHead(BitMapFileHeader pBmpHead) {
//	cout << "λͼ�ļ�ͷ:" << endl;
//	cout << "�ļ���С:" << pBmpHead.bfSize << endl;
//	cout << "������_1:" << pBmpHead.bfReserved1 << endl;
//	cout << "������_2:" << pBmpHead.bfReserved2 << endl;
//	cout << "ʵ��λͼ���ݵ�ƫ���ֽ���:" << pBmpHead.bfOffBits << endl << endl;
//}

//void showBmpInforHead(BitMapInfoHeader pBmpInforHead) {
//	cout << "λͼ��Ϣͷ:" << endl;
//	cout << "�ṹ��ĳ���:" << pBmpInforHead.biSize << endl;
//	cout << "λͼ��:" << pBmpInforHead.biWidth << endl;
//	cout << "λͼ��:" << pBmpInforHead.biHeight << endl;
//	cout << "biPlanesƽ����:" << pBmpInforHead.biPlanes << endl;
//	cout << "biBitCount������ɫλ��:" << pBmpInforHead.biBitCount << endl;
//	cout << "ѹ����ʽ:" << pBmpInforHead.biCompression << endl;
//	cout << "biSizeImageʵ��λͼ����ռ�õ��ֽ���:" << pBmpInforHead.biSizeImage << endl;
//	cout << "X����ֱ���:" << pBmpInforHead.biXPelsPerMeter << endl;
//	cout << "Y����ֱ���:" << pBmpInforHead.biYPelsPerMeter << endl;
//	cout << "ʹ�õ���ɫ��:" << pBmpInforHead.biClrUsed << endl;
//	cout << "��Ҫ��ɫ��:" << pBmpInforHead.biClrImportant << endl;
//}


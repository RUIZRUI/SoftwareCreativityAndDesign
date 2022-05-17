#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#pragma pack(1)
#pragma warning(disable:4996)
typedef struct tagBITMAPFILEHEADER
{
	unsigned char  bfType[2];//�ļ���ʽ
	unsigned int   bfSize;        // �ļ���С ���ֽ�Ϊ��λ(2-5�ֽ�)
	unsigned short bfReserved1;   // ��������������Ϊ0 (6-7�ֽ�)
	unsigned short bfReserved2;   // ��������������Ϊ0 (8-9�ֽ�)
	unsigned int   bfOffBits;     // ���ļ�ͷ���������ݵ�ƫ��  (10-13�ֽ�)
}BITMAPFILEHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
	unsigned int    biSize;          // �˽ṹ��Ĵ�С (14-17�ֽ�)
	long            biWidth;         // ͼ��Ŀ�  (18-21�ֽ�)
	long            biHeight;        // ͼ��ĸ�  (22-25�ֽ�)
	unsigned short  biPlanes;        // ��ʾbmpͼƬ��ƽ��������Ȼ��ʾ��ֻ��һ��ƽ�棬���Ժ����1 (26-27�ֽ�)
	unsigned short  biBitCount;      // һ������ռ��λ����(28-29�ֽ�)��biBitCount=24ʱ����BMPͼ�����24Bit���ͼ��û�е�ɫ���
	unsigned int    biCompression;   // ˵��ͼ������ѹ�������ͣ�0Ϊ��ѹ���� (30-33�ֽ�)
	unsigned int    biSizeImage;     // ����������ռ��С, ���ֵӦ�õ��������ļ�ͷ�ṹ��bfSize-bfOffBits (34-37�ֽ�)
	long            biXPelsPerMeter; // ˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (38-41�ֽ�)
	long            biYPelsPerMeter; // ˵����ֱ�ֱ��ʣ�������/�ױ�ʾ��һ��Ϊ0 (42-45�ֽ�)
	unsigned int    biClrUsed;       // ˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ����� (46-49�ֽ�)
	unsigned int    biClrImportant;  // ˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��(50-53�ֽ�)
}BITMAPINFOHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;   //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
	unsigned char rgbGreen;  //����ɫ����ɫ����  (ֵ��ΧΪ0-255)
	unsigned char rgbRed;    //����ɫ�ĺ�ɫ����  (ֵ��ΧΪ0-255)
	unsigned char rgbReserved;// ����������Ϊ0
}RGBQUAD;
#pragma pack()

//rotate 90��
void rotation90()
{
	unsigned char ImgData[3000][3];
	unsigned char ImgData2[3000];
	unsigned char ImgData3[600][600][3]; 
	int i, j, k;
	long lTmp;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;

	if ((fpBMP = fopen("./pictest/1.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/rotate90.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);

	lTmp = infoHeader->biHeight;
	infoHeader->biHeight = infoHeader->biWidth;
	infoHeader->biWidth = lTmp;

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);

	for (i = 0; i < infoHeader->biWidth; i++)
	{
		k = 0;
		for (j = 0; j < ((infoHeader->biHeight) * 3 + 3) / 4 * 4; j++)
		{
			fread(&ImgData[j / 3][k], 1, 1, fpBMP);
			ImgData3[i][j / 3][k] = ImgData[j / 3][k];
			k = (++k) % 3;
		}
	}
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		k = 0;
		for (j =((infoHeader->biWidth) * 3 + 3) / 4 * 4; j >0 ; j--)
		{
			fwrite(&ImgData3[infoHeader->biWidth - 1 - j / 3][i][k], 1, 1, fpGray);
			k = (++k) % 3;
		}
	}

	free(fileHeader);
	free(infoHeader);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmpͼƬ��ת���\n");
}

//�ҶȻ�
void gray()
{
	unsigned char ImgData[3000][3];	
	unsigned char ImgData2[3000];
	int i, j, k;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;

	if ((fpBMP = fopen("./pictest/rotate90.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/gray.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);

	infoHeader->biBitCount = 8;
	infoHeader->biSizeImage = ((infoHeader->biWidth * 1 + 3) / 4) * 4 * infoHeader->biHeight;

	fileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	fileHeader->bfSize = fileHeader->bfOffBits + infoHeader->biSizeImage;

	ipRGB = (RGBQUAD*)malloc(256 * sizeof(RGBQUAD));
	for (i = 0; i < 256; i++)
	{
		ipRGB[i].rgbBlue = ipRGB[i].rgbGreen = ipRGB[i].rgbRed = i;
	}

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);
	fwrite(ipRGB, sizeof(RGBQUAD), 256, fpGray);

	k = 0;
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 3 + 3) / 4 * 4; j++)
		{
			fread(&ImgData[j / 3][k], 1, 1, fpBMP);
			k = (++k) % 3;
		}
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			ImgData2[j] = (int)((float)ImgData[j][0] * 0.114 +
				(float)ImgData[j][1] * 0.587 +
				(float)ImgData[j][2] * 0.299);

		}
		fwrite(ImgData2, j, 1, fpGray);
	}
	free(fileHeader);
	free(infoHeader);
	free(ipRGB);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmpͼƬ�ҶȻ����\n");
}

//ͼ�����ؿ黥��
void exchange13()
{
	unsigned char ImgData[3000][3];
	unsigned char ImgData2[3000];
	unsigned char ImgData3[600][600][3]; 
	int i, j, k;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;

	if ((fpBMP = fopen("./pictest/1.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/exchange.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));

	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);

	for (i = 0; i < infoHeader->biHeight; i++)
	{
		k = 0;
		for (j = 0; j < ((infoHeader->biWidth) * 3 + 3) / 4 * 4; j++)
		{
			fread(&ImgData[j / 3][k], 1, 1, fpBMP);
			ImgData3[i][j / 3][k] = ImgData[j / 3][k];
			k = (++k) % 3;
		}
	}
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		k = 0;
		for (j = 0; j < ((infoHeader->biWidth) * 3 + 3) / 4 * 4; j++)
		{
			//��һ��
			if (i >= 0 && i < (infoHeader->biHeight / 2) && j >= 0 && j <= (((infoHeader->biWidth) * 3 + 3) / 4 * 2))
			{
				fwrite(&ImgData3[i + (infoHeader->biHeight / 2)][j / 3][k], 1, 1, fpGray);
			}
			//������
			else if (i > (infoHeader->biHeight / 2) && i < (infoHeader->biHeight) && j >= 0 && j <= (((infoHeader->biWidth) * 3 + 3) / 4 * 2))
			{
				fwrite(&ImgData3[i - (infoHeader->biHeight / 2)][j / 3][k], 1, 1, fpGray);
			}
			else
				fwrite(&ImgData3[i][j / 3][k], 1, 1, fpGray);
			k = (++k) % 3;
		}
	}

	free(fileHeader);
	free(infoHeader);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmpͼƬת�����\n");
}

//��ɫͼ���ֵ��
void binary()
{
	unsigned char ImgData[3000][3];	
	unsigned char ImgData2[3000];
	unsigned char ImgTmp[3000];
	int i, j, k;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;
	RGBQUAD* ipRGBtmp;

	if ((fpBMP = fopen("./pictest/gray.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/graybinary.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	ipRGB = (RGBQUAD*)malloc(2 * sizeof(RGBQUAD));
	ipRGBtmp = (RGBQUAD*)malloc(256 * sizeof(RGBQUAD));

	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);
	fread(ipRGBtmp, sizeof(RGBQUAD), 256, fpBMP);

	infoHeader->biBitCount = 8;
	infoHeader->biSizeImage = ((infoHeader->biWidth * 1 + 3) / 4) * 4 * infoHeader->biHeight;
	infoHeader->biClrUsed = 2;
	infoHeader->biClrImportant = 0;

	fileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD);
	fileHeader->bfSize = fileHeader->bfOffBits + infoHeader->biSizeImage;
	ipRGB[1].rgbBlue = ipRGB[1].rgbGreen = ipRGB[1].rgbRed = ipRGB[1].rgbReserved = 0;
	ipRGB[0].rgbBlue = ipRGB[0].rgbGreen = ipRGB[0].rgbRed = 190;
	ipRGB[1].rgbReserved = 0;

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);
	fwrite(ipRGB, sizeof(RGBQUAD), 2, fpGray);

	k = 0;
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			fread(&ImgTmp[j], 1, 1, fpBMP);
			if (ImgTmp[j] > 180)
				ImgTmp[j] = 0;
			else
				ImgTmp[j] = 1;
		}
		fwrite(ImgTmp, j, 1, fpGray);
	}
	free(fileHeader);
	free(infoHeader);
	free(ipRGB);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmp��ɫͼƬ��ֵ�����\n");
}

//sobel
void sobel()
{
	unsigned char ImgData[3000][3];
	unsigned char ImgData2[3000];
	unsigned char ImgData3[600][600];
	unsigned char ImgTmp[3000];
	int iGX = 0;
	int iGY = 0;
	int i, j;
	unsigned char k = 0;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;
	RGBQUAD* ipRGBtmp;

	if ((fpBMP = fopen("./pictest/gray.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/graysobel.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	ipRGB = (RGBQUAD*)malloc(2 * sizeof(RGBQUAD));
	ipRGBtmp = (RGBQUAD*)malloc(256 * sizeof(RGBQUAD));

	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);
	fread(ipRGBtmp, sizeof(RGBQUAD), 256, fpBMP);

	infoHeader->biBitCount = 8;
	infoHeader->biSizeImage = ((infoHeader->biWidth * 1 + 3) / 4) * 4 * infoHeader->biHeight;
	infoHeader->biClrUsed = 2;
	infoHeader->biClrImportant = 0;

	fileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD);
	fileHeader->bfSize = fileHeader->bfOffBits + infoHeader->biSizeImage;
	ipRGB[1].rgbBlue = ipRGB[1].rgbGreen = ipRGB[1].rgbRed = ipRGB[1].rgbReserved = 0;
	ipRGB[0].rgbBlue = ipRGB[0].rgbGreen = ipRGB[0].rgbRed = 150;
	ipRGB[1].rgbReserved = 0;

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);
	fwrite(ipRGB, sizeof(RGBQUAD), 2, fpGray);

	k = 0;
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			fread(&ImgData3[i][j], 1, 1, fpBMP);
		}
	}
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			iGX = (ImgData3[i + 1][j - 1] + 2 * ImgData3[i + 1][j] + ImgData3[i + 1][j + 1]) - (ImgData3[i - 1][j - 1] + 2 * ImgData3[i - 1][j] + ImgData3[i - 1][j + 1]);
			iGY = (ImgData3[i - 1][j - 1] + 2 * ImgData3[i][j - 1] + ImgData3[i + 1][j - 1]) - (ImgData3[i - 1][j + 1] + 2 * ImgData3[i][j + 1] + ImgData3[i + 1][j + 1]);
			if (iGY * iGY + iGX * iGX > 150 * 150)
				k = 1;
			else
				k = 0;
			fwrite(&k, 1, 1, fpGray);
		}
	}

	free(fileHeader);
	free(infoHeader);
	free(ipRGB);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmp��ɫͼƬsobel��Ե�����\n");
}

//Gaussianƽ��
void Gaussian()
{
	unsigned char ImgData[3000][3];
	unsigned char ImgData2[3000];
	unsigned char ImgData3[600][600];  
	unsigned char ImgTmp[3000];
	double tem[9] = { 1.0,2.0,1.0,2.0,4.0,2.0,1.0,2.0,1.0 };
	int iGX = 0;
	int iGY = 0;
	int i, j, k;
	double sum;
	FILE* fpBMP, * fpGray;
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* ipRGB;
	RGBQUAD* ipRGBtmp;

	if ((fpBMP = fopen("./pictest/gray.bmp", "rb")) == NULL)
	{
		printf("��ͼƬʧ��");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/grayGaussian.bmp", "wb")) == NULL)
	{
		printf("����ͼƬʧ��");
		exit(0);
	}

	fileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	ipRGB = (RGBQUAD*)malloc(2 * sizeof(RGBQUAD));
	ipRGBtmp = (RGBQUAD*)malloc(256 * sizeof(RGBQUAD));
	
	fread(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBMP);
	fread(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBMP);
	fread(ipRGBtmp, sizeof(RGBQUAD), 256, fpBMP);

	infoHeader->biBitCount = 8;
	infoHeader->biSizeImage = ((infoHeader->biWidth * 1 + 3) / 4) * 4 * infoHeader->biHeight;
	
	fileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	fileHeader->bfSize = fileHeader->bfOffBits + infoHeader->biSizeImage;

	fwrite(fileHeader, sizeof(BITMAPFILEHEADER), 1, fpGray);
	fwrite(infoHeader, sizeof(BITMAPINFOHEADER), 1, fpGray);
	fwrite(ipRGBtmp, sizeof(RGBQUAD), 256, fpGray);
	k = 0;
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			fread(&ImgData3[i][j], 1, 1, fpBMP);
		}
	}
	for (i = 0; i < infoHeader->biHeight; i++)
	{
		for (j = 0; j < ((infoHeader->biWidth) * 1 + 3) / 4 * 4; j++)
		{
			sum = 0;
			sum += (double)(ImgData3[i - 1][j - 1]) * tem[0];   
			sum += (double)(ImgData3[i - 1][j]) * tem[1];        
			sum += (double)(ImgData3[i - 1][j + 1]) * tem[2];  
			sum += (double)(ImgData3[i][j - 1]) * tem[3];   
			sum += (double)(ImgData3[i][j]) * tem[4];   
			sum += (double)(ImgData3[i][j + 1]) * tem[5];  
			sum += (double)(ImgData3[i + 1][j - 1]) * tem[6];    
			sum += (double)(ImgData3[i + 1][j]) * tem[7];   
			sum += (double)(ImgData3[i + 1][j + 1]) * tem[8];  
			k = (unsigned char)(sum / 16.0);
			fwrite(&k, 1, 1, fpGray);
		}
	}

	free(fileHeader);
	free(infoHeader);
	free(ipRGB);
	fclose(fpBMP);
	fclose(fpGray);
	printf("bmp��ɫͼƬGaussianƽ�����\n\n\n");
}

int main()
{
	rotation90();
	gray();
	exchange13();
	binary();
	sobel();
	Gaussian();
	return 0;
}



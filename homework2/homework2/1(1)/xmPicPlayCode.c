#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#pragma pack(1)
#pragma warning(disable:4996)
typedef struct tagBITMAPFILEHEADER
{
	unsigned char  bfType[2];//文件格式
	unsigned int   bfSize;        // 文件大小 以字节为单位(2-5字节)
	unsigned short bfReserved1;   // 保留，必须设置为0 (6-7字节)
	unsigned short bfReserved2;   // 保留，必须设置为0 (8-9字节)
	unsigned int   bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
}BITMAPFILEHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
	unsigned int    biSize;          // 此结构体的大小 (14-17字节)
	long            biWidth;         // 图像的宽  (18-21字节)
	long            biHeight;        // 图像的高  (22-25字节)
	unsigned short  biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
	unsigned short  biBitCount;      // 一像素所占的位数，(28-29字节)当biBitCount=24时，该BMP图像就是24Bit真彩图，没有调色板项。
	unsigned int    biCompression;   // 说明图象数据压缩的类型，0为不压缩。 (30-33字节)
	unsigned int    biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits (34-37字节)
	long            biXPelsPerMeter; // 说明水平分辨率，用象素/米表示。一般为0 (38-41字节)
	long            biYPelsPerMeter; // 说明垂直分辨率，用象素/米表示。一般为0 (42-45字节)
	unsigned int    biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
	unsigned int    biClrImportant;  // 说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
}BITMAPINFOHEADER;
#pragma pack()

#pragma pack(1)
typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;   //该颜色的蓝色分量  (值范围为0-255)
	unsigned char rgbGreen;  //该颜色的绿色分量  (值范围为0-255)
	unsigned char rgbRed;    //该颜色的红色分量  (值范围为0-255)
	unsigned char rgbReserved;// 保留，必须为0
}RGBQUAD;
#pragma pack()

//rotate 90°
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/rotate90.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
	printf("bmp图片旋转完成\n");
}

//灰度化
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/gray.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
	printf("bmp图片灰度化完成\n");
}

//图像像素块互换
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/exchange.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
			//第一块
			if (i >= 0 && i < (infoHeader->biHeight / 2) && j >= 0 && j <= (((infoHeader->biWidth) * 3 + 3) / 4 * 2))
			{
				fwrite(&ImgData3[i + (infoHeader->biHeight / 2)][j / 3][k], 1, 1, fpGray);
			}
			//第三块
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
	printf("bmp图片转块完成\n");
}

//灰色图像二值化
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/graybinary.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
	printf("bmp灰色图片二值化完成\n");
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/graysobel.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
	printf("bmp灰色图片sobel边缘化完成\n");
}

//Gaussian平滑
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
		printf("打开图片失败");
		exit(0);
	}

	if ((fpGray = fopen("./pictest/grayGaussian.bmp", "wb")) == NULL)
	{
		printf("创建图片失败");
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
	printf("bmp灰色图片Gaussian平滑完成\n\n\n");
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



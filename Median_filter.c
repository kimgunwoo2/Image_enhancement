#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char
void DesSort(int *arr)
{
	int temp;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				temp = arr[j];	//arr[j]와 arr[j+1]을 바꾸는 부분
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}

		}
	}
}


void bmpBWrw()
{
	int histogram[256] = { 0, };
	FILE *infile;
	if ((infile = fopen("Lena.bmp", "rb")) == NULL) {
		printf("No Image File g");
		return;
	}
	srand(time(NULL));
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);


	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }
	RGBQUAD hRGB[256];


	fread(hRGB, sizeof(RGBQUAD), 256, infile);
	BYTE *lpImg = malloc(hInfo.biSizeImage);
	BYTE *outImg = malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);


	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int x, y, SorP;
	for (int i = 0; i < hInfo.biWidth*hInfo.biHeight*0.05; i++)
	{
		x = rand() % hInfo.biWidth;
		y = rand() % hInfo.biWidth;
		SorP = rand() % 2;
		if (SorP == 1)
			lpImg[y*rwsize + x] = 255;
		else
			lpImg[y*rwsize + x] = 0;
	}

	FILE *outfile = fopen("result1.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);

	int mean[3] = { 0, };
	for (int i = 0; i<hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (j == 0) {
				mean[0] = 0;
				mean[1] = lpImg[i*rwsize + (j)];
				mean[2] = lpImg[i*rwsize + (j + 1)];
			}
			else if (j == hInfo.biWidth - 1) {
				mean[0] = lpImg[i*rwsize + (j - 1)];
				mean[1] = lpImg[i*rwsize + (j)];
				mean[2] = 0;

			}
			else {
				for (int k = -1; k < 2; k++)
				{

					mean[k + 1] = lpImg[i*rwsize + (j + k)];

				}
			}
			DesSort(mean);
			outImg[i*rwsize + j] = mean[1];
		}

	FILE *outfile1 = fopen("result2.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile1);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile1);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile1);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile1);
	fclose(outfile1);
	for (int i = 0; i<hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (i == 0) {
				mean[0] = 0;
				mean[1] = outImg[i*rwsize + j];
				mean[2] = outImg[(i+1)*rwsize + j ];
			}
			else if (i == hInfo.biHeight - 1) {
				mean[0] = outImg[(i-1)*rwsize + j ];
				mean[1] = outImg[i*rwsize + j];
				mean[2] = 0;

			}
			else {
				for (int k = -1; k < 2; k++)
				{

					mean[k + 1] = outImg[(i+k)*rwsize + j];

				}
			}
			DesSort(mean);
			outImg[i*rwsize + j] = mean[1];
		}
	FILE *outfile2 = fopen("result3.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile2);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile2);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile2);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile2);
	fclose(outfile2);
	free(outImg);
	free(lpImg);
}

void main()
{

	bmpBWrw();
}

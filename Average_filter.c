#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char

void bmpBWrw()
{
	int histogram[256] = { 0, };
	FILE *infile;
	int sum_histogram[256] = { 0, };
	if ((infile = fopen("Snow.bmp", "rb")) == NULL) {
		printf("No Image File g");
		return;
	}
	int sum = 0;
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);


	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }
	RGBQUAD hRGB[256];


	fread(hRGB, sizeof(RGBQUAD), 256, infile);
	BYTE *lpImg = malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	BYTE *outImg = malloc(hInfo.biSizeImage);
	float windows[21][21];
	for (int i = 0; i < 21; i++)
		for (int j = 0; j < 21; j++) {
			windows[i][j] = (float)1.0 / (21 * 21);
		}
	float tmp;
	for (int i = 10; i < hInfo.biHeight - 11; i++)
		for (int j = 10; j < hInfo.biWidth - 11; j++) {
			tmp = 0.;
			for (int k = -10; k <= 10; k++)
				for (int l = -10; l <= 10; l++) {
					tmp += windows[k + 10][l + 10] * lpImg[(i + k)*rwsize + (j + l)];
				}
			outImg[i *rwsize + j] = (int)(tmp + 0.5);
		}



	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
}

void main()
{

	bmpBWrw();
}

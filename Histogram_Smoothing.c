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
	if ((infile = fopen("chest.bmp", "rb")) == NULL) {
		printf("No Image File ");
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
	for (int i = 0; i<hInfo.biHeight; i++) {
		for (int j = 0; j<hInfo.biWidth; j++)
			histogram[lpImg[i*rwsize + j]]++;
	}
	for (int i = 0; i < 256; i++)
	{
		sum = histogram[i] + sum;
		sum_histogram[i] = (int)((sum*(255.0 / (hInfo.biHeight*hInfo.biWidth))) + 0.5);
	}
	for (int i = 0; i<hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++)
			lpImg[i*rwsize + j] = sum_histogram[lpImg[i*rwsize + j]];
	}
	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);

	free(lpImg);
}

void main()
{

	bmpBWrw();
}

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

	FILE *infile;
	if ((infile = fopen("Snow.bmp", "rb")) == NULL) {
		printf("No Image File g");
		return;
	}
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);


	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }
	RGBQUAD hRGB[256];


	fread(hRGB, sizeof(RGBQUAD), 256, infile);
	BYTE *lpImg = malloc(hInfo.biSizeImage);
	BYTE *outImg1 = malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	float *outImg = (float*)malloc(sizeof(float)*hInfo.biSizeImage);
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (j == 0)
				outImg[i*rwsize + j] = lpImg[i*rwsize + j];
			else
				outImg[i*rwsize + j] = outImg[i*rwsize + (j - 1)] + lpImg[i*rwsize + j];
		}
	}

	for (int j = 0; j < hInfo.biWidth; j++) {
		for (int i = 1; i < hInfo.biHeight; i++) {
			outImg[i*rwsize + j] += outImg[(i - 1)*rwsize + j];
		}
	}


	float windows= (float)1.0 / (21 * 21);
	
	for (int i = 10; i < hInfo.biHeight - 11; i++) {
		for (int j = 10; j < hInfo.biWidth - 11; j++) {
			if (i == 10 && j == 10)
				outImg1[i*rwsize + j] = (int)(windows * outImg[(i + 10)*rwsize + (j + 10)]);
			else if (i == 10)
				outImg1[i*rwsize + j] = (int)(windows * (outImg[(i + 10)*rwsize + (j + 10)] - outImg[(i + 10)*rwsize + (j - 11)]));
			else if (j == 10)
				outImg1[i*rwsize + j] = (int)(windows * (outImg[(i + 10)*rwsize + (j + 10)] - outImg[(i - 11)*rwsize + (j + 10)]));
			else
				outImg1[i*rwsize + j] = (int)(windows * (outImg[(i + 10)*rwsize + (j + 10)] - outImg[(i - 11)*rwsize + (j + 10)] - outImg[(i + 10)*rwsize + (j - 11)] + outImg[(i - 11)*rwsize + (j - 11)]));
		}
	}
	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg1, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
	free(outImg1);



}

void main()
{

	bmpBWrw();
}
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
	if ((infile = fopen("Pentagon.bmp", "rb")) == NULL) {
		printf("No Image File");
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
	BYTE *outImg2 = malloc(hInfo.biSizeImage);
	BYTE *outImg3 = malloc(hInfo.biSizeImage);
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
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg1[i*rwsize + j] = lpImg[i*rwsize + j];

		}
	}



	float windows[5][5];
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			windows[i][j] = (float)1.0 / (5 * 5);
		}
	for (int i = 2; i < hInfo.biHeight - 2; i++) {
		for (int j = 2; j < hInfo.biWidth - 2; j++) {
			if (i == 2 && j == 2)
				outImg1[i*rwsize + j] = (int)(windows[0][0] * outImg[(i + 2)*rwsize + (j + 2)]);
			else if (i == 2)
				outImg1[i*rwsize + j] = (int)(windows[0][0] * (outImg[(i + 2)*rwsize + (j + 2)] - outImg[(i + 2)*rwsize + (j - 3)]));
			else if (j == 2)
				outImg1[i*rwsize + j] = (int)(windows[0][0] * (outImg[(i + 2)*rwsize + (j + 2)] - outImg[(i - 3)*rwsize + (j + 2)]));
			else
				outImg1[i*rwsize + j] = (int)(windows[0][0] * (outImg[(i + 2)*rwsize + (j + 2)] - outImg[(i - 3)*rwsize + (j + 2)] - outImg[(i + 2)*rwsize + (j - 3)] + outImg[(i - 3)*rwsize + (j - 3)]));
		}
	}
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg2[i*rwsize + j] = lpImg[i*rwsize + j] - outImg1[i*rwsize + j];
		}
	}
	int temp;
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			temp = (int)(lpImg[i*rwsize + j] + 0.3*outImg1[i*rwsize + j]);
			if (temp > 255)
				temp = 255;
			if (temp < 0)
				temp = 0;
			outImg3[i*rwsize + j] = temp;
		}
	}

	FILE *outfile = fopen("result1.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg1, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);


	FILE *outfile1 = fopen("result2.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile1);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile1);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile1);
	fwrite(outImg2, sizeof(char), hInfo.biSizeImage, outfile1);
	fclose(outfile1);

	FILE *outfile2 = fopen("result3.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile2);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile2);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile2);
	fwrite(outImg3, sizeof(char), hInfo.biSizeImage, outfile2);
	fclose(outfile2);
	free(lpImg);
	free(outImg1);


}

void main()
{

	bmpBWrw();
}

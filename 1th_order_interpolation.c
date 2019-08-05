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
	if ((infile = fopen("Lena.bmp", "rb")) == NULL) {
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
	BYTE *outImg = malloc(hInfo.biSizeImage * 16);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int out_rwsize = WIDTHBYTES(2 * hInfo.biBitCount * 2 * hInfo.biWidth);
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			outImg[(i * 4)*out_rwsize + (j * 4)] = lpImg[i*rwsize + j];
		}
	}

	for (int i = 0; i < hInfo.biHeight * 4; i++) {
		for (int j = 0; j < hInfo.biWidth * 4; j++) {

			if (i % 4 == 0 && j % 4 != 0) {
				outImg[i* out_rwsize + j] = (int)((outImg[i* out_rwsize + (j - (j % 4))] + (outImg[i * out_rwsize + (j + (4 - j % 4))] - outImg[i* out_rwsize + (j - (j % 4))]) * ((j % 4) / 4) + 0.5));
			}
			else if (i % 4 != 0) {
				outImg[i* out_rwsize + j] = (int)((outImg[(i - (i % 4))* out_rwsize + j] + (outImg[(i + (4 - (i % 4))) * out_rwsize + j] - outImg[(i - (i % 4))* out_rwsize + j]) * ((i % 4) / 4) + 0.5));
			}

		}
	}
	hInfo.biSizeImage = hInfo.biSizeImage * 16;
	hInfo.biWidth = hInfo.biWidth * 4;
	hInfo.biHeight = hInfo.biHeight * 4;

	FILE *outfile = fopen("result.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
	free(outImg);



}

void main()
{

	bmpBWrw();
}


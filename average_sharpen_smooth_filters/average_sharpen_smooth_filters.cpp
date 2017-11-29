#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void MemFree2D(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}

void AverageFilter1(unsigned char **In_Pad, unsigned char **Out, int nHeight, int nWidth, int nFilterSize)
{
	int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;

	FILE *out_average;
	fopen_s(&out_average, "average1.raw", "wb");

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m< nFilterSize; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)1 / 9;
				}
			}

			Out[h][w] = (int)nSum;
		}
	}

	for (int h = 0; h < 256; h++)
	{
		fwrite(Out[h], sizeof(unsigned char), 256, out_average);
	}

	fclose(out_average);
}

void AverageFilter2(unsigned char **In_Pad, unsigned char **Out, int nHeight, int nWidth, int nFilterSize)
{
	int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;

	FILE *out_average;
	fopen_s(&out_average, "average2.raw", "wb");

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m< nFilterSize; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)1 / 49;
				}
			}

			Out[h][w] = (int)nSum;
		}
	}

	for (int h = 0; h < 256; h++)
	{
		fwrite(Out[h], sizeof(unsigned char), 256, out_average);
	}

	fclose(out_average);
}

void SmoothFilter(unsigned char **In_Pad, unsigned char **Out, int nHeight, int nWidth)
{
	//int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;
	double mask[3][3] = { { 1, 2, 1 },{ 2, 4, 2 },{ 1, 2, 1 } };

	FILE *out_smooth;
	fopen_s(&out_smooth, "smooth.raw", "wb");

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m< 3; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)(mask[n][m] / 16);
				}
			}

			Out[h][w] = (int)nSum;
		}
	}

	for (int h = 0; h < 256; h++)
	{
		fwrite(Out[h], sizeof(unsigned char), 256, out_smooth);
	}

	fclose(out_smooth);
}

void SharpenFilter(unsigned char **In_Pad, unsigned char **Out, int nHeight, int nWidth)
{
	//int PadSize = (int)(nFilterSize / 2);
	int nSum = 0;
	double mask[3][3] = { { 0, -1, 0 },{ -1, 5, -1 },{ 0, -1, 0 } };

	FILE *out_sharpen;
	fopen_s(&out_sharpen, "sharpen.raw", "wb");

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nSum = 0;
			for (int n = 0; n < 3; n++)
			{
				for (int m = 0; m< 3; m++)
				{
					nSum += In_Pad[h + n][w + m] * (double)mask[n][m];
				}
			}

			if (nSum > 255)
				nSum = 255;
			if (nSum < 0)
				nSum = 0;

			Out[h][w] = (int)nSum;
		}
	}

	for (int h = 0; h < 256; h++)
	{
		fwrite(Out[h], sizeof(unsigned char), 256, out_sharpen);
	}

	fclose(out_sharpen);
}

int main()
{
	int width = 256;
	int height = 256;
	int filtersize;

	//cin >> filtersize;

	unsigned char **input_img;
	unsigned char **Pad_img;
	unsigned char **average1;
	unsigned char **average2;
	unsigned char **smooth;
	unsigned char **sharpen;

	FILE *infile;
	FILE *aver1;
	fopen_s(&infile, "lena256.raw", "rb");

	input_img = MemAlloc2D(width, height, 0);
	average1 = MemAlloc2D(width, height, 0);
	average2 = MemAlloc2D(width, height, 0);
	smooth = MemAlloc2D(width, height, 0);
	sharpen = MemAlloc2D(width, height, 0);

	for (int h = 0; h < height; h++)
	{
		fread(input_img[h], sizeof(unsigned char), width, infile);
	}

	Pad_img = Padding(input_img, height, width, 3);

	AverageFilter1(Pad_img, average1, height, width, 3);

	Pad_img = Padding(input_img, height, width, 7);

	AverageFilter2(Pad_img, average2, height, width, 7);

	Pad_img = Padding(input_img, height, width, 3);

	SmoothFilter(Pad_img, smooth, height, width);

	fopen_s(&aver1, "average1.raw", "rb");

	for (int h = 0; h < height; h++)
	{
		fread(average1[h], sizeof(unsigned char), width, aver1);
	}

	Pad_img = Padding(average1, height, width, 3);

	SharpenFilter(Pad_img, sharpen, height, width);

	MemFree2D(input_img, 256);
	MemFree2D(average1, 256);
	MemFree2D(average2, 256);
	MemFree2D(smooth, 256);
	MemFree2D(sharpen, 256);
	MemFree2D(Pad_img, 256);
	fclose(infile);
	fclose(aver1);

	return 0;
}
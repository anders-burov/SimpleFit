#define _CRT_SECURE_NO_WARNINGS 1 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1 

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <FreeImage.h>
#include "Image.h"

//#define SHAPE

#define SMPL_POSEDIRS_ELEMENTS_COUNT 4278690 // 6890 * 3 * 207
#define SMPL_SHAPEDIRS_ELEMENTS_COUNT 206700 // 6890 * 3 * 10

#ifdef SHAPE
#define ELEMENTS_COUNT SMPL_SHAPEDIRS_ELEMENTS_COUNT
#endif
#ifndef SHAPE 
#define ELEMENTS_COUNT SMPL_POSEDIRS_ELEMENTS_COUNT
#endif

int Convert(int argc, char** argv)
{
	size_t a = sizeof(DirectX::XMFLOAT3);
	if (argc != 3)
	{
		std::cout << "./ConvertDirsToBinary input output\n" << std::endl;
		return 0;
	}

	std::cout << "Converting " << argv[1] << " to " << argv[2] << std::endl;

	float* posedirs = new float[ELEMENTS_COUNT];

	{
		std::ifstream in(argv[1], std::ifstream::in);

		for (unsigned int i = 0; i < ELEMENTS_COUNT; i++)
		{
			float f;
			in >> f;
			posedirs[i] = f;
		}

		in.close();
	}

	FILE* out = fopen(argv[2], "wb");
	fwrite(posedirs, sizeof(float), ELEMENTS_COUNT, out);
	fclose(out);

	delete[] posedirs;

	{
		float* p = new float[ELEMENTS_COUNT];
		FILE* in = fopen(argv[2], "rb");
		fread(p, sizeof(float), ELEMENTS_COUNT, in);
		fclose(in);
		delete[] p;
	}

	return 0;
}

void ConvertBack(int argc, char** argv)
{
	float* p = new float[ELEMENTS_COUNT];
	FILE* in = fopen(argv[1], "rb");
	fread(p, sizeof(float), ELEMENTS_COUNT, in);
	fclose(in);

	std::ofstream out(argv[2], std::ofstream::out);

	for (unsigned int i = 0; i < ELEMENTS_COUNT; i++)
	{
		out << p[i] << " ";
	}

	delete[] p;
}

#include <Eigen/Eigen>

typedef Eigen::Triplet<float> Tri;

void triplet()
{
	std::vector<Tri> triplets;
	triplets.push_back(Tri(4, 1, 0.23));
	triplets.push_back(Tri(2, 3, 0.32));
	triplets.push_back(Tri(1, 1, 0.74));
	triplets.push_back(Tri(2, 1, 0.55));
	triplets.push_back(Tri(1, 4, 0.98));

	Eigen::SparseMatrix<float, Eigen::RowMajor> matrix(5,5);
	matrix.setFromTriplets(triplets.begin(), triplets.end());

	for (int k = 0; k < matrix.outerSize(); ++k)
	{
		for (Eigen::SparseMatrix<float, Eigen::RowMajor>::InnerIterator it(matrix, k); it; ++it)
		{
			std::cout << it.row() << "\t";
			std::cout << it.col() << "\t";
			std::cout << it.value() << std::endl;
		}
	}
}

//void FreeImageExample()
//{
//	Image im(10, 2);
//
//	for (int y = 0; y < 1; y++)
//	{
//		for (int x = 0; x < 5; x++)
//		{
//			im[y][x].rgbtBlue = 255;
//			RGBTRIPLE a = im[y][x];
//			a.rgbtBlue = a.rgbtBlue;
//		}
//	}
//
//	for (int y = 0; y < 2; y++)
//	{
//		for (int x = 0; x < 10; x++)
//		{
//			unsigned r = (unsigned)im[y][x].rgbtRed;
//			unsigned g = (unsigned)im[y][x].rgbtGreen;
//			unsigned b = (unsigned)im[y][x].rgbtBlue;
//			std::cout << " (" << r << "," << g << "," << b << "),";
//		}
//		std::cout << std::endl;
//	}
//
//	im.SavePNG("hello.png");
//	system("pause");
//}

Eigen::MatrixXf derivative(const Eigen::Vector3f& t)
{
	float z2 = t(2) * t(2);
	float r[6] = { 1.f / t(2), 0, 1.f * t(0) / z2, 0, 1.f / t(2), 1.f * t(1) / z2 };
	return Eigen::Map<Eigen::MatrixXf, Eigen::RowMajor>(r, 3, 2);
}

int main(int argc, char** argv)
{
	Eigen::Vector3f t(1, 2, 3);
	Eigen::MatrixXf m = derivative(t);
	
	return 0;
}

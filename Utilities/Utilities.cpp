#include <vector>
#include <FreeImage.h>
#include "Image.h"

#include "Utilities.h"

#include <Eigen/Eigen>
#include <unsupported/Eigen/EulerAngles>

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

Eigen::MatrixXf derivative(const Eigen::Vector3f& t)
{
	float z2 = t(2) * t(2);
	float r[6] = { 1.f / t(2), 0, 1.f * t(0) / z2, 0, 1.f / t(2), 1.f * t(1) / z2 };
	return Eigen::Map<Eigen::MatrixXf, Eigen::RowMajor>(r, 3, 2);
}

void RotationCheck()
{
	float alpha = -0.5f;
	float beta = 0.33f;
	float gamma = -0.7f;

	Eigen::Vector3f t(-1, 2, 3);
	Eigen::Matrix3f e = Eigen::EulerAngles<float, Eigen::EulerSystemZYX>(alpha, beta, gamma).toRotationMatrix();
	Eigen::Matrix3f aX = Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitX()).matrix();
	Eigen::Matrix3f aY = Eigen::AngleAxisf(beta, Eigen::Vector3f::UnitY()).matrix();
	Eigen::Matrix3f aZ = Eigen::AngleAxisf(gamma, Eigen::Vector3f::UnitZ()).matrix();
	Eigen::Matrix3f a = aZ * aY * aX;

	Eigen::Transform<float, 3, Eigen::Affine> t1 = Eigen::Translation3f(t) * a * Eigen::Translation3f(-t);
	Eigen::Transform<float, 3, Eigen::Affine> t2 = Eigen::Translation3f(t) * aZ * Eigen::Translation3f(-t) * Eigen::Translation3f(t) * aY * Eigen::Translation3f(-t) * Eigen::Translation3f(t) * aX * Eigen::Translation3f(-t);

	Eigen::Matrix4f m1 = t1.matrix();
	Eigen::Matrix4f m2 = t2.matrix();

	std::cout << "!\n";
}

int main(int argc, char** argv)
{
	//TakeSnapshot(argc, argv);
	//calibration(argc, argv);
	
	//RecordVideo(argc, argv);
	//grabcut(argc, argv);

	EraseGray(argc, argv);
	//Serialize(argc, argv);

	return 0;
}
 
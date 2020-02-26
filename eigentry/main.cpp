#include <eigen3/Eigen/Dense>
#include <iostream>

using namespace Eigen;

const Matrix<double, 2, 2> mod_mat(const Matrix2d& in_mat) {
	Matrix<double, 2, 2> res;
	res = in_mat.Zero();
	return res;
}

int main() {
	Matrix2d in_mat;
	in_mat.Identity();
	Matrix<double, 2, 2> res = mod_mat(in_mat);
	std::cout<<res<<std::endl;
	MatrixXf m(5,1);
  	m << 1,-2,-5,2,2;
  	std::cout << "sum = " << m.cwiseAbs2().sum();


	return 0;
}

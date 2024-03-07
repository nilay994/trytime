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
  	m(0,0) = 5;
	m(1,0) = -4;
	m(2,0) = -9;
	m(3,0) = -10;
	m(4,0) = -15;

  	std::cout << "sum = " << m.cwiseAbs().sum();


	return 0;
}

#ifndef _QUATERNION_HPP_
#define _QUATERNION_HPP_

#define _USE_MATH_DEFINES
#include <math.h>

class Quaternion {
public:
	double x, i, j, k;
	
public:
	Quaternion()
		: x(0.0),
		  i(0.0),
		  j(0.0),
		  k(0.0)
	{
	}

	Quaternion(double x_, double i_, double j_, double k_)
		: x(x_),
		  i(i_),
		  j(j_),
		  k(k_)
	{
	}

	Quaternion(const Quaternion& q)
		: x(q.x),
		  i(q.i),
		  j(q.j),
		  k(q.k)
	{
	}

	Quaternion& operator=(const Quaternion q) {
		this->x = q.x;
		this->i = q.i;
		this->j = q.j;
		this->k = q.k;
		return *this;
	}

	Quaternion operator*(Quaternion q) const {
		double nx = this->x * q.x - this->i * q.i - this->j * q.j - this->k * q.k;
		double ni = this->x * q.i + this->i * q.x + this->j * q.k - this->k * q.j;
		double nj = this->x * q.j + this->j * q.x + this->k * q.i - this->i * q.k;
		double nk = this->x * q.k + this->k * q.x + this->i * q.j - this->j * q.i;
		return Quaternion(nx, ni, nj, nk);
	}

	Quaternion operator*(double s) const {
		return Quaternion(s * x, s * i, s * j, s * k);
	}

	double norm() const {
		return sqrt(x * x + i * i + j * j + k * k);
	}

	Quaternion rotate(double angle, double axisX, double axisY, double axisZ) {
		angle *= M_PI / 180.0;
		double r = sin(angle / 2.0) / sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);
		return (*this) * Quaternion(cos(angle / 2.0), r * axisX, r * axisY, r * axisZ);
	}

	void setToMatrix(double* matrix, double* offset) {
		double d  = this->norm();
		matrix[0] = 1 - 2 * (j * j + k * k) / d;
		matrix[1] = 2 * (i * j + x * k) / d;
		matrix[2] = 2 * (i * k - x * j) / d;
		matrix[3] = 0.0;
		matrix[4] = 2 * (i * j - x * k) / d;
		matrix[5] = 1 - 2 * (i * i + k * k);
		matrix[6] = 2 * (j * k + x * i) / d;
		matrix[7] = 0.0;
		matrix[8] = 2 * (i * k + x * j) / d;
		matrix[9] = 2 * (j * k - x * i) / d;
		matrix[10] = 1 - 2 * (i * i + j * j) / d;
		matrix[11] = 0.0;
		matrix[12] = offset[0];
		matrix[13] = offset[1];
		matrix[14] = offset[2];
		matrix[15] = 1.0;
	}

	static Quaternion identity() {
		return Quaternion(1.0, 0.0, 0.0, 0.0);
	}
};

#endif

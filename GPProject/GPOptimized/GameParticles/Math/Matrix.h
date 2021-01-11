//----------------------------------------------------------------------------
// Mike Bunger
// Matrix header
//----------------------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include "Vect4D.h"

// forward declare
class Vect4D;

// class
class Matrix
{
public:

	// local enumerations
	enum MatrixRowEnum
	{
		MATRIX_ROW_0,
		MATRIX_ROW_1,
		MATRIX_ROW_2,
		MATRIX_ROW_3
	};

	Matrix() = default;
	Matrix(const Matrix&);
	Matrix& operator = (const Matrix&);
	~Matrix() = default;

	void set(const int& row, Vect4D *t);
	void get(const int& row, Vect4D *vOut);

	void setIdentMatrix();
	void setTransMatrix(Vect4D *t);
	void setScaleMatrix(Vect4D *s);
	void setRotZMatrix(float Z_Radians);

	//float &operator[](INDEX_ENUM e);

	Matrix& operator * (const Matrix &t);
	Matrix& operator - (const Matrix &t);

	float Determinant();

	Matrix GetAdjugate();
	Matrix& Matrix::operator/=(float t);

	void Matrix::Inverse(Matrix &out);

public:

	union
	{
		struct
		{
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};

		struct
		{
			float m0;
			float m1;
			float m2;
			float m3;
			float m4;
			float m5;
			float m6;
			float m7;
			float m8;
			float m9;
			float m10;
			float m11;
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

#endif 

// End of File


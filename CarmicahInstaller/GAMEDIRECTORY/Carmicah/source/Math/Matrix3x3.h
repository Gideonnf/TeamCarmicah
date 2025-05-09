/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Matrix3x3.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			The Matrix3x3 class template provides a 3x3 matrix implementation for mathematical 
				operations in the Carmicah game engine. It supports matrix arithmetic (multiplication),
				matrix-vector multiplication, and various transformation matrices such as translation,
				scaling, and rotation (in both radians and degrees). The class also includes functions 
				for matrix identity, transpose, and inverse calculations.

				The template is designed to handle different data types and integrates with the 
				Vector2D class for 2D transformations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/#pragma once

#ifndef MATRIX_3X3_H
#define MATRIX_3X3_H
#include "Vec2.h"
#include <cmath>

constexpr float PI = 3.14159265358f;

namespace Carmicah
{
	template <typename T> class Matrix3x3
	{
	public:
		union
		{
			struct
			{
				T m00, m01, m02;
				T m10, m11, m12;
				T m20, m21, m22;
			};

			T m[9];
		};

		//Constructors
		Matrix3x3() : m00(1.0f), m01(0.0f), m02(0.0f), m10(0.0f), m11(1.0f), m12(0.0f), m20(0.0f), m21(0.0f), m22(1.0f) {}

		/**************************************************************************/
		/*!
			Constructor for Matrix3x3 object, containing a pointer to the first element of an array.
		 */
		 /**************************************************************************/
		Matrix3x3(const T* pArr)
		{
			m00 = pArr[0]; m01 = pArr[1]; m02 = pArr[2];
			m10 = pArr[3]; m11 = pArr[4]; m12 = pArr[5];
			m20 = pArr[6]; m21 = pArr[7]; m22 = pArr[8];
		}

		/**************************************************************************/
		/*!
			Constructor for a Matrix3x3 object, containing 9 T values corresponding to the 9 elements in a 3x3 matrix.
		 */
		 /**************************************************************************/
		Matrix3x3(T _00, T _01, T _02,
			T _10, T _11, T _12,
			T _20, T _21, T _22) : m00(_00), m01(_01), m02(_02), m10(_10), m11(_11), m12(_12), m20(_20), m21(_21), m22(_22){}

		/**************************************************************************/
		/*!
			Copy Assignment operator for a Matrix3x3 object.
		 */
		 /**************************************************************************/
		Matrix3x3& operator=(const Matrix3x3& rhs)
		{
			m00 = rhs.m00;
			m01 = rhs.m01;
			m02 = rhs.m02;
			m10 = rhs.m10;
			m11 = rhs.m11;
			m12 = rhs.m12;
			m20 = rhs.m20;
			m21 = rhs.m21;
			m22 = rhs.m22;

			return *this;
		}

		Matrix3x3(const Matrix3x3& rhs) = default;

		/**************************************************************************/
		/*!
			*= Operator Overload that does matrix multiplication between rhs and the Matrix3x3 object.
		 */
		 /**************************************************************************/
		Matrix3x3& operator *= (const Matrix3x3& rhs)
		{
			Matrix3x3<T> result;

			result.m00 = m00 * rhs.m00 + m10 * rhs.m01 + m20 * rhs.m02;
			result.m10 = m00 * rhs.m10 + m10 * rhs.m11 + m20 * rhs.m12;
			result.m20 = m00 * rhs.m20 + m10 * rhs.m21 + m20 * rhs.m22;

			result.m01 = m01 * rhs.m00 + m11 * rhs.m01 + m21 * rhs.m02;
			result.m11 = m01 * rhs.m10 + m11 * rhs.m11 + m21 * rhs.m12;
			result.m21 = m01 * rhs.m20 + m11 * rhs.m21 + m21 * rhs.m22;

			result.m02 = m02 * rhs.m00 + m12 * rhs.m01 + m22 * rhs.m02;
			result.m12 = m02 * rhs.m10 + m12 * rhs.m11 + m22 * rhs.m12;
			result.m22 = m02 * rhs.m20 + m12 * rhs.m21 + m22 * rhs.m22;

			*this = result;

			return *this;
		}


		//Other Functions
		static Matrix3x3 identity() { return Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); }

		//Translation/Scale/Rot
		static Matrix3x3 translate(T x, T y) { return Matrix3x3(1, 0, 0, 0, 1, 0, x, y, 1); }
		static Matrix3x3 translate(Vector2D<T> other) { return Matrix3x3(1, 0, 0, 0, 1, 0, other.x, other.y, 1); }
		static Matrix3x3 scale(T x, T y) { return Matrix3x3(x, 0, 0, 0, y, 0, 0, 0, 1); }
		static Matrix3x3 scale(Vector2D<T> other) { return Matrix3x3(other.x, 0, 0, 0, other.y, 0, 0, 0, 1); }
		static Matrix3x3 rotRad(T angle) { return Matrix3x3(cos(angle), sin(angle), 0, -sin(angle), cos(angle), 0, 0, 0, 1); }
		static Matrix3x3 rotDeg(T angle) { T rad = angle * (PI / 180); return rotRad(rad); }

		//Scale
		Matrix3x3& scaleThis(T x, T y)
		{
			m[0] *= x;
			m[1] *= x;
			m[3] *= y;
			m[4] *= y;
			return *this;
		}

		Matrix3x3& scaleThis(Vector2D<T> other)
		{
			m[0] *= other.x;
			m[1] *= other.x;
			m[3] *= other.y;
			m[4] *= other.y;
			return *this;
		}

		//Rotation
		Matrix3x3& rotRadThis(T angle)
		{
			Matrix3x3 rotation = rotRad(angle);

			*this = *this * rotation;

			return *this;

		}

		Matrix3x3& rotDegThis(T angle)
		{
			T rad = angle * (PI / 180);

			return rotRadThis(rad);
		}

		//Translate
		Matrix3x3& translateThis(T x, T y)
		{
			m[6] += x * m[0] + y * m[3];
			m[7] += x * m[1] + y * m[4];
			return *this;
		}

		Matrix3x3& translateThis(Vector2D<T> other)
		{
			m[6] += other.x * m[0] + other.y * m[3];
			m[7] += other.x * m[1] + other.y * m[4];
			return *this;
		}

		Matrix3x3& lookAtDeg(const Vector2D<T>& pos, const T& angle, const Vector2D<T>& scale)
		{
			T rad = angle * (PI / 180);

			m[0] = cos(rad) * scale.x;
			m[3] = sin(rad) * scale.x;
			m[1] = cos(rad + PI / 2) * scale.y;
			m[4] = sin(rad + PI / 2) * scale.y;
			m[6] = -pos.x * m[0] -pos.y * m[3];
			m[7] = -pos.x * m[1] - pos.y * m[4];

			return *this;
		}


		//Transpose
		Matrix3x3 getTranspose() const
		{
			return Matrix3x3(m00, m10, m20, m01, m11, m21, m02, m12, m22);
		}


	};

	//

	/**************************************************************************/
	/*!
		* operator overload that does matrix multiplication of lhs and rhs Matrix3x3 objects.
	 */
	 /**************************************************************************/
	template <typename T> Matrix3x3<T> operator * (const Matrix3x3<T>& lhs, const Matrix3x3<T>& rhs)
	{
		Matrix3x3 result{
			lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01 + lhs.m20 * rhs.m02,
			lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01 + lhs.m21 * rhs.m02,
			lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01 + lhs.m22 * rhs.m02,
			lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11 + lhs.m20 * rhs.m12,
			lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11 + lhs.m21 * rhs.m12,
			lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11 + lhs.m22 * rhs.m12,
			lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21 + lhs.m20 * rhs.m22,
			lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21 + lhs.m21 * rhs.m22,
			lhs.m02 * rhs.m20 + lhs.m12 * rhs.m21 + lhs.m22 * rhs.m22};
		return result;
	}

	/**************************************************************************/
	/*!
		This function multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	 */
	 /**************************************************************************/
	template <typename T> Carmicah::Vector2D<T>  operator * (const Matrix3x3<T>& lhs, const Carmicah::Vector2D<T>& rhs)
	{
		return Vector2D(lhs.m00 * rhs.x + lhs.m10 * rhs.y + lhs.m20, lhs.m01 * rhs.x + lhs.m11 * rhs.y + lhs.m21);
	}

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	 */
	 /**************************************************************************/
	template <typename T> void Mtx33Identity(Matrix3x3<T>& pResult)
	{
		pResult.m00 = 1.0f;
		pResult.m01 = 0.0f;
		pResult.m02 = 0.0f;
		pResult.m10 = 0.0f;
		pResult.m11 = 1.0f;
		pResult.m12 = 0.0f;
		pResult.m20 = 0.0f;
		pResult.m21 = 0.0f;
		pResult.m22 = 1.0f;
	}
	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	template <typename T> void Mtx33Translate(Matrix3x3<T>& pResult, T x, T y)
	{
		pResult.m20 = x;
		pResult.m21 = y;
	}
	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	template <typename T> void Mtx33Scale(Matrix3x3<T>& pResult, T x, T y)
	{
		pResult.m00 = x;
		pResult.m01 = x;
		pResult.m02 = x;
		pResult.m10 = y;
		pResult.m11 = y;
		pResult.m12 = y;
	}

	template <typename T> void Mtx33RotRad(Matrix3x3<T>& pResult, T angle)
	{
		pResult.m00 = cos(angle);
		pResult.m10 = -sin(angle);
		pResult.m01 = sin(angle);
		pResult.m11 = cos(angle);
	}

	template <typename T> void Mtx33RotDeg(Matrix3x3<T>& pResult, T angle)
	{
		pResult.m00 = cosf(angle * (PI / 180));
		pResult.m10 = -sinf(angle * (PI / 180));
		pResult.m01 = sinf(angle * (PI / 180));
		pResult.m11 = cosf(angle * (PI / 180));
	}

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	 */
	/**************************************************************************/
	template <typename T> void Mtx33Transpose(Matrix3x3<T>& pResult, const Matrix3x3<T>& pMtx)
	{
		pResult.m00 = pMtx.m00;
		pResult.m01 = pMtx.m10;
		pResult.m02 = pMtx.m20;
		pResult.m10 = pMtx.m01;
		pResult.m11 = pMtx.m11;
		pResult.m12 = pMtx.m21;
		pResult.m20 = pMtx.m02;
		pResult.m21 = pMtx.m12;
		pResult.m22 = pMtx.m22;
	}
	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	template <typename T> void Mtx33Inverse(Matrix3x3<T>* pResult, T* determinant, const Matrix3x3<T>& pMtx)
	{
		Matrix3x3<T> cofactor = { pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21,
							 -(pMtx.m10 * pMtx.m22 - pMtx.m12 * pMtx.m20),
							 pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20,
							 -(pMtx.m01 * pMtx.m22 - pMtx.m02 * pMtx.m21),
							 pMtx.m00 * pMtx.m22 - pMtx.m02 * pMtx.m20,
							 -(pMtx.m00 * pMtx.m21 - pMtx.m01 * pMtx.m20),
							 pMtx.m01 * pMtx.m12 - pMtx.m02 * pMtx.m11,
							 -(pMtx.m00 * pMtx.m12 - pMtx.m02 * pMtx.m10),
							 pMtx.m00 * pMtx.m11 - pMtx.m01 * pMtx.m10 };
		Matrix3x3<T> adjoint;
		Mtx33Transpose(adjoint, cofactor);

		(*determinant) = (pMtx.m00 * pMtx.m11 * pMtx.m22 + pMtx.m10 * pMtx.m21 * pMtx.m02 + pMtx.m20 * pMtx.m01 * pMtx.m12)
			- (pMtx.m20 * pMtx.m11 * pMtx.m02 + pMtx.m10 * pMtx.m01 * pMtx.m22 + pMtx.m00 * pMtx.m21 * pMtx.m12);

		if (*determinant == 0.f)
		{
			*pResult = Matrix3x3<T>{};
		}
		else
		{
			pResult->m00 = adjoint.m00 / (*determinant);
			pResult->m01 = adjoint.m01 / (*determinant);
			pResult->m02 = adjoint.m02 / (*determinant);
			pResult->m10 = adjoint.m10 / (*determinant);
			pResult->m11 = adjoint.m11 / (*determinant);
			pResult->m12 = adjoint.m12 / (*determinant);
			pResult->m20 = adjoint.m20 / (*determinant);
			pResult->m21 = adjoint.m21 / (*determinant);
			pResult->m22 = adjoint.m22 / (*determinant);
		}
	}
	using Mtx3x3f = Matrix3x3<float>;
	using Mtx3x3d = Matrix3x3<double>;
	using Mtx3x3i = Matrix3x3<int>;
}

#endif
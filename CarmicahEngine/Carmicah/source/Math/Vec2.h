/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Vec2.h

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:			The Vector2D class template provides a 2D vector implementation for mathematical 
				operations in the Carmicah game engine. It supports basic vector arithmetic 
				(addition, subtraction, scalar multiplication, division), vector normalization, 
				length calculations, distance, dot product, and cross product magnitude. 

				The class is designed to be flexible and can handle different data types for 
				its components (x and y).

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include <cmath>
#include <iostream>

namespace Carmicah
{
	const float PI = 3.14159265358979323846f;

	template<typename T> class Vector2D
	{
	public:
		union
		{
			struct
			{
				T x, y;
			};
			T _mAccessor[2];
		};

		//Constructors
		Vector2D() : x(0), y(0) {}
		Vector2D(T _x, T _y) : x(_x), y(_y) {}

		//Assignment Operators
		Vector2D& operator=(const Vector2D& rhs) = default;
		Vector2D(const Vector2D& rhs) = default;
		Vector2D(Vector2D&&) = default;
		Vector2D& operator=(Vector2D&&) = default;

		//Compound Assignment Operators
		Vector2D& operator += (const Vector2D& rhs)
		{
			x += rhs.x;
			y += rhs.y;

			return *this;
		}
		Vector2D& operator -= (const Vector2D& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vector2D& operator *= (T rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		Vector2D& operator /= (T rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

		//Unary Operators (flip the +/- sign)
		Vector2D operator -() const
		{
			return Vector2D(-x, -y);
		}

		//Other Member Functions:
		//Length
		T length()
		{
			return std::sqrt(x * x + y * y);
		}
		//Square Length
		T squareLength()
		{
			return (x * x + y * y);
		}
		//Normalize
		Vector2D& normalize()
		{
			T len = length();
			if (len == 0)
			{
				throw std::domain_error("Cannot normalize zero vector");
			}
			*this /= len;
			return *this;
		}
		//Dot Product
		T dot(const Vector2D& other)
		{
			return(x * other.x + y * other.y);
		}
		//Cross Product
		T cross(const Vector2D& other)
		{
			return(x * other.y - y * other.x);
		}


		//Default Vectors
		static Vector2D zero() { return Vector2D(0, 0); }
		static Vector2D up() { return Vector2D(0, 1); }
		static Vector2D down() { return Vector2D(0, -1); }
		static Vector2D left() { return Vector2D(-1, 0); }
		static Vector2D right() { return Vector2D(1, 0); }
	};

	// Binary operators
	template<typename T> Vector2D<T> operator + (const Vector2D<T>& lhs, const Vector2D<T>& rhs)
	{
		return Vector2D (lhs.x + rhs.x, lhs.y + rhs.y);
	}
	template<typename T> Vector2D<T> operator - (const Vector2D<T>& lhs, const Vector2D<T>& rhs)
	{
		return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	template<typename T> Vector2D<T> operator * (const Vector2D<T>& lhs, T rhs)
	{
		return Vector2D(lhs.x * rhs, lhs.y * rhs);
	}
	template<typename T> Vector2D<T> operator * (T lhs, const Vector2D<T>& rhs)
	{
		return Vector2D(rhs.x * lhs, rhs.y * lhs);
	}
	template<typename T> Vector2D<T> operator / (const Vector2D<T>& lhs, T rhs)
	{
		if (rhs == 0)
		{
			throw std::domain_error("Division by zero");
		}
		return Vector2D(lhs.x / rhs, lhs.y / rhs);
	}

	// Normalise
	template<typename T> void Vector2DNormalize(Vector2D<T>& pResult, const Vector2D<T>& pVec0)
	{
		T normalisation = sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y);

		if (normalisation == 0)
		{
			pResult.x = 0;
			pResult.y = 0;
		}
		else
		{
			pResult.x = pVec0.x / normalisation;
			pResult.y = pVec0.y / normalisation;

		}
	}

	//Length
	template<typename T> T Vector2DLength(const Vector2D<T>& pVec0)
	{
		T length = sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y);

		return length;
	}

	//Square Length
	template<typename T> T Vector2DSquareLength(const Vector2D<T>& pVec0)
	{
		T length = pVec0.x * pVec0.x + pVec0.y * pVec0.y;

		return length;
	}

	//Dot Product
	template<typename T> T Vector2DDotProduct(const Vector2D<T>& pVec0, const Vector2D<T>& pVec1)
	{
		T dot_product = pVec0.x * pVec1.x + pVec0.y * pVec1.y;

		return dot_product;
	}

	//Cross Product
	template<typename T> T Vector2DCrossProductMag(const Vector2D<T>& pVec0, const Vector2D<T>& pVec1)
	{
		return pVec0.x * pVec1.y - pVec0.y * pVec1.x;
	}


	//Distance

	template<typename T> T Vector2DDistance(const Vector2D<T>& pVec0, const Vector2D<T>& pVec1)
	{
		Vector2D DistanceVector = pVec1 - pVec0;

		T distance = Vector2DLength(DistanceVector);

		return distance;
	}

	//Square Distance
	template<typename T> T Vector2DSquareDistance(const Vector2D<T>& pVec0, const Vector2D<T>& pVec1)
	{
		return (pVec1 - pVec0).squareLength();
	}

	//<< Operator Overload
	template <typename T> std::ostream& operator<<(std::ostream& os, const Vector2D<T>& pVec0)
	{
		os << "(" << pVec0.x << "," << pVec0.y << ")";
		return os;
	}

	//Could be used(?)
	using Vec2f = Vector2D<float>;
	using Vec2d = Vector2D<double>;
	using Vec2i = Vector2D<int>;
}
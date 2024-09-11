/* !
@file       myDataTypes.h
@author     won.m@digipen.edu
@date       22/07/2024


This file contains the declaration for the data types
    structure Idx used for storing the indexes of each tri (position, texture, normal)
    structure Color used for storing color data (each rgba is a uByte)
    structure Vec2 used for storing "2D" vectors
    structure Vec3 used for storing "3D" vectors
    structure Mat4x4 used for storing 4x4 matrixes for transforming "3D" vectors

*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef MY_MAT4_H
#define MY_MAT4_H

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <ostream>

namespace myDat
{
    struct Idx;
    union Color;
    struct Vec2;
    struct Vec3;
    union Mat4;

    struct Idx
    {
        unsigned short p0, p1, p2, t0, t1, t2, n0, n1, n2;
        bool texExists, nomExists;

        Idx(const unsigned short& p0, const unsigned short& p1, const unsigned short& p2);
        void AddTex(const unsigned short& t0, const unsigned short& t1, const unsigned short& t2);
        void AddNom(const unsigned short& n0, const unsigned short& n1, const unsigned short& n2);
    };

    union Color
    {
        struct {
            GLubyte r, g, b, a; // a, b, g, r
        };
        GLubyte val[4]; // treat <r,g,b,a> as array of 8-bit unsigned values
        GLuint raw;     // treat <r,g,b,a> as 32-bit unsigned value

        Color(GLubyte re = 0, GLubyte gr = 0, GLubyte bl = 0, GLubyte al = 255);
        Color(const Vec3& rhs);
    };

    struct Vec2
    {
        GLfloat x, y;

        Vec2(const GLfloat& x = 0.f, const GLfloat& y = 0.f);
        Vec2(const Vec2& rhs);

    };

    struct Vec3
    {
        GLfloat x, y, z;

        Vec3(const GLfloat& x = 0.f, const GLfloat& y = 0.f, const GLfloat& z = 0.f);
        Vec3(const Vec3& rhs);

        Vec3& operator+=(const Vec3& r);
        Vec3 operator-() const;
        Vec3 operator+(const Vec3& r) const;
        Vec3 operator+(const GLfloat& r) const;
        Vec3 operator-(const Vec3& r) const;
        Vec3 operator*(const Vec3& r) const;
        Vec3 operator*(const GLfloat& r) const;
        GLfloat Dot(const Vec3& r) const;
        Vec3 Cross(const Vec3& r) const;
        Vec3 LineEqn(const Vec3& point) const;
        GLfloat Length() const;
        Vec3& Normalized();
        Vec3 Normalize() const;
    };

    std::ostream& operator<<(std::ostream& os, Vec3 const& s);

    // Uses Row Major
    union Mat4
    {
        struct {
            GLfloat m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
        };
        GLfloat m[16];
        GLfloat mm[4][4];

        Mat4();
        Mat4(const GLfloat& m11, const GLfloat& m12, const GLfloat& m13, const GLfloat& m14, const GLfloat& m21, const GLfloat& m22, const GLfloat& m23, const GLfloat& m24,
            const GLfloat& m31, const GLfloat& m32, const GLfloat& m33, const GLfloat& m34, const GLfloat& m41, const GLfloat& m42, const GLfloat& m43, const GLfloat& m44);
        Mat4(const Mat4& rhs);

        Mat4 operator*(const Mat4& r) const;
        Vec3 operator*(const Vec3& r) const;
        void Reset();
        void Trans(const GLfloat& x, const GLfloat& y, const GLfloat& z, const bool& reset = false);
        void Trans(const Vec3& ref, const bool& reset = false);
        void Scale(const GLfloat& x = 1.f, const GLfloat& y = 1.f, const GLfloat& z = 1.f, const bool& reset = false);
        void Scale(const Vec3& ref, const bool& reset = false);
        void RotX(const GLfloat& angle = 0.f, const bool& reset = true);
        void RotY(const GLfloat& angle = 0.f, const bool& reset = true);
        void RotZ(const GLfloat& angle = 0.f, const bool& reset = true);
        void RotAbt(const Vec3& vec, const GLfloat& angle = 0.f, const bool& reset = true);
        Mat4 Inverse() const;
        Mat4 Transpose() const;
        void LookAt(const Vec3& eye, const Vec3& tgt, const Vec3& up = Vec3{ 0.f, 1.f });
        void Ortho(const GLfloat& l, const GLfloat& r, const GLfloat& b, const GLfloat& t, const GLfloat& n, const GLfloat& f);
        void NDCToVP(const GLfloat& w, const GLfloat& h, const GLfloat& x, const GLfloat& y);
    };

}


#endif
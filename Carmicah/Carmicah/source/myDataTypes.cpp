/* !
@file       myDataTypes.cpp
@author     won.m@digipen.edu
@date       22/07/2024


This file contains the defination for the data types
    structure Color used for storing color data (each rgba is a uByte)
    structure Vec2 used for storing "2D" vectors
    structure Vec3 used for storing "3D" vectors
    structure Mat4x4 used for storing 4x4 matrixes for transforming "3D" vectors

*//*__________________________________________________________________________*/

#include "myDataTypes.h"
#include <cmath>
#include <limits>

#define M_PI           3.14159265358979323846

namespace
{
    /*!*************************************************************************
    \brief
        To find if the number is effectively 0, and should be treated as such
    \param[in] f
        the float to check
    \return
        is the number is really close to 0 to be considered 0
    ***************************************************************************/
    bool Is0(const float& f)
    {
        return fabs(f) < FLT_EPSILON;
    }
}

namespace myDat
{
#pragma region Idx
    /*!*************************************************************************
    \brief
        Only constructor for an Idx triangle - it must have 3 position index
    \param[in] p0
        first point of the tri
    \param[in] p1
        second point of the tri
    \param[in] p2
        third point of the tri
    ***************************************************************************/
    Idx::Idx(const unsigned short& p0, const unsigned short& p1, const unsigned short& p2) : 
        p0(p0), p1(p1), p2(p2),
        t0(0), t1(0), t2(0),
        n0(0), n1(0), n2(0),
        texExists(false), nomExists(false) {}

    /*!*************************************************************************
    \brief
        Introduce texture maping for the indexes of the triangle
    \param[in] t0
        the texture index 0 of the tri
    \param[in] t1
        the texture index 1 of the tri
    \param[in] t2
        the texture index 2 of the tri
    ***************************************************************************/
    void Idx::AddTex(const unsigned short& t0, const unsigned short& t1, const unsigned short& t2)
    {
        texExists = true;
        this->t0 = t0;
        this->t1 = t1;
        this->t2 = t2;
    }

    /*!*************************************************************************
    \brief
        Introduce normal maping for the indexes of the triangle
    \param[in] t0
        the normal index 0 of the tri
    \param[in] t1
        the normal index 1 of the tri
    \param[in] t2
        the normal index 2 of the tri
    ***************************************************************************/
    void Idx::AddNom(const unsigned short& n0, const unsigned short& n1, const unsigned short& n2)
    {
        nomExists = true;
        this->n0 = n0;
        this->n1 = n1;
        this->n2 = n2;
    }

#pragma endregion

#pragma region Vec2

    /*!*************************************************************************
    \brief
        Basic / Default constructor for an Color vector
    \param[in] re
        red value
    \param[in] gr
        green value
    \param[in] bl
        blue value
    \param[in] al
        alpha value
    ***************************************************************************/
    Color::Color(GLubyte re, GLubyte gr, GLubyte bl, GLubyte al) : r(re), g(gr), b(bl), a(al) {}

    /*!*************************************************************************
    \brief
        Converting constructor for converting a Vec3 into Color 
        (Assumes the Vec3 values are [0.f,1.f])
    \param[in] rhs
        the vector to convert into color
    ***************************************************************************/
    Color::Color(const Vec3& rhs) : r(static_cast<GLubyte>(rhs.x * 255.f)), g(static_cast<GLubyte>(rhs.y * 255.f)), b(static_cast<GLubyte>(rhs.z * 255.f)), a(255) {}
#pragma endregion

#pragma region Vec2
    /*!*************************************************************************
    \brief
        Basic / Default constructor for an vector
    \param[in] x
        x value
    \param[in] y
        y value
    ***************************************************************************/
    Vec2::Vec2(const GLfloat& x, const GLfloat& y) : x(x), y(y) {}

    /*!*************************************************************************
    \brief
        Copy constuctor for the vector
    \param[in] rhs
        the vector to copy
    ***************************************************************************/
    Vec2::Vec2(const Vec2& rhs) : x(rhs.x), y(rhs.y) {}
#pragma endregion

#pragma region Vec3
    /*!*************************************************************************
    \brief
        Basic / Default constructor for an vector
    \param[in] x
        x value
    \param[in] y
        y value
    \param[in] z
        z value
    ***************************************************************************/
    Vec3::Vec3(const GLfloat& x, const GLfloat& y, const GLfloat& z) : x(x), y(y), z(z) {}

    /*!*************************************************************************
    \brief
        Copy constuctor for the vector
    \param[in] rhs
        the vector to copy
    ***************************************************************************/
    Vec3::Vec3(const Vec3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

    /*!*************************************************************************
    \brief
        Adds the other vector into this vector
    \param[in] r
        the other vector to add into this vector
    \return
        this vector referenced
    ***************************************************************************/
    Vec3& Vec3::operator+=(const Vec3& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
        return *this;
    }

    /*!*************************************************************************
    \brief
        Returns a copy of this vector negated
    \return
        the generated negated vector
    ***************************************************************************/
    Vec3 Vec3::operator-() const
    {
        Vec3 ret{ -x,-y,-z };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Adds this vector by the other vector
    \param[in] r
        the other vector to add by
    \return
        the generated vector
    ***************************************************************************/
    Vec3 Vec3::operator+(const Vec3& r) const
    {
        Vec3 ret{ x + r.x, y + r.y, z + r.z };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Adds each element of this vector by the float amount
    \param[in] r
        the value to add each element of the vector by
    \return
        the generated vector
    ***************************************************************************/
    Vec3 myDat::Vec3::operator+(const GLfloat& r) const
    {
        Vec3 ret{ x + r, y + r, z + r };
        return ret;
    }
   
    /*!*************************************************************************
    \brief
        Minuses this vector by the other vector
    \param[in] r
        the other vector to minus by
    \return
        the generated vector
    ***************************************************************************/
    Vec3 Vec3::operator-(const Vec3& r) const
    {
        Vec3 ret{ x - r.x, y - r.y, z - r.z };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Multiplies the coresponding xyz values by the other vectors xyz
    \param[in] r
        the other vector to multiply by
    \return
        the generated multiplied vector
    ***************************************************************************/
    Vec3 Vec3::operator*(const Vec3& r) const
    {
        Vec3 ret{ x * r.x, y * r.y, z * r.z };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Scalar Multiply this vector by a float
    \param[in] r
        the scalar value to multiply by
    \return
        the generated multiplied vector
    ***************************************************************************/
    Vec3 Vec3::operator*(const GLfloat& r) const
    {
        Vec3 ret{ x * r, y * r, z * r };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Prints the data in Vec3 into a ostream
    \param[in] os
        ostream reference
    \param[in] s
        the Vec3 data to send into the ostream
    \return
        the ostream
    ***************************************************************************/
    std::ostream& operator<<(std::ostream& os, Vec3 const& s)
    {
        os << "(" << s.x <<", " << s.y << ", " << s.z << ")";
        return os;
    }

    /*!*************************************************************************
    \brief
        Calculates the dot product of the 2 vectors
    \param[in] r
        the other vector to dot
    \return
        the generated dot product
    ***************************************************************************/
    GLfloat Vec3::Dot(const Vec3& r) const
    {
        GLfloat ret = x * r.x + y * r.y + z * r.z;
        return ret;
    }

    /*!*************************************************************************
    \brief
        Calculates the cross product of the 2 vectors
    \param[in] r
        the other vector to cross
    \return
        the generated cross product vector
    ***************************************************************************/
    Vec3 Vec3::Cross(const Vec3& r) const
    {
        Vec3 ret{ y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Calculates the 2D Line Equation of the vector and point
    \param[in] point
        reference point to calculate the line equation
    \return
        the generated 2D Line Equation
    ***************************************************************************/
    Vec3 Vec3::LineEqn(const Vec3& point) const
    {
        Vec3 ret{ -y, x, y * point.x - x * point.y };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Finds the length of the vector
    \return
        Returns the length of the vector
    ***************************************************************************/
    GLfloat Vec3::Length() const
    {
        if (Is0(x) && Is0(y) && Is0(z))
            return 0.f;
        return sqrtf(x * x + y * y + z * z);
    }

    /*!*************************************************************************
    \brief
        Tries to normalize the line
    \return
        Reference to this vector itself (after normalizing)
    \Error
        (unable to normalize line): Vector is unchanged
    ***************************************************************************/
    Vec3& Vec3::Normalized()
    {
        if (!Is0(x) || !Is0(y) || !Is0(z))
        {
            GLfloat length = sqrtf(x * x + y * y + z * z);
            x /= length;
            y /= length;
            z /= length;
        }
        return *this;
    }

    /*!*************************************************************************
    \brief
        Tries to calculate the normalized line, and returns a new line
    \return
        the calculated normalized line
    \Error
        (unable to normalize line): Returns a 0 vector
    ***************************************************************************/
    Vec3 Vec3::Normalize() const
    {
        Vec3 ans{ x, y, z };
        ans.Normalized();
        return ans;
    }
#pragma endregion

#pragma region Mat4

    /*!*************************************************************************
    \brief
        Default constructor for an identity matrix
    ***************************************************************************/
    Mat4::Mat4() : m11(1.f), m12(), m13(), m14(), m21(), m22(1.f), m23(), m24(), m31(), m32(), m33(1.f), m34(), m41(), m42(), m43(), m44(1.f) {}

    /*!*************************************************************************
    \brief
        Overloaded constructor for an matrix
    \param[in] m11 ~ m44
        There are alot of parameters/ arguments, so here is a visual example (Row Major):
        m11 m12 m13 m14
        m21 m22 m23 m24
        m31 m32 m33 m34
        m41 m42 m43 m44
    ***************************************************************************/
    Mat4::Mat4(const GLfloat& m11, const GLfloat& m12, const GLfloat& m13, const GLfloat& m14, const GLfloat& m21, const GLfloat& m22, const GLfloat& m23, const GLfloat& m24,
        const GLfloat& m31, const GLfloat& m32, const GLfloat& m33, const GLfloat& m34, const GLfloat& m41, const GLfloat& m42, const GLfloat& m43, const GLfloat& m44):
        m11(m11), m12(m12), m13(m13), m14(m14), m21(m21), m22(m22), m23(m23), m24(m24), m31(m31), m32(m32), m33(m33), m34(m34), m41(m41), m42(m42), m43(m43), m44(m44) {}

    /*!*************************************************************************
    \brief
        Copy constuctor for the matrix
    \param[in] rhs
        the matrix to copy
    ***************************************************************************/
    Mat4::Mat4(const Mat4& rhs) : m11(rhs.m11), m12(rhs.m12), m13(rhs.m13), m14(rhs.m14), m21(rhs.m21), m22(rhs.m22), m23(rhs.m23), m24(rhs.m24),
        m31(rhs.m31), m32(rhs.m32), m33(rhs.m33), m34(rhs.m34), m41(rhs.m41), m42(rhs.m42), m43(rhs.m43), m44(rhs.m44) {}

    /*!*************************************************************************
    \brief
        Multiply this matrix by another matrix, and return the resulting matrix
    \param[in] r
        matrix to multiply by
    ***************************************************************************/
    Mat4 Mat4::operator*(const Mat4& r) const
    {
        Mat4 ret{
            m11 * r.m11 + m12 * r.m21 + m13 * r.m31 + m14 * r.m41,  // 11
            m11 * r.m12 + m12 * r.m22 + m13 * r.m32 + m14 * r.m42,  // 12
            m11 * r.m13 + m12 * r.m23 + m13 * r.m33 + m14 * r.m43,  // 13
            m11 * r.m14 + m12 * r.m24 + m13 * r.m34 + m14 * r.m44,  // 14

            m21 * r.m11 + m22 * r.m21 + m23 * r.m31 + m24 * r.m41,  // 21
            m21 * r.m12 + m22 * r.m22 + m23 * r.m32 + m24 * r.m42,  // 22
            m21 * r.m13 + m22 * r.m23 + m23 * r.m33 + m24 * r.m43,  // 23
            m21 * r.m14 + m22 * r.m24 + m23 * r.m34 + m24 * r.m44,  // 24

            m31 * r.m11 + m32 * r.m21 + m33 * r.m31 + m34 * r.m41,  // 31
            m31 * r.m12 + m32 * r.m22 + m33 * r.m32 + m34 * r.m42,  // 32
            m31 * r.m13 + m32 * r.m23 + m33 * r.m33 + m34 * r.m43,  // 33
            m31 * r.m14 + m32 * r.m24 + m33 * r.m34 + m34 * r.m44,  // 34

            m41 * r.m11 + m42 * r.m21 + m43 * r.m31 + m44 * r.m41,  // 41
            m41 * r.m12 + m42 * r.m22 + m43 * r.m32 + m44 * r.m42,  // 42
            m41 * r.m13 + m42 * r.m23 + m43 * r.m33 + m44 * r.m43,  // 43
            m41 * r.m14 + m42 * r.m24 + m43 * r.m34 + m44 * r.m44,  // 44
        };
        return ret; // Uses less calls when it's declared seperately
    }

    /*!*************************************************************************
    \brief
        Multiply this matrix by a vector, and return the resulting vector
    \param[in] r
        vector to multiply by
    ***************************************************************************/
    Vec3 Mat4::operator*(const Vec3& r) const
    {
        Vec3 ret{
            m11 * r.x + m12 * r.y + m13 * r.z + m14,
            m21 * r.x + m22 * r.y + m23 * r.z + m24,
            m31 * r.x + m32 * r.y + m33 * r.z + m34,
        };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Reset the matrix to an identity matrix
    ***************************************************************************/
    void Mat4::Reset()
    {
        std::fill(m, m + 16, 0.f);
        m11 = m22 = m33 = m44 = 1.f;
    }

    /*!*************************************************************************
    \brief
        Translation Matrix calculations
    \param[in] x
      translation in x axis
    \param[in] y
      translation in y axis
    \param[in] z
      translation in z axis
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::Trans(const GLfloat& x, const GLfloat& y, const GLfloat& z, const bool& reset)
    {
        if(reset)
            Reset();
        m14 += x;
        m24 += y;
        m34 += z;
    }

    /*!*************************************************************************
    \brief
        Overloaded Translation Matrix calculations
    \param[in] ref
        Vector to translate by
    \param[in] reset
        To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::Trans(const Vec3& ref, const bool& reset)
    {
        Trans(ref.x, ref.y, ref.z, reset);
    }

    /*!*************************************************************************
    \brief
        Scale Matrix calculations
    \param[in] x
      scaling in x axis
    \param[in] y
      scaling in y axis
    \param[in] z
      scaling in z axis
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::Scale(const GLfloat& x, const GLfloat& y, const GLfloat& z, const bool& reset)
    {
        Mat4 ans;
        ans.m11 *= x;
        ans.m22 *= y;
        ans.m33 *= z;
        if (reset)
            *this = ans;
        else
            *this = ans * *this;
    }

    /*!*************************************************************************
    \brief
        Overloaded Scale Matrix calculations
    \param[in] ref
        scaling axis
    \param[in] reset
        To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::Scale(const Vec3& ref, const bool& reset)
    {
        Scale(ref.x, ref.y, ref.z, reset);
    }

    /*!*************************************************************************
    \brief
        Rotation Matrix for rotating about the x-axis
    \param[in] angle
      angle(in degrees) to rotate about
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::RotX(const GLfloat& angle, const bool& reset)
    {
        GLfloat c = cosf(static_cast<GLfloat>(M_PI) * angle / 180.f),
            s = sinf(static_cast<GLfloat>(M_PI) * angle / 180.f);
        Mat4 ans{
            1.f, 0.f, 0.f, 0.f,
            0.f, c  , -s , 0.f,
            0.f, s  , c  , 0.f,
            0.f, 0.f, 0.f, 1.f
        };
        if (reset)
            *this = ans;
        else
            *this = ans * *this;
    }

    /*!*************************************************************************
    \brief
        Rotation Matrix for rotating about the y-axis
    \param[in] angle
      angle(in degrees) to rotate about
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::RotY(const GLfloat& angle, const bool& reset)
    {
        GLfloat c = cosf(static_cast<GLfloat>(M_PI) * angle / 180.f),
            s = sinf(static_cast<GLfloat>(M_PI) * angle / 180.f);
        Mat4 ans{
            c  , 0.f, s  , 0.f,
            0.f, 1.f, 0.f, 0.f,
            -s , 0.f, c  , 0.f,
            0.f, 0.f, 0.f, 1.f
        };
        if (reset)
            *this = ans;
        else
            *this = *this * ans;
    }

    /*!*************************************************************************
    \brief
        Rotation Matrix for rotating about the z-axis
    \param[in] angle
      angle(in degrees) to rotate about
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::RotZ(const GLfloat& angle, const bool& reset)
    {
        GLfloat c = cosf(static_cast<GLfloat>(M_PI) * angle / 180.f),
            s = sinf(static_cast<GLfloat>(M_PI) * angle / 180.f);
        Mat4 ans{
            c  , -s , 0.f, 0.f,
            s  , c  , 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
        };
        if (reset)
            *this = ans;
        else
            *this = *this * ans;
    }

    /*!*************************************************************************
    \brief
        Rotation Matrix for rotating about a line
    \param[in] vec
      Vector to rotate about (assume not 0)
    \param[in] angle
      angle(in degrees) to rotate about
    \param[in] reset
      To reset the matrix, or to compound it with previously calculated values
    ***************************************************************************/
    void Mat4::RotAbt(const Vec3& vec, const GLfloat& angle, const bool& reset)
    {
        Vec3 n = vec.Normalize();
        GLfloat c = cosf(static_cast<GLfloat>(M_PI) * angle / 180.f),
            s = sinf(static_cast<GLfloat>(M_PI) * angle / 180.f);

        Mat4 ans{
            (1 - c) * n.x * n.x + c,
            (1 - c) * n.x * n.y - s * n.z, 
            (1 - c) * n.x * n.z + s * n.y,
            0.f,

            (1 - c) * n.x * n.y + s * n.z,
            (1 - c) * n.y * n.y + c, 
            (1 - c) * n.y * n.z - s * n.x,
            0.f,

            (1 - c) * n.x * n.z - s * n.y,
            (1 - c) * n.y * n.z + s * n.x, 
            (1 - c) * n.z * n.z + c,
            0.f,

            0.f,
            0.f,
            0.f,
            1.f,
        };

        if (reset)
            *this = ans;
        else
            *this = *this * ans;
    }

    /*!*************************************************************************
    \brief
        Returns a calculated inverse of the "affine" matrix
    \return
        Computed inverse Matrix
    \error
        Does not work if matrix is not affine
        Returns the input matrix if matrix is not inversable
    ***************************************************************************/
    Mat4 Mat4::Inverse() const
    {
        // Affine 4x4 Matrix calculations
        //  m11 m12 m13     m14
        //  m21 m22 m23     m24
        //  m31 m32 m33     m34
        // 
        //  0   0   0       1

        GLfloat det =
            m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 -
            m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31;
        if(Is0(det)) // Matrix cannot be inversed
            return *this;
        Mat4 ret{};
        // Already Transposed
        ret.m11 = (m22 * m33 - m23 * m32) / det;
        ret.m21 = (m23 * m31 - m21 * m33) / det;
        ret.m31 = (m21 * m32 - m22 * m31) / det;
        ret.m12 = (m13 * m32 - m12 * m33) / det;
        ret.m22 = (m11 * m33 - m13 * m31) / det;
        ret.m32 = (m12 * m31 - m11 * m32) / det;
        ret.m13 = (m12 * m23 - m13 * m22) / det;
        ret.m23 = (m13 * m21 - m11 * m23) / det;
        ret.m33 = (m11 * m22 - m12 * m21) / det;

        Vec3 temp{ -m14,-m24,-m34 };
        temp = ret * temp;

        ret.m14 = temp.x;
        ret.m24 = temp.y;
        ret.m34 = temp.z;

        return ret;
    }

    /*!*************************************************************************
    \brief
        Returns a calculated transpose matrix
    \return
        Computed transpose matrix
    ***************************************************************************/
    Mat4 Mat4::Transpose() const
    {
        Mat4 ret{
            m11, m21, m31, m41,
            m12, m22, m32, m42,
            m13, m23, m33, m43,
            m14, m24, m34, m44
        };
        return ret;
    }

    /*!*************************************************************************
    \brief
        Camera(view) Matrix calculation, changes the matrix into a view matrix
    \param[in] eye
        Position of the camera
    \param[in] tgt
        Target point
    \param[in] up
        Ideal up vector
    ***************************************************************************/
    void Mat4::LookAt(const Vec3& eye, const Vec3& tgt, const Vec3& up)
    {
        // Right, Up, Back, eye
        Vec3 u, v, w;
        w = (eye - tgt).Normalized();
        u = up.Cross(w);
        if (!Is0(u.x) || !Is0(u.y) || !Is0(u.z))
        {
            u.Normalized();
            v = w.Cross(u).Normalized();
        }
        else
        {
            v = (up - w * up.Dot(w)).Normalized();
            u = v.Cross(w).Normalized();
        }

        Reset();

        m11 = u.x;
        m12 = u.y;
        m13 = u.z;
        m21 = v.x;
        m22 = v.y;
        m23 = v.z;
        m31 = w.x;
        m32 = w.y;
        m33 = w.z;
        m14 = (-u).Dot(eye);
        m24 = (-v).Dot(eye);
        m34 = (-w).Dot(eye);
    }

    /*!*************************************************************************
    \brief
        Orthographic projection Matrix calculation
    \param[in] l
        left bounds
    \param[in] r
        right bounds
    \param[in] b
        bottom bounds
    \param[in] t
        top bounds
    \param[in] n
        near bounds
    \param[in] f
        far bounds
    ***************************************************************************/
    void Mat4::Ortho(const GLfloat& l, const GLfloat& r, const GLfloat& b, const GLfloat& t, const GLfloat& n, const GLfloat& f)
    {
        Reset();
        m11 = 2.f / (r - l);
        m22 = 2.f / (t - b);
        m33 = -2.f / (f - n);
        m14 = -(r + l) / (r - l);
        m24 = -(t + b) / (t - b);
        m34 = -(f + n) / (f - n);
    }

    /*!*************************************************************************
    \brief
        NDC to Viewport Matrix calculation
    \param[in] w
        width
    \param[in] h
        height
    \param[in] x
        x position
    \param[in] y
        y position
    ***************************************************************************/
    void Mat4::NDCToVP(const GLfloat& w, const GLfloat& h, const GLfloat& x, const GLfloat& y)
    {
        Reset();
        m11 = w / 2.f;
        m22 = h / 2.f;
        m33 = m34 = 0.5f;
        m14 = m11 + x;
        m24 = m22 + y;
    }

#pragma endregion

}
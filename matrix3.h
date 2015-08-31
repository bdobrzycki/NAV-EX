#ifndef __MATRIX3_H__
#define __MATRIX3_H__

#include "mathUtils.h"
#include "vector4.h"
#include "quat.h"

namespace GrapheneMath
{
    /**
     *  3x3 row-major matrix.
     */
    template <typename Scalar>
    class Matrix3
    {
    private:
        Scalar m_data[ 9 ];

    public:
        Matrix3(
            const Scalar& r0 = Scalar{},
            const Scalar& r1 = Scalar{},
            const Scalar& r2 = Scalar{},
            const Scalar& r3 = Scalar{},
            const Scalar& r4 = Scalar{},
            const Scalar& r5 = Scalar{},
            const Scalar& r6 = Scalar{},
            const Scalar& r7 = Scalar{},
            const Scalar& r8 = Scalar{} )
        {
            m_data[0] = r0; m_data[1] = r1; m_data[2] = r2;
            m_data[3] = r3; m_data[4] = r4; m_data[5] = r5;
            m_data[6] = r6; m_data[7] = r7; m_data[8] = r8;
        }

        Matrix3(
            const Vector3<Scalar>& row0,
            const Vector3<Scalar>& row1,
            const Vector3<Scalar>& row2 )
        {
            m_data[0] = row0.GetX(); m_data[1] = row0.GetY(); m_data[2] = row0.GetZ();
            m_data[3] = row1.GetX(); m_data[4] = row1.GetY(); m_data[5] = row1.GetZ();
            m_data[6] = row2.GetX(); m_data[7] = row2.GetY(); m_data[8] = row2.GetZ();
        }

        const Matrix3<Scalar> Transpose() const
        {
            return Matrix3(m_data[0], m_data[3], m_data[6],
                           m_data[1], m_data[4], m_data[7],
                           m_data[2], m_data[5], m_data[8]);
        }

        const Matrix3<Scalar> SetTranspose()
        {
            const Matrix3<Scalar> mat = (*this).Transpose();
            m_data[0] = mat[0]; m_data[1] = mat[1]; m_data[2] = mat[2];
            m_data[3] = mat[3]; m_data[4] = mat[4]; m_data[5] = mat[5];
            m_data[6] = mat[6]; m_data[7] = mat[7]; m_data[8] = mat[8];
            return *this;
        }

        const Vector3<Scalar> operator*( const Vector3<Scalar>& vec ) const;
        const Matrix3<Scalar> operator*( Scalar s ) const;
        const Matrix3<Scalar> operator*( const Matrix3<Scalar>& mat) const;

        const Scalar operator[](int index) const
        {
            assert(index < 9 && index >= 0);
            const int idx = std::max((std::min(index, 8)), 0);
            return m_data[idx];
        }

        const Scalar GetDeterminant() const
        {
            const Scalar det =
                ((m_data[0] * (m_data[4] * m_data[8] - m_data[5] * m_data[7])) -
                ( m_data[1] * (m_data[3] * m_data[8] - m_data[5] * m_data[6])) +
                ( m_data[2] * (m_data[3] * m_data[7] - m_data[4] * m_data[6])));

            return det;
        }

        const Matrix3<Scalar> operator-(const Matrix3<Scalar>& mat) const;
        const Matrix3<Scalar> operator-=( const Matrix3<Scalar>& mat );
        const Matrix3<Scalar> operator+( const Matrix3<Scalar>& mat ) const;
        const Matrix3<Scalar> operator+=( const Matrix3<Scalar>& mat );
        
        /**
        * Sets the matrix to be the inverse of the given matrix.
        * @param mat The matrix to invert and use to set this.
        */
        void SetInverse( const Matrix3<Scalar>& mat );
       
        const Matrix3<Scalar> Inverse() const;

        void FromQuat(const Quaternion<Scalar>& q)
        {
            const Scalar ONE = Scalar(1.0);
            const Scalar TWO = Scalar(2.0);
            m_data[0] = ONE - (TWO * q.GetY() * q.GetY() + TWO * q.GetZ() * q.GetZ());
            m_data[1] = TWO * q.GetX() * q.GetY() + TWO * q.GetZ() * q.GetW();
            m_data[2] = TWO * q.GetX() * q.GetZ() - TWO * q.GetY() * q.GetW();
            m_data[3] = TWO * q.GetX() * q.GetY() - TWO * q.GetZ() * q.GetW();
            m_data[4] = ONE - (TWO * q.GetX() * q.GetX() + TWO * q.GetZ() * q.GetZ());
            m_data[5] = TWO * q.GetY() * q.GetZ() + TWO * q.GetX() * q.GetW();
            m_data[6] = TWO * q.GetX() * q.GetZ() + TWO * q.GetY() * q.GetW();
            m_data[7] = TWO * q.GetY() * q.GetZ() - TWO * q.GetX() * q.GetW();
            m_data[8] = ONE - (TWO * q.GetX() * q.GetX() + TWO * q.GetY() * q.GetY());
        }
    };
    
    template <typename Scalar>
        const Vector3<Scalar> Matrix3<Scalar>::operator*(const Vector3<Scalar>& vec) const
        {
            return Vector3<Scalar>(
                vec.GetX() * m_data[0] +
                vec.GetY() * m_data[1] +
                vec.GetZ() * m_data[2],

                vec.GetX() * m_data[3] +
                vec.GetY() * m_data[4] +
                vec.GetZ() * m_data[5],

                vec.GetX() * m_data[6] +
                vec.GetY() * m_data[7] +
                vec.GetZ() * m_data[8]);
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator*(Scalar s) const
        {
            return Matrix3<Scalar>(
                m_data[0] * s,
                m_data[1] * s,
                m_data[2] * s,
                m_data[3] * s,
                m_data[4] * s,
                m_data[5] * s,
                m_data[6] * s,
                m_data[7] * s,
                m_data[8] * s);
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator*(const Matrix3<Scalar>& mat) const
        {
            Matrix3<Scalar> resultMat;

            resultMat.m_data[0] = m_data[0] * mat.m_data[0] +
                                  m_data[1] * mat.m_data[3] +
                                  m_data[2] * mat.m_data[6];

            resultMat.m_data[1] = m_data[0] * mat.m_data[1] +
                                  m_data[1] * mat.m_data[4] +
                                  m_data[2] * mat.m_data[7];

            resultMat.m_data[2] = m_data[0] * mat.m_data[2] +
                                  m_data[1] * mat.m_data[5] +
                                  m_data[2] * mat.m_data[8];

            resultMat.m_data[3] = m_data[3] * mat.m_data[0] +
                                  m_data[4] * mat.m_data[3] +
                                  m_data[5] * mat.m_data[6];

            resultMat.m_data[4] = m_data[3] * mat.m_data[1] +
                                  m_data[4] * mat.m_data[4] +
                                  m_data[5] * mat.m_data[7];

            resultMat.m_data[5] = m_data[3] * mat.m_data[2] +
                                  m_data[4] * mat.m_data[5] +
                                  m_data[5] * mat.m_data[8];

            resultMat.m_data[6] = m_data[6] * mat.m_data[0] +
                                  m_data[7] * mat.m_data[3] +
                                  m_data[8] * mat.m_data[6];

            resultMat.m_data[7] = m_data[6] * mat.m_data[1] +
                                  m_data[7] * mat.m_data[4] +
                                  m_data[8] * mat.m_data[7];

            resultMat.m_data[8] = m_data[6] * mat.m_data[2] +
                                  m_data[7] * mat.m_data[5] +
                                  m_data[8] * mat.m_data[8];

            return resultMat;
        }
        
        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator-(const Matrix3<Scalar>& mat) const
        {
            return Matrix3(m_data[0] - mat[0], m_data[1] - mat[1], m_data[2] - mat[2],
                           m_data[3] - mat[3], m_data[4] - mat[4], m_data[5] - mat[5],
                           m_data[6] - mat[6], m_data[7] - mat[7], m_data[8] - mat[8]);
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator-=(const Matrix3<Scalar>& mat)
        {
            m_data[0] -= mat[0]; m_data[1] -= mat[1]; m_data[2] -= mat[2];
            m_data[3] -= mat[3]; m_data[4] -= mat[4]; m_data[5] -= mat[5];
            m_data[6] -= mat[6]; m_data[7] -= mat[7]; m_data[8] -= mat[8];
            return *this;
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator+(const Matrix3<Scalar>& mat) const
        {
            return Matrix3(m_data[0] + mat[0], m_data[1] + mat[1], m_data[2] + mat[2],
                           m_data[3] + mat[3], m_data[4] + mat[4], m_data[5] + mat[5],
                           m_data[6] + mat[6], m_data[7] + mat[7], m_data[8] + mat[8]);
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::operator+=(const Matrix3<Scalar>& mat)
        {
            m_data[0] += mat[0]; m_data[1] += mat[1]; m_data[2] += mat[2];
            m_data[3] += mat[3]; m_data[4] += mat[4]; m_data[5] += mat[5];
            m_data[6] += mat[6]; m_data[7] += mat[7]; m_data[8] += mat[8];
            return *this;
        }

        /**

        * Sets the matrix to be the inverse of the given matrix.

        * @param mat The matrix to invert and use to set this.

        */
        template <typename Scalar>
        void Matrix3<Scalar>::SetInverse(const Matrix3<Scalar>& mat)
        {
            const Scalar t4  = mat.m_data[0] * mat.m_data[4];
            const Scalar t6  = mat.m_data[0] * mat.m_data[5];
            const Scalar t8  = mat.m_data[1] * mat.m_data[3];
            const Scalar t10 = mat.m_data[2] * mat.m_data[3];
            const Scalar t12 = mat.m_data[1] * mat.m_data[6];
            const Scalar t14 = mat.m_data[2] * mat.m_data[6];

            // calculate the determinant
            const Scalar t16 =
                (t4  * mat.m_data[8] - t6  * mat.m_data[7] - t8  * mat.m_data[8] +
                 t10 * mat.m_data[7] + t12 * mat.m_data[5] - t14 * mat.m_data[4]);

            // make sure the determinant is non-zero
            if (abs(t16) < 1.0E-10) return; // TODO add global tolerance

            const Scalar t17 = Scalar(1.0) / t16;

            m_data[0] = (mat.m_data[4] * mat.m_data[8] - mat.m_data[5] * mat.m_data[7]) * t17;
            m_data[1] = -(mat.m_data[1] * mat.m_data[8] - mat.m_data[2] * mat.m_data[7]) * t17;
            m_data[2] = (mat.m_data[1] * mat.m_data[5] - mat.m_data[2] * mat.m_data[4]) * t17;
            m_data[3] = -(mat.m_data[3] * mat.m_data[8] - mat.m_data[5] * mat.m_data[6]) * t17;
            m_data[4] = (mat.m_data[0] * mat.m_data[8] - t14) * t17;
            m_data[5] = -(t6 - t10) * t17;
            m_data[6] = (mat.m_data[3] * mat.m_data[7] - mat.m_data[4] * mat.m_data[6]) * t17;
            m_data[7] = -(mat.m_data[0] * mat.m_data[7] - t12) * t17;
            m_data[8] = (t4 - t8) * t17;
        }

        template <typename Scalar>
        const Matrix3<Scalar> Matrix3<Scalar>::Inverse() const
        {
            Matrix3<Scalar> invMat;
            invMat.SetInverse(*this);
            return invMat;
        }


}  //< End of GrapheneMath namespace.
#endif //_MATRIX3_H

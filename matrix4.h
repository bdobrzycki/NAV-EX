#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "vector4.h"
#include "matrix3.h"

namespace GrapheneMath
{
    /**
     * Holds a transform matrix, consisting of rotation matrix and a position.
     * homogenous matrix, p 154 Robotics book
     */

    template <typename Scalar>
    class Matrix4
    {
    public:
        enum Elements
        {
            r11 = 0, r12,    r13,    X,
            r21,     r22,    r23,    Y,
            r31,     r32,    r33,    Z,
            O_,      _O_,     _O,   _1_,
            numOfElems
        };

    private:
        Scalar m_data[ numOfElems ];

    public:
        // Default ctor.
        Matrix4( const Vector4<Scalar>& _row0 = Vector4<Scalar>{},
                 const Vector4<Scalar>& _row1 = Vector4<Scalar>{},
                 const Vector4<Scalar>& _row2 = Vector4<Scalar>{},
                 const Vector4<Scalar>& _row3 = Vector4<Scalar>{} )
        {
            m_data[r11] = _row0.GetX();
            m_data[r12] = _row0.GetY();
            m_data[r13] = _row0.GetZ();
            m_data[X]   = _row0.GetW();

            m_data[r21] = _row1.GetX();
            m_data[r22] = _row1.GetY();
            m_data[r23] = _row1.GetZ();
            m_data[Y]   = _row1.GetW();

            m_data[r31] = _row2.GetX();
            m_data[r32] = _row2.GetY();
            m_data[r33] = _row2.GetZ();
            m_data[Z]   = _row2.GetW();

            m_data[ O_] = _row3.GetX();
            m_data[_O_] = _row3.GetY();
            m_data[_O ] = _row3.GetZ();
            m_data[_1_] = _row3.GetW();
        }

        Matrix4(Scalar _r11, Scalar _r12, Scalar _r13,
                Scalar _r21, Scalar _r22, Scalar _r23,
                Scalar _r31, Scalar _r32, Scalar _r33,
                Scalar _x,   Scalar _y,   Scalar _z)
        {
            m_data[r11] = _r11;
            m_data[r12] = _r12;
            m_data[r13] = _r13;
            m_data[X]   = _x;

            m_data[r21] = _r21;
            m_data[r22] = _r22;
            m_data[r23] = _r23;
            m_data[Y] =   _y;

            m_data[r31] = _r31;
            m_data[r32] = _r32;
            m_data[r33] = _r33;
            m_data[Z]   = _z;

            m_data[ O_] = 0;
            m_data[_O_] = 0;
            m_data[_O ] = 0;
            m_data[_1_] = 1;
        }


        void FromRot3x3Trans( const Matrix3<Scalar>& rotMat, const Vector3<Scalar>& translation );

        const Scalar operator[](int index) const
        {
            assert(index < numOfElems && index >= 0);
            const int safeIdx = std::max((std::min(index, (int)numOfElems)), 0);
            return m_data[safeIdx];
        }

        /**
        * Transform the given vector by this matrix
        * @param vec The vector to transform.
        * eq. 4.24, p. 156 "Theory of applied robotics".
        */
        const Vector4<Scalar> operator*( const Vector4<Scalar>& vec ) const;
        const Vector3<Scalar> operator*( const Vector3<Scalar>& vec ) const;
        
        //No assumption that both are rotation matrices + homogenous (more concise is possible, this is naive)
        const Matrix4<Scalar> operator*( const Matrix4<Scalar>& mat ) const;
        
        //template <typename Scalar>
        const Matrix3<Scalar> GetRotation3x3() const
        {
            return Matrix3<Scalar>(m_data[r11], m_data[r12], m_data[r13],
                                   m_data[r21], m_data[r22], m_data[r23],
                                   m_data[r31], m_data[r32], m_data[r33]);
        }

        void SetRotation3x3( const Matrix3<Scalar>& rotMat );

        const Vector3<Scalar> GetTranslation() const
        {
            return Vector3<Scalar>(m_data[X], m_data[Y], m_data[Z]);
        }

        void SetTranslation( const Vector3<Scalar>& translation );
        
        void SetTranslation( Scalar x, Scalar y, Scalar z );

        const Matrix4<Scalar> OrthoInverse() const
        {
            Matrix4<Scalar> invMat;
            invMat.SetRotation3x3(GetRotation3x3().Transpose());
            Vector3<Scalar> translation = (invMat.GetRotation3x3() * Scalar(-1.0)) * GetTranslation();
            invMat.SetTranslation(translation);
            return invMat;
        }

        // Make transform matrix from position and quaternion.
        const Matrix4<Scalar> CalculateTransformMatrix(const Vector4<Scalar>& position, const Quaternion<Scalar>& orientation);

    };
    
    template <typename Scalar>
    void Matrix4<Scalar>::FromRot3x3Trans( const Matrix3<Scalar>& rotMat, const Vector3<Scalar>& translation )
    {
        SetRotation3x3( rotMat );
        SetTranslation( translation );
    }
    
    /**
    * Transform the given vector by this matrix
    * @param vec The vector to transform.
    * eq. 4.24, p. 156 "Theory of applied robotics".
    */
    template <typename Scalar>
    const Vector4<Scalar> Matrix4<Scalar>::operator*(const Vector4<Scalar>& vec) const
    {
        return Vector4<Scalar>(
            m_data[X] + m_data[r11] * vec.GetX() + m_data[r12] * vec.GetY() + m_data[r13] * vec.GetZ(),
            m_data[Y] + m_data[r21] * vec.GetX() + m_data[r22] * vec.GetY() + m_data[r23] * vec.GetZ(),
            m_data[Z] + m_data[r31] * vec.GetX() + m_data[r32] * vec.GetY() + m_data[r33] * vec.GetZ(),
            1);
    }

    template <typename Scalar>
    const Vector3<Scalar> Matrix4<Scalar>::operator*(const Vector3<Scalar>& vec) const
    {
        return Vector3<Scalar>(
            m_data[X] + m_data[r11] * vec.GetX() + m_data[r12] * vec.GetY() + m_data[r13] * vec.GetZ(),
            m_data[Y] + m_data[r21] * vec.GetX() + m_data[r22] * vec.GetY() + m_data[r23] * vec.GetZ(),
            m_data[Z] + m_data[r31] * vec.GetX() + m_data[r32] * vec.GetY() + m_data[r33] * vec.GetZ());
    }
    
    /*
          r11,     r12,    r13,    X,        r11,     r12,    r13,    X,
          r21,     r22,    r23,    Y,        r21,     r22,    r23,    Y,
          r31,     r32,    r33,    Z,        r31,     r32,    r33,    Z,
          O_,      _O_,     _O,   _1_,       O_,      _O_,     _O,   _1_,
    */
    template <typename Scalar>
    const Matrix4<Scalar> Matrix4<Scalar>::operator*( const Matrix4<Scalar>& mat ) const
    {
        const Vector4<Scalar> row0(
          m_data[ r11 ] * mat[ r11 ] + 
          m_data[ r12 ] * mat[ r21 ] + 
          m_data[ r13 ] * mat[ r31 ] + 
          m_data[ X ]   * mat[ O_ ],
                           
          m_data[ r11 ] * mat[ r12 ] + 
          m_data[ r12 ] * mat[ r22 ] + 
          m_data[ r13 ] * mat[ r32 ] + 
          m_data[ X ]   * mat[ _O_ ], 
                          
          m_data[ r11 ] * mat[ r13 ] + 
          m_data[ r12 ] * mat[ r23 ] + 
          m_data[ r13 ] * mat[ r33 ] + 
          m_data[ X ]   * mat[ _O ],
                           
          m_data[ r11 ] * mat[ X ] + 
          m_data[ r12 ] * mat[ Y ] + 
          m_data[ r13 ] * mat[ Z ] + 
          m_data[ X ]   * mat[ _1_ ] );
         
        const Vector4<Scalar> row1(                 
          m_data[ r21 ] * mat[ r11 ] + 
          m_data[ r22 ] * mat[ r21 ] + 
          m_data[ r23 ] * mat[ r31 ] + 
          m_data[ Y ]   * mat[ O_ ],
                           
          m_data[ r21 ] * mat[ r12 ] + 
          m_data[ r22 ] * mat[ r22 ] + 
          m_data[ r23 ] * mat[ r32 ] + 
          m_data[ Y ]   * mat[ _O_ ],
                           
          m_data[ r21 ] * mat[ r13 ] + 
          m_data[ r22 ] * mat[ r23 ] + 
          m_data[ r23 ] * mat[ r33 ] + 
          m_data[ Y ]   * mat[ _O ],
                           
          m_data[ r21 ] * mat[ X ] + 
          m_data[ r22 ] * mat[ Y ] + 
          m_data[ r23 ] * mat[ Z ] + 
          m_data[ Y ]   * mat[ _1_ ] );
                   
        const Vector4<Scalar> row2(                      
          m_data[ r31 ] * mat[ r11 ] + 
          m_data[ r32 ] * mat[ r21 ] + 
          m_data[ r33 ] * mat[ r31 ] + 
          m_data[ Z ]   * mat[   O_],
                           
          m_data[ r31 ] * mat[ r12 ] + 
          m_data[ r32 ] * mat[ r22 ] + 
          m_data[ r33 ] * mat[ r32 ] + 
          m_data[ Z ]   * mat[  _O_],
                           
          m_data[ r31 ] * mat[ r13 ] + 
          m_data[ r32 ] * mat[ r23 ] + 
          m_data[ r33 ] * mat[ r33 ] + 
          m_data[ Z ]   * mat[ _O ],
                           
          m_data[ r31 ] * mat[ X ] + 
          m_data[ r32 ] * mat[ Y ] + 
          m_data[ r33 ] * mat[ Z ] + 
          m_data[ Z ]   * mat[ _1_ ] );
                           
        const Vector4<Scalar> row3(                      
          m_data[  O_ ] * mat[ r11 ] + 
          m_data[ _O_ ] * mat[ r21 ] + 
          m_data[ _O ]  * mat[ r31 ] + 
          m_data[ _1_ ] * mat[ O_ ],
                           
          m_data[  O_ ] * mat[ r12 ] + 
          m_data[ _O_ ] * mat[ r22 ] + 
          m_data[ _O ]  * mat[ r32 ] + 
          m_data[ _1_ ] * mat[ _O_ ],
                           
          m_data[  O_ ] * mat[ r13 ] + 
          m_data[ _O_ ] * mat[ r23 ] + 
          m_data[ _O ]  * mat[ r33 ] + 
          m_data[ _1_ ] * mat[ _O ],
                           
          m_data[  O_ ] * mat[ X ] + 
          m_data[ _O_ ] * mat[ Y ] + 
          m_data[ _O ]  * mat[ Z ] + 
          m_data[ _1_ ] * mat[ _1_ ] );
          
        const Matrix4<Scalar> resultMat( row0,
                                         row1,
                                         row2,
                                         row3 );
       return resultMat;
    }
       
    template <typename Scalar>
    void Matrix4<Scalar>::SetRotation3x3( const Matrix3<Scalar>& rotMat )
    {
        m_data[r11] = rotMat[0]; m_data[r12] = rotMat[1]; m_data[r13] = rotMat[2];
        m_data[r21] = rotMat[3]; m_data[r22] = rotMat[4]; m_data[r23] = rotMat[5];
        m_data[r31] = rotMat[6]; m_data[r32] = rotMat[7]; m_data[r33] = rotMat[8];
    }
    
    template <typename Scalar>
    void Matrix4<Scalar>::SetTranslation(const Vector3<Scalar>& translation)
    {
        m_data[X] = translation.GetX();
        m_data[Y] = translation.GetY();
        m_data[Z] = translation.GetZ();
    }

    template <typename Scalar>
    void Matrix4<Scalar>::SetTranslation(Scalar x, Scalar y, Scalar z)
    {
        m_data[X] = x;
        m_data[Y] = y;
        m_data[Z] = z;
    }
    
    template <typename Scalar>
    const Matrix4<Scalar> Matrix4<Scalar>::CalculateTransformMatrix( const Vector4<Scalar>& position, const Quaternion<Scalar>& orientation )
    {  
        const Scalar ONE = Scalar( 1.0 );
        const Scalar TWO = Scalar( 2.0 );

        m_data[r11] = (ONE - TWO * orientation.GetY() * orientation.GetY() - TWO * orientation.GetZ() * orientation.GetZ());
        m_data[r12] = (TWO * orientation.GetX() * orientation.GetY() + TWO * orientation.GetW() * orientation.GetZ());
        m_data[r13] = (TWO * orientation.GetX() * orientation.GetZ() - TWO * orientation.GetW() * orientation.GetY());
        m_data[X] = position.GetX();

        m_data[r21] = (TWO * orientation.GetX() * orientation.GetY() - TWO * orientation.GetW() * orientation.GetZ());
        m_data[r22] = (ONE - TWO * orientation.GetX() * orientation.GetX() - TWO * orientation.GetZ() * orientation.GetZ());
        m_data[r23] = (TWO * orientation.GetY() * orientation.GetZ() + TWO * orientation.GetW() * orientation.GetX());
        m_data[Y] = position.GetY();

        m_data[r31] = (TWO * orientation.GetX() * orientation.GetZ() + TWO * orientation.GetW() * orientation.GetY());
        m_data[r32] = (TWO * orientation.GetY() * orientation.GetZ() - TWO * orientation.GetW() * orientation.GetX());
        m_data[r33] = (ONE - TWO * orientation.GetX() * orientation.GetX() - TWO * orientation.GetY() * orientation.GetY());
        m_data[Z] = position.GetZ();

        m_data[ O_] = 0;
        m_data[_O_] = 0;
        m_data[_O ] = 0;
        m_data[_1_] = 1;
        
        return *this;
    }
}
#endif //__MATRIX4_H__

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <assert.h>
#include "vector3.h"
#include "vector4.h"

namespace GrapheneMath
{
    // In this implementation Quaternion is represented by a real part r and
    // three dimensional vector part v.
    template <typename Scalar>
    class Quaternion
    {
    private:
        Scalar re;
        Vector3<Scalar> vec;

    public:
        // Default ctor creates identity quaternion.
        Quaternion(const Scalar& x = Scalar{},
                   const Scalar& y = Scalar{},
                   const Scalar& z = Scalar{},
                   const Scalar& w = Scalar{})
            : re(w), vec(x, y, z)
        {}

        Quaternion(const Scalar& w, const Vector3<Scalar>& v)
            :re(w), vec(v)
        {}

        // Explicit ctor creates quaternion from vector4.
        explicit Quaternion(const Vector4<Scalar>& _vec)
            : re(_vec.GetW()), vec(_vec.GetXYZ())
        {}

        inline const Scalar GetX() const { return vec.GetX(); }
        inline const Scalar GetY() const { return vec.GetY(); }
        inline const Scalar GetZ() const { return vec.GetZ(); }

        // Function returns the real part of the quaternion.
        inline const Scalar GetW() const { return re; }

        // Function returns the vector part of the quaternion.
        const Vector3<Scalar> GetXYZ() const { return vec; }

        // Set quaternion with new real and vector parts.
        void Set( Scalar w, const Vector3<Scalar>& v );
        
        // Function creates quaternion form unit axis and angle (in radians).
        void FromAxisAngle( const Vector3<Scalar>& axis, Scalar angle );

        // Function returns angle in radians.
        inline const Scalar GetAngle() const
        {
            return(Scalar(2.0) * atan2(vec.Mag(), re));
        }

        // Multiplication operator which follows exact mathematical definition but
        // becomes inefficient due to excessive usage of dot and cross product vector operators.
        // s = qp = qrpr - q · p + qrp + prq + q × p
        inline const Quaternion<Scalar> operator^( const Quaternion<Scalar>& p ) const;

        // Multiplication operator in its unwrapped and efficient implementation.
        const Quaternion<Scalar> operator*(const Quaternion<Scalar>& p) const;

        // Complex conjugate postfix operator.
        const Quaternion operator~() const
        {
            return Quaternion(re, (vec.ScalarMult(Scalar(-1.0))));
        }

        const Scalar Mag() const
        {
            return (sqrt(vec.GetX() * vec.GetX() +
                         vec.GetY() * vec.GetY() +
                         vec.GetZ() * vec.GetZ() + re*re));
        }

        // Function makes this quaternion a unit length quaternion.
        void Normalise()
        {
            const Scalar magnitude = Mag();
            if(magnitude)
            {
                const Scalar invMagnitude = Scalar(1.0) / magnitude;
                re *= invMagnitude;
                vec = vec.ScalarMult(invMagnitude);
            }
        }

        // Function rotates input vector w with a quaternion i.e. it explicitly applies
        // quaternion rotation operator to that vector.
        // It returns new, rotated vector, extracted from the pure quaternion.
        // Implementation strictly follows mathematical definition, but it is computationally inefficient.
        const Vector3<Scalar> Rotate(const Vector3<Scalar>& w) const;

        // Function rotates input vector with a quaternion in most efficient way.
        // Please see attached 'QuaternionRotationOperator.pdf' paper to see
        // how to derive the efficient formula.
        // w' = e(0+w)e* = (2*r*r - 1)w + 2r(v × w) + 2(v · w)v
        const Vector3<Scalar> RotateFast(const Vector3<Scalar>& w) const;
        
  };  //< End of class.
  
  // Set quaternion with new real and vector parts.
  template <typename Scalar>
  void Quaternion<Scalar>::Set( Scalar w, const Vector3<Scalar>& v )
  { 
      vec = v; re = w;
  }

  // Function creates quaternion form unit axis and angle (in radians).
  template <typename Scalar>
  void Quaternion<Scalar>::FromAxisAngle( const Vector3<Scalar>& axis, Scalar angle )
  {
      assert( axis.IsUnit() );
      angle /= Scalar( 2.0 );
      re = cos( angle );
      vec = axis.ScalarMult( sin ( angle ) );
  }
  
  // Multiplication operator which follows exact mathematical definition but
  // becomes inefficient due to excessive usage of dot and cross product vector operators.
  // s = qp = qrpr - q · p + qrp + prq + q × p
  template <typename Scalar>
  const Quaternion<Scalar> Quaternion<Scalar>::operator^( const Quaternion<Scalar>& p ) const
  {
      return Quaternion(re * p.GetW() - vec.Dot( p.GetXYZ() ),  //< qrpr - q · p
                       ( ( p.GetXYZ() ).ScalarMult( re ) +      //< qrp
                        vec.ScalarMult( p.GetW() ) +            //< prq
                        vec.Cross( p.GetXYZ() ) ) );            //< q × p
  }

  // Multiplication operator in its unwrapped and efficient implementation.
  template <typename Scalar>
  const Quaternion<Scalar> Quaternion<Scalar>::operator*(const Quaternion<Scalar>& p) const
  {
      return Quaternion(
                GetW() * p.GetX() + p.GetW() * vec.GetX() + vec.GetY() * p.GetZ() - vec.GetZ() * p.GetY(),
                GetW() * p.GetY() - vec.GetX() * p.GetZ() + p.GetW() * vec.GetY() + vec.GetZ() * p.GetX(),
                GetW() * p.GetZ() + vec.GetX() * p.GetY() - vec.GetY() * p.GetX() + p.GetW() * vec.GetZ(),
                GetW() * p.GetW() - vec.GetX() * p.GetX() - vec.GetY() * p.GetY() - vec.GetZ() * p.GetZ());
  }
  
  // Function rotates input vector w with a quaternion i.e. it explicitly applies
  // quaternion rotation operator to that vector.
  // It returns new, rotated vector, extracted from the pure quaternion.
  // Implementation strictly follows mathematical definition, but it is computationally inefficient.
  template <typename Scalar>
  const Vector3<Scalar> Quaternion<Scalar>::Rotate(const Vector3<Scalar>& w) const
  {
      // Assuming e has unit length here.
      Quaternion e = *this;
      const Quaternion r( Scalar( 0.0 ), w );
      return ( e * ( r * ( ~e ) ) ).GetXYZ();
  }

  // Function rotates input vector with a quaternion in most efficient way.
  // Please see attached 'QuaternionRotationOperator.pdf' paper to see
  // how to derive the efficient formula.
  // w' = e(0+w)e* = (2*r*r - 1)w + 2r(v × w) + 2(v · w)v
  template <typename Scalar>
  const Vector3<Scalar> Quaternion<Scalar>::RotateFast(const Vector3<Scalar>& w) const
  {
      // Assuming e has unit length here.
      const Scalar exrMult = Scalar( 2.0 ) * re;
      const Scalar rMult = exrMult * re - Scalar( 1.0 );
      const Scalar eMult = Scalar( 2.0 ) * vec.Dot( w );
      return Vector3<Scalar>(
            rMult * w.GetX() + exrMult * (vec.GetY() * w.GetZ() - vec.GetZ() * w.GetY()) + eMult * vec.GetX(),
            rMult * w.GetY() + exrMult * (vec.GetZ() * w.GetX() - vec.GetX() * w.GetZ()) + eMult * vec.GetY(),
            rMult * w.GetZ() + exrMult * (vec.GetX() * w.GetY() - vec.GetY() * w.GetX()) + eMult * vec.GetZ() );
  }

}  //< End of namespace GrapheneMath
#endif //__QUATERNION_H__

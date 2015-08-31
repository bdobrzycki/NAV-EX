#ifndef __DUAL_QUATERNION_H__
#define __DUAL_QUATERNION_H__

#include <assert.h>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include "quat.h"
//#include "vector3.h"
//#include "vector4.h"

namespace GrapheneMath
{
    template <typename Scalar>
    class DualQuaternion
    {
      private:
          Quaternion<Scalar> q0;
          Quaternion<Scalar> qe;

      public:
      // Default ctor creates identity quaternions.
      DualQuaternion()
        : q0( Scalar(1.0), Vector3<Scalar>( 0.0, 0.0, 0.0 ) ), //< identity quaternion
          qe( Scalar(1.0), Vector3<Scalar>( 0.0, 0.0, 0.0 ) )  //< identity quaternion
      {}
      
      inline const Quaternion<Scalar> GetQ0() const { return q0; }
      inline const Quaternion<Scalar> GetQe() const { return qe; }
      
  
      /*DualQuaternion( const Quaternion& _q0, const Quaternion& _qe )
        : q0( _q0 ), qe( _qe ) 
      {}
  
      // Function creates dual quaternion form unit axis, angle (in radians) and translation.
      inline void FromAxisAngleTranslation(
         const Vector3& axis,
         double         angle,
         const Vector3& translation )
      {
        //assert( abs (1.0 - axis.Magnitude() ) < MathTools::UNIT_TOLERANCE );
      
      q0.FromAxisAngle( axis, angle );
      qe.FromRealPartVector( 0.0, translation * 0.5 );
      qe = qe * q0;
     }
  
  // Dual quaternion - dual quaternionMultiplication operator
  // ( non-optimal implementation )
  // @ - dual unit, @^2 = 0
  // d - dual quaternion
  // dq * dp = (q0 + @qe)(p0 + @pe) = q0p0 + @q0pe + @qep0 + @^2qepe
  //                                = q0p0 + @(q0pe + qep0) + 0
  //                                = q0p0 + @(q0pe + qep0)
  inline const DualQuaternion operator*( const DualQuaternion& dp ) const
  {
    return DualQuaternion( q0 * dp.q0, q0 * dp.qe + qe * dp.q0 );
  }
  
  // Dual quaternion - quaternion multiplication operator
  // ( non-potimal implementation )
  // @ - dual unit, @^2 = 0
  // dq * p = (q0 + @qe)p = q0p + @qep
  inline const DualQuaternion operator*( const Quaternion& p ) const
  {
    return DualQuaternion( q0 * p, qe * p );
  }
  
  // Transform - rotates and translates input vector v using this dual quaternion.
  Vector3 Transform( const Vector3& v )
  {
    // Convert vertex to dual quaternion.
    const DualQuaternion dv( Quaternion( 1.0, Vector3( 0.0, 0.0, 0.0 ) ), // q0
                             Quaternion( 0.0, v ) );                      // qe
    const DualQuaternion dq = *this;
    DualQuaternion dqCDC( dq );
    dqCDC = dqCDC.Conjugate();
    dqCDC = dqCDC.DualConjugate();
    return ( dq * ( dv * dqCDC ) ).qe.v;
  }
  
  // Conjugation.
  inline const DualQuaternion Conjugate( void ) const
  {
    return DualQuaternion( ~q0, ~qe );
  }
  
  // Dual conjugation.
  inline const DualQuaternion DualConjugate( void ) const
  {
    return DualQuaternion( q0, Quaternion( qe.GetRealPart() * ( -1.0 ),
                                           qe.GetVectorPart() * ( -1.0 ) ) );
  }
  */
};

  
  
}  //< End of namespace GrapheneMath
#endif //__DUAL_QUATERNION_H__

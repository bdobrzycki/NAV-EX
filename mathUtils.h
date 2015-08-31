#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>

namespace GrapheneMath
{
    // Define the real number precision. GrapheneMath can be compiled in single or
    // double precision versions. By default single precision is provided.
    typedef long double Real;

    template<typename Scalar>
    class Matrix4;
    
    template<typename Scalar>
    class Quaternion;
    
    template<typename Scalar>
    class DualQuaternion;

    static const Real PI = Real(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862);
    static const Real PI_OVER_TWO = Real(1.57079632679489661923132169163975144209858469968755291048747229615390820314310449931);
    static const Real NUM_TOLERANCE = Real(1E-4);
    static const Real ONE = Real(1.0);
    static const Real TWO = Real(2.0);
    static const Real ONE_EIGHTY = Real(180.0);
    static const Real PI_OVER_ONE_EIGHTY = PI / ONE_EIGHTY;
    static const Real ONE_EIGHTY_OVER_PI = ONE_EIGHTY / PI;

    template<typename Scalar>
    const Scalar Deg2Rad( Scalar angleDeg )
    {
        return ( angleDeg * PI_OVER_ONE_EIGHTY );
    }

    template<typename Scalar>
    const Scalar Rad2Deg( Scalar angleRad )
    {
        return ( angleRad * ONE_EIGHTY_OVER_PI );
    }

    template<typename Scalar>
    static void PrintMatrix(const Matrix4<Scalar>& mat)
    {
        std::cout << std::setprecision(3) << std::fixed <<
            "[ " << mat[Matrix4<Scalar>::r11] <<
            " "  << mat[Matrix4<Scalar>::r12] <<
            " "  << mat[Matrix4<Scalar>::r13] <<
            " "  << mat[Matrix4<Scalar>::X] << " ]" << std::endl;

        std::cout << std::setprecision(3) << std::fixed <<
            "[ " << mat[Matrix4<Scalar>::r21] <<
            " "  << mat[Matrix4<Scalar>::r22] <<
            " "  << mat[Matrix4<Scalar>::r23] <<
            " "  << mat[Matrix4<Scalar>::Y] << " ]" << std::endl;

        std::cout << std::setprecision(3) << std::fixed <<
            "[ " << mat[Matrix4<Scalar>::r31] <<
            " "  << mat[Matrix4<Scalar>::r32] <<
            " "  << mat[Matrix4<Scalar>::r33] <<
            " "  << mat[Matrix4<Scalar>::Z] << " ]" << std::endl;

        std::cout << std::setprecision(3) << std::fixed <<
            "[ " << mat[Matrix4<Scalar>::O_]  <<
            " "  << mat[Matrix4<Scalar>::_O_] <<
            " "  << mat[Matrix4<Scalar>::_O]  <<
            " "  << mat[Matrix4<Scalar>::_1_] << " ]" << std::endl;
    }
    
    // Overload output stream operator for dual quaternion.
    // Use it like this: std::cout << dq;
    template <typename Scalar>
    static std::ostream& operator<<( std::ostream& os, const DualQuaternion<Scalar>& dq )
    {
        const Quaternion<Scalar> q0 = dq.GetQ0();
        const Quaternion<Scalar> qe = dq.GetQe();
        
        os << "|  q0 [ r( " << q0.GetW() << " ) , v( "   << q0.GetXYZ().GetX() <<
                                                   ", "  << q0.GetXYZ().GetY() <<
                                                   ", "  << q0.GetXYZ().GetZ() << " ) ]  |\n"
                                                             
           << "|  qe [ r( " << qe.GetW() << " ) , v( "   << qe.GetXYZ().GetX() <<
                                                   ", "  << qe.GetXYZ().GetY() <<
                                                   ", "  << qe.GetXYZ().GetZ() << " ) ]  |" << std::endl;
        return os;
    }
}

#endif //MATH_UTILS_H

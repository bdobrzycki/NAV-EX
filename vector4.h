#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include "mathUtils.h"

namespace GrapheneMath
{
    // Four dimensional vector class.
    template <typename Scalar>
    class Vector4
    {
        // Holds vector x, y, z, w components.

        Scalar x, y, z, w;

    public:
        Vector4(const Scalar& _x = Scalar{},
                const Scalar& _y = Scalar{},
                const Scalar& _z = Scalar{},
                const Scalar& _w = Scalar{})
            : x(_x), y(_y), z(_z), w(_w)
        {}

        inline void SetX(Scalar _x) { x = _x; }
        inline void SetY(Scalar _y) { y = _y; }
        inline void SetZ(Scalar _z) { z = _z; }
        inline void SetW(Scalar _w) { w = _w; }

        inline const Scalar GetX() const { return x; }
        inline const Scalar GetY() const { return y; }
        inline const Scalar GetZ() const { return z; }
        inline const Scalar GetW() const { return w; }

        // Setter function that sets vector x, y, z, w components.
        void SetXYZW(Scalar _x, Scalar _y, Scalar _z, Scalar _w)
        {
            x = _x; y = _y; z = _z; w = _w;
        }

        const Vector4<Scalar> operator+(const Vector4<Scalar>& vec) const;
        const Vector4<Scalar> operator+=(const Vector4<Scalar>& vec);
        const Vector4<Scalar> operator-(const Vector4<Scalar>& vec) const;
        const Vector4<Scalar> operator-=(const Vector4<Scalar>& vec);
        const Vector4<Scalar> ScalarMult(Scalar scalar) const;
      
        // Vector scalar product operator.
        const Scalar Dot(const Vector4<Scalar>& vec) const;
      
        const Scalar Mag() const
        {
            return (sqrt(x*x + y*y + z*z + w*w));
        }

        void Normalise()
        {
            const Scalar mag = Mag();
            if(mag)
            {
                x /= mag;
                y /= mag;
                z /= mag;
                w /= mag;
            }
        }

        bool IsUnit() const
        {
            return (abs(Mag() - ONE) < NUM_TOLERANCE) ? true : false;
        }
    };  //< End of Vector4 class.
    
    template <typename Scalar>
    const Vector4<Scalar> Vector4<Scalar>::operator+(const Vector4<Scalar>& vec) const
    {
       return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
    }

    template <typename Scalar>
    const Vector4<Scalar> Vector4<Scalar>::operator+=(const Vector4<Scalar>& vec)
    {
        x += vec.x; y += vec.y; z += vec.z, w += vec.w;
        return *this;
    }

    template <typename Scalar>
    const Vector4<Scalar> Vector4<Scalar>::operator-(const Vector4<Scalar>& vec) const
    {
        return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
    }

    template <typename Scalar>
    const Vector4<Scalar> Vector4<Scalar>::operator-=(const Vector4<Scalar>& vec)
    {
        x -= vec.x; y -= vec.y; z -= vec.z, w -= vec.w;
        return *this;
    }

    template <typename Scalar>
    const Vector4<Scalar> Vector4<Scalar>::ScalarMult(Scalar scalar) const
    {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Vector scalar product operator.
    template <typename Scalar>
    const Scalar Vector4<Scalar>::Dot(const Vector4<Scalar>& vec) const
    {
        return (x * vec.x + y * vec.y + z * vec.z + w * vec.w);
    }
        
}  //< End of GrapheneMath namespace.
#endif // __VECTOR4_H__

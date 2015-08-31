#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "mathUtils.h"

namespace GrapheneMath
{
    template <typename Scalar>
    class Vector3
    {
    private:
        Scalar x, y, z;

    public:
        Vector3(const Scalar& _x = Scalar{},
                const Scalar& _y = Scalar{},
                const Scalar& _z = Scalar{})
            : x(_x), y(_y), z(_z)
        {}

        inline void SetX(Scalar _x) { x = _x; }
        inline void SetY(Scalar _y) { y = _y; }
        inline void SetZ(Scalar _z) { z = _z; }

        inline const Scalar GetX() const { return x; }
        inline const Scalar GetY() const { return y; }
        inline const Scalar GetZ() const { return z; }

        // Setter function that sets vector x, y, z, w components.
        void SetXYZ(Scalar _x, Scalar _y, Scalar _z)
        {
            x = _x; y = _y; z = _z;
        }
        
        const Vector3<Scalar> operator+(const Vector3<Scalar>& vec) const;
        const Vector3<Scalar> operator+=(const Vector3<Scalar>& vec);
        const Vector3<Scalar> operator-(const Vector3<Scalar>& vec) const;
        const Vector3<Scalar> operator-=(const Vector3<Scalar>& vec);
        const Vector3<Scalar> ScalarMult(Scalar scalar) const;
        const Scalar Dot(const Vector3<Scalar>& vec) const;
        const Vector3<Scalar> Cross(const Vector3<Scalar>& vec) const;
    
        const Scalar Mag() const
        {
            return (sqrt(x*x + y*y + z*z));
        }

        void Normalise()
        {
            const Scalar mag = Mag();
            if (mag)
            {
                x /= mag;
                y /= mag;
                z /= mag;
            }
        }

        bool IsUnit() const
        {
            return (abs(Mag() - ONE) < NUM_TOLERANCE) ? true : false;
        }

        // Numerically stable way to calculate the angle between
        // two vectors.
        const Scalar GetAngle(const Vector3<Scalar>& b) const;
    };
    
    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::operator+(const Vector3<Scalar>& vec) const
    {
        return Vector3(x + vec.x, y + vec.y, z + vec.z);
    }

    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::operator+=(const Vector3<Scalar>& vec)
    {
        x += vec.x; y += vec.y; z += vec.z;
        return *this;
    }
    
    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::operator-(const Vector3<Scalar>& vec) const
    {
        return Vector3(x - vec.x, y - vec.y, z - vec.z);
    }
    
    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::operator-=(const Vector3<Scalar>& vec)
    {
        x -= vec.x; y -= vec.y; z -= vec.z;
        return *this;
    }

    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::ScalarMult(Scalar scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Vector scalar product operator.
    template <typename Scalar>
    const Scalar Vector3<Scalar>::Dot(const Vector3<Scalar>& vec) const
    {
        return (x * vec.x + y * vec.y + z * vec.z);
    }

    // Vector cross product operator.
    template <typename Scalar>
    const Vector3<Scalar> Vector3<Scalar>::Cross(const Vector3<Scalar>& vec) const
    {
       return Vector3(y * vec.z - z * vec.y,
                      z * vec.x - x * vec.z,
                      x * vec.y - y * vec.x);
    }
    
    template <typename Scalar>
    const Scalar Vector3<Scalar>::GetAngle(const Vector3<Scalar>& b) const
    {
        const Vector3& a = *this;

        assert(a.IsUnit());
        assert(b.IsUnit());

        if (a.Dot(b) < Scalar(0.0))
        {
            return Scalar(PI) - Scalar(TWO) * asin((b.ScalarMult(Scalar(-1.0)) - a).Mag() / Scalar(TWO));
        }
        else
        {
            return Scalar(TWO) * asin((b - a).Mag() / Scalar(TWO));
        }
    }

}  //< End of GrapheneMath namespace.
#endif // __VECTOR3_H__

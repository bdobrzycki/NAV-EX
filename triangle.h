#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <assert.h>
#include <GLFW/glfw3.h>

// Math
#include "mathUtils.h"
#include "quat.h"

using namespace GrapheneMath;

struct TriangleOfVelocities
{
    // directions in DEG T
    // speeds in kts
    double  TR;  //< track (DEG T)
    double  TAS; //< true airspeed (kts)
    double  W;   //< wind direction (DEG T, direction FROM the wind is blowing)
    double  V;   //< wind speed (kts)
    double  HDG; //< heading (DEG T)
    double  GS;  //< ground speed (kts)

    TriangleOfVelocities()
        : TR ( 0.0f )
        , TAS( 0.0f )
        , W  ( 0.0f )
        , V  ( 0.0f )
        , HDG( 0.0f )
        , GS ( 0.0f )
    {}
};

class TriangleOfVelocitiesSolver
{
  public:
    static const double THREE_SIXTY_DEG;
    static const double ONE_EIGHTY_DEG;

    enum DataFields : uint8_t
    {
        eHDG = 1 << 5,
        eTAS = 1 << 4,
        eW   = 1 << 3,
        eV   = 1 << 2,
        eTR  = 1 << 1,
        eGS  = 1 << 0
    };

    // | 0 | 0 |HDG|TAS| W | V | TR| GS| (8 bit mask)
    // | 0 | 0 | 0 | 1 | 1 | 1 | 1 | 0 | (00011110)   //< most common example 
                                                      //< vector (W/V), dir(TR), length(TAS)
    uint8_t m_mask;

public:

    TriangleOfVelocitiesSolver();

    void solve( TriangleOfVelocities& triangle );
    double invertWind( double windFromDeg );
    double invertAngle( double angleDeg ) const;
    
private:
    void solveVecDirLength();
    void solveVecDirLength2();
    bool isSane( const TriangleOfVelocities& tov );

};

#endif //__TRIANGLE_H__

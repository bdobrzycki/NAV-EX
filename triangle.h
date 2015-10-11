#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <assert.h>
#include <GLFW/glfw3.h>

// Math
#include "mathUtils.h"

using namespace GrapheneMath;

class TriangleOfVelocities
{
    static const double THREE_SIXTY_DEG;
    static const double ONE_EIGHTY_DEG;
    
    struct TriangleData
    {
        // directions in DEG T
        // speeds in kts
        float TR;  //< track (DEG T)
        float TAS; //< true airspeed (kts)
        float W;   //< wind direction (DEG T, direction FROM the wind is blowing)
        float V;   //< wind speed (kts)
        float HDG; //< heading (DEG T)
        float GS;  //< ground speed (kts)

        TriangleData()
        : TR (0.0f)
        , TAS(0.0f)
        , W  (0.0f)
        , V  (0.0f)
        , HDG(0.0f)
        , GS (0.0f)
        {}

    }  m_data;
    
    enum DataFields :  uint8_t
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
    TriangleOfVelocities();

    void getData();
    void solve();
    double invertWind( double windFromDeg );
    double invertAngle( double angleDeg );
    
private:
    void solveVecDirLength();
    void solveVecDirLength2();
    void test();

};

#endif //__TRIANGLE_H__

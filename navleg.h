#ifndef __NAVLEG_H__
#define __NAVLEG_H__

#include "xlocator.h"
#include "varrow2d.h"

class NavLeg
{
private:
    Vector3<float> m_startPos;
    Vector3<float> m_endPos;
    float          m_TR;         //< angle in DEG M
    float          m_distanceNM; //< NM
    
    // Debug graphics
    const Vector3<GLfloat> c_colorGreen;
    XLocator               m_startLocator;
    XLocator               m_endLocator;
    GLLine                 m_leg;
    Varrow2D               m_vArrow1;
    Varrow2D               m_vArrow2;
    
public:
     NavLeg( Vector3<float> startPos,
             float          TR,
             float          distanceNM );
     
     const Vector3<float> getStartPos( void ) const { return m_startPos; }
     const Vector3<float> getEndPos( void ) const { return m_endPos; }
     float                getTrack( void ) const { return m_TR; }
     
     void Draw();
};

#endif //__NAVLEG_H__
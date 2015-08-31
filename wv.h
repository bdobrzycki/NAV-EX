#ifndef __VW_H__
#define __VW_H__

#include <assert.h>
#include <GLFW/glfw3.h>

// Math
#include "mathUtils.h" //< Graphene Math - nowy kod!
#include "quat.h"
#include "matrix4.h"
#include "glLine.h"
#include "varrow2d.h"

using namespace GrapheneMath;

class WV //< wind vector
{
private:
  const Vector3<GLfloat>    c_colorBlue;
  GLLine                    m_line;
  static const unsigned int c_numOfVArrows = 3;
  std::vector<Varrow2D>     m_vArrows;
  Vector3<float>            m_wind;
  float                     c_dirDegM;
   
public:
   WV( float dirDegM, float speedKts ); // dirDegM - from where the wind is blowing, deg magnetic
   
   const Vector3<float> GetWV( void ) const { return m_wind; }  // kts = NM / H
   const float GetDirectionDeg( void ) const { return c_dirDegM; } // kts = NM / H
   const float GetMaxDriftAngleDeg( float speedKt );
   void Draw() const; 
};


#endif //__VW_H__

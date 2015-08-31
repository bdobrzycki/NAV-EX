#ifndef __VARROW2D_H__
#define __VARROW2D_H__

#include <assert.h>
#include <GLFW/glfw3.h>

#include "mathUtils.h"
#include "quat.h"
#include "glLine.h"

using namespace GrapheneMath;

class Varrow2D
{
  std::vector<GLLine>       m_lines;
  static const unsigned int c_numOfLines = 2;
  
public:
  explicit Varrow2D();
  
  
  void Set( const Vector3<float>&   pos,
            const Vector3<float>&   dir,
            const Vector3<float>&   planeNormal,
            const Vector3<GLfloat>& color,
            float length );
  
  void Draw() const;
};

#endif //__VARROW2D_H__

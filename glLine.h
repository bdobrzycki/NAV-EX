#ifndef __GLLINE_H__
#define __GLLINE_H__

#include <assert.h>

#include <GLFW/glfw3.h>
//#include "mathUtils.h" //< Graphene Math v.11
#include "vector3.h"

using namespace GrapheneMath;

// Color line for debug drawing.
class GLLine
{
private:
  static const unsigned int dim = 3; 
  
  std::vector< GLfloat > from;
  std::vector< GLfloat > to;
  std::vector< GLfloat > color;
  
  inline void Initialize()
  {
    from.resize( dim );
    to.resize( dim );
    color.resize( dim );
  }

public:
  explicit GLLine()
  {
    Initialize();
    const Vector3< GLfloat > zero(0.0, 0.0, 0.0);
    Set( zero, zero, zero );
  }
  
  explicit GLLine( const Vector3< GLfloat >& from,
                   const Vector3< GLfloat >& to,
                   const Vector3< GLfloat >& color)
  {
    Initialize();
    Set( from, to, color );
  }
  
  inline void SetFrom( const Vector3< GLfloat >& from )
  {
    GLLine* const line = this;
    line->from[0] = from.GetX();
    line->from[1] = from.GetY();
    line->from[2] = from.GetZ();
  }
  
  inline void SetTo(const Vector3<GLfloat>& to)
  {
    GLLine* const line = this;
    line->to[0] = to.GetX();
    line->to[1] = to.GetY();
    line->to[2] = to.GetZ();
  }
  
  inline void SetColor(const Vector3<GLfloat>& color)
  {
    GLLine* const line = this;
    line->color[0] = color.GetX();
    line->color[1] = color.GetY();
    line->color[2] = color.GetZ();
  }
    
  void Set(const Vector3<GLfloat>& from, const Vector3<GLfloat>& to, const Vector3<GLfloat>& color)
  {
    GLLine* const line = this;
    line->SetFrom( from );
    line->SetTo( to );
    line->SetColor( color );
  }
  
  inline Vector3< GLfloat > GetFrom() { return Vector3<GLfloat>(from[0], from[1], from[2]); }
  inline Vector3< GLfloat > GetTo() { return Vector3<GLfloat>(to[0], to[1], to[2]); }
  
  void Draw() const
  {
    glColor3f( color[0], color[1], color[2] );
    glBegin( GL_LINES );
    glVertex3f( from[0], from[1], from[2] );
    glVertex3f( to[0], to[1], to[2] );
    glEnd();
  }
};
#endif //__GLLINE_H__

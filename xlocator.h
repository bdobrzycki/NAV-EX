#ifndef __XLOCATOR_H__
#define __XLOCATOR_H__

#include <assert.h>
#include <GLFW/glfw3.h>

// Math
#include "mathUtils.h" //< Graphene Math - nowy kod!
#include "quat.h"
#include "matrix4.h"
#include "glLine.h"

using namespace GrapheneMath;

// Cross locator for debug drawing.
class XLocator
{
  std::vector<GLLine> X; // stores GLLines
  static const unsigned int numOfLines = 3;
  float radius;
  std::vector< Vector3<float> > offsets; 
  
  void Initialise();
  void Set( const Vector3<float>& pos, float radius, const Vector3<float>& color);
  void UpdateOffsetsFromRadius( float radius );
  
public:

  explicit XLocator();
  explicit XLocator( const Vector3<float>& pos, float _radius, const Vector3<float>& color );
    
  void SetPosition( const Vector3<float>& pos );
  void SetRadius( float radius );
  void SetColor( const Vector3<float>& color );
  void Draw() const;
};

#endif //__XLOCATOR_H__

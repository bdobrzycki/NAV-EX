#include "varrow2d.h"

using namespace GrapheneMath;

Varrow2D::Varrow2D( )
{
    m_lines.resize( c_numOfLines );
}
  
void Varrow2D::Set( const Vector3<float>&   pos,
                    const Vector3<float>&   dir,
                    const Vector3<float>&   planeNormal,
                    const Vector3<GLfloat>& color,
                    float length )
  { 
      const Vector3<float>& opposiDir = dir.ScalarMult( -1.0f );
      Quaternion<float> rot;
      
      rot.FromAxisAngle( planeNormal, Deg2Rad( 45.0f ) );
      Vector3<float> v = rot.RotateFast( opposiDir );
      m_lines[0].SetFrom( pos );
      m_lines[0].SetTo( pos + v.ScalarMult( length ) );
      m_lines[0].SetColor( color );
      
      rot.FromAxisAngle( planeNormal, Deg2Rad( -45.0f ) );
      v = rot.RotateFast( opposiDir );
      m_lines[1].SetFrom( pos );
      m_lines[1].SetTo( pos + v.ScalarMult( length ) );
      m_lines[1].SetColor( color );
  }
  
  void Varrow2D::Draw() const
  { 
    for( unsigned int i = 0; i < c_numOfLines; ++i ) {
        m_lines[i].Draw();
    }
  }
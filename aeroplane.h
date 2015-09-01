#ifndef __AEROPLANE_H__
#define __AEROPLANE_H__

#include <assert.h>
#include <vector>

#include <GLFW/glfw3.h>

#include "matrix4.h"
#include "vector3.h"
#include "quat.h"
#include "glLine.h"

using namespace GrapheneMath;

class AeroplaneRenderModel
{
private:
    typedef Vector3<float>    Vec3;
    typedef Vector3<GLfloat>  Vec3GL;
    typedef Quaternion<float> Quat;

    struct Colors //< Helper struct to keep the enum scoped.
    {
      enum
      {
        eRed   = 0,
        eGreen,
        eBlue,
        eWhite,
        eMaxNumOfCols
      };
    };
    
    struct Propeller
    {
      std::vector< Vec3 >     m_verticesL; //< Read only.
      std::vector< Vec3 >     m_vertices;  //< Transformed.
      GLLine                  m_line; 
      Vec3                    m_pos;      //< in local aeroplane space
      Quat                    m_propOrient;
      const Vec3              c_propRotAxis;
      Quat                    m_propRevs;
      
      Propeller( float scale )
       : m_pos( 4.0f, 0.0f, 0.0f )
       , m_propOrient( 0.0f, 0.0f, 0.0f, 1.0f ) 
       , c_propRotAxis( 1.0f, 0.0f, 0.0f )
      {
        m_verticesL.resize( 0 );      
        // Propeller blades
        m_pos = m_pos.ScalarMult( scale );
        m_verticesL.push_back( (Vec3 ( 0.0f, 0.0f,  2.5f )).ScalarMult( scale ) );
        m_verticesL.push_back( (Vec3 ( 0.0f, 0.0f, -2.5f )).ScalarMult( scale ) );
        
        m_vertices.resize( m_verticesL.size() );
          
        m_propRevs.FromAxisAngle( c_propRotAxis, Deg2Rad( 20.0f ) );
      }
      
      void Spin()
      {
        // Update orientation. 
        m_propOrient = m_propOrient * m_propRevs;
        m_propOrient.Normalise();
        
        // Transform vertices.
        for( size_t i = 0; i < m_verticesL.size(); ++i ) {
            m_vertices[i] = m_propOrient.RotateFast( m_verticesL[i] );
        }  
      }
      
      void Draw() const { m_line.Draw(); }
    } m_prop;
    
    std::vector< Vec3 >     m_vertices; //< vertices in local space.  
    std::vector< GLLine >   m_lines;    //< GLLines buffer in Global space.
    std::vector< Vec3GL >   m_colors;

    // Fills in the m_vertices buffer.
    void GenerateVertices( float scale );
    void MakeColors();

public:
    AeroplaneRenderModel( float scale );

    void Update( const Vec3& pos, const Quat& orient );
    void Draw() const;
};


class Aeroplane
{
private:
  Vector3<float>       m_pos;
  Vector3<float>       m_vel;
  Quaternion<float>    m_orient;
  Matrix4<float>       m_tm;
  AeroplaneRenderModel m_renderModel;

  void SyncRenderModel();

public:
  Aeroplane();

  void SetHDG( float HDG_DEG );
  void SetOrientation( const Quaternion<float>& orient ) { m_orient = orient; }
  void SetPosition( const Vector3<float>& pos ) { m_pos = pos; }
  void SetVelocity( const Vector3<float>& vel ) { m_vel = vel; }

  const Vector3<float> GetPosition() const { return m_pos; }
  const Vector3<float> GetVelocity() const { return m_vel; }
  const Quaternion<float> GetOrientation() const { return m_orient; }
  const Matrix3<float> GetOrientationMatrix() const;
  const Matrix4<float> GetTransformMatrix() const;

  void AddRotation( const Quaternion<float>& rotation );

  void Draw();
};

#endif //__AEROPLANE_H__

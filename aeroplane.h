#ifndef __AEROPLANE_H__
#define __AEROPLANE_H__

#include <assert.h>
#include <vector>

#include <GLFW/glfw3.h>

#include "matrix4.h"
#include "vector3.h"
#include "quat.h"

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
    void GenerateVertices( float scale )
    {
      m_vertices.resize( 0 );
      
      // Fuselage
      m_vertices.push_back( ( Vec3 (4.0f, 0.0f, -0.5f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (4.0f, 0.0f,  0.5f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (2.0f, 0.0f,  1.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  1.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  6.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, 6.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, 1.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, 0.5f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, 2.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-8.0f, 0.0f, 2.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-8.0f, 0.0f, -2.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, -2.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, -0.5f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, -1.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, -6.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  -6.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  -1.0f) ).ScalarMult( scale ) );
      m_vertices.push_back( ( Vec3 (2.0f, 0.0f,  -1.0f) ).ScalarMult( scale ) );
    }
       
    void MakeColors()
    {
      m_colors.resize( Colors::eMaxNumOfCols );
      
      m_colors[ Colors::eRed ]   = Vec3GL( 1.0f, 0.0f, 0.0f );
      m_colors[ Colors::eGreen ] = Vec3GL( 0.0f, 1.0f, 0.0f );
      m_colors[ Colors::eBlue ]  = Vec3GL( 0.0f, 0.0f, 1.0f );
      m_colors[ Colors::eWhite ] = Vec3GL( 1.0f, 1.0f, 1.0f );
    }
    
public:
    AeroplaneRenderModel( float scale )
     : m_prop( scale )
    {
        MakeColors();
        GenerateVertices( scale );
        m_lines.resize( m_vertices.size() );
        Update( Vec3(0.0f, 0.0f, 0.0f), Quat( 0.0f, 0.0f, 0.0f, 1.0f ) ); //< identity quaternion)
    }
    
    void Update( const Vec3& pos, const Quat& orient )
    {     
        const Quat orientCnj = ~orient;
        Vec3 from, to; //< Global space
        
        const size_t size = m_vertices.size();
        
        for( size_t i = 0; i < (size + 1); ++i )
        {
          // Rotate.
          from = orientCnj.RotateFast( m_vertices[ i % size ] );
          to   = orientCnj.RotateFast( m_vertices[ (i + 1) % size ] );
          
          // Translate.
          from += pos;
          to   += pos;
          
          m_lines[ i % size ] = GLLine( from, to, m_colors[ Colors::eWhite ] );
        }
        
        // Prop    
        m_prop.Spin();
        // Rotation
        from = orientCnj.RotateFast( m_prop.m_vertices[ 0 ] );
        to   = orientCnj.RotateFast( m_prop.m_vertices[ 1 ] );
        // translation
        from += (pos + orientCnj.RotateFast( m_prop.m_pos ));
        to   += (pos + orientCnj.RotateFast( m_prop.m_pos ));
        m_prop.m_line = GLLine( from, to, m_colors[ Colors::eWhite ] );
        
    }
    
    void Draw() const
    {
       for( size_t i = 0; i < m_lines.size(); ++i ) {
            m_lines[ i ].Draw();
       }
       
       m_prop.Draw();
    }
};


class Aeroplane
{
private:
  Vector3<float>       m_pos;
  Quaternion<float>    m_orient;
  Matrix4<float>       m_tm;
  AeroplaneRenderModel m_renderModel;
  
  void SyncRenderModel()
  {
     m_renderModel.Update( m_pos, m_orient );  
  }    
  
public:
  Aeroplane()
    : m_pos( 0.0f, 0.0f, 0.0f )
    , m_orient( 0.0f, 0.0f, 0.0f, 1.0f ) //< identity quaternion
    , m_renderModel( 0.3f )
  {
  }
  
  void SetHDG( float HDG_DEG )
  {
      const Vector3<float> upDir( 0.0f, 1.0f, 0.0f );
      m_orient.FromAxisAngle( upDir, Deg2Rad( HDG_DEG ) );
  }
  
  void AddRotation( const Quaternion<float>& rotation )
  {
      m_orient = m_orient * rotation;
  }
  
  void SetOrientation( const Quaternion<float>& orient ) { m_orient = orient; }
  void SetPosition( const Vector3<float>& pos ) { m_pos = pos; }
  
  const Vector3<float> GetPosition( void ) const { return m_pos; }
  
  const Quaternion<float> GetOrientation( void ) const
  {
      return m_orient;
  }
  
  const Matrix3<float> GetOrientationMatrix( void ) const
  {
      Matrix3<float> orient;
      orient.FromQuat( m_orient );
      return orient;
  }
  
  const Matrix4<float> GetTransformMatrix( void ) const
  {
      Matrix4<float> tm;
      tm.CalculateTransformMatrix( Vector4<float>( m_pos.GetX(),
                                                   m_pos.GetY(),
                                                   m_pos.GetZ(), 0.0f), m_orient );
      return tm;
  }
  
  void Draw()
  {
     SyncRenderModel();
     m_renderModel.Draw();   
  }
  
};

#endif //__AEROPLANE_H__

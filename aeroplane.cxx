#include "aeroplane.h"

using namespace GrapheneMath;

// Fills in the m_vertices buffer.
void AeroplaneRenderModel::GenerateVertices( float scale )
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
    m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, 2.0f) ).ScalarMult( scale ) );     m_vertices.push_back( ( Vec3 (-8.0f, 0.0f, 2.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (-8.0f, 0.0f, -2.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, -2.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (-7.0f, 0.0f, -0.5f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, -1.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (-2.0f, 0.0f, -6.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  -6.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (1.0f, 0.0f,  -1.0f) ).ScalarMult( scale ) );
    m_vertices.push_back( ( Vec3 (2.0f, 0.0f,  -1.0f) ).ScalarMult( scale ) );
}

void AeroplaneRenderModel::MakeColors()
{
    m_colors.resize( Colors::eMaxNumOfCols );
    
    m_colors[ Colors::eRed ]   = Vec3GL( 1.0f, 0.0f, 0.0f );
    m_colors[ Colors::eGreen ] = Vec3GL( 0.0f, 1.0f, 0.0f );
    m_colors[ Colors::eBlue ]  = Vec3GL( 0.0f, 0.0f, 1.0f );
    m_colors[ Colors::eWhite ] = Vec3GL( 1.0f, 1.0f, 1.0f );
}

AeroplaneRenderModel::AeroplaneRenderModel( float scale )
    : m_prop( scale )
{
      MakeColors();
      GenerateVertices( scale );
      m_lines.resize( m_vertices.size() );
      Update( Vec3(0.0f, 0.0f, 0.0f), Quat( 0.0f, 0.0f, 0.0f, 1.0f ) ); //< identity quaternion)
}

void AeroplaneRenderModel::Update( const Vec3& pos, const Quat& orient )
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

void AeroplaneRenderModel::Draw() const
{
    for( size_t i = 0; i < m_lines.size(); ++i )
    {
        m_lines[ i ].Draw();
    }

    m_prop.Draw();
}

Aeroplane::Aeroplane()
    : m_pos( 0.0f, 0.0f, 0.0f )
    , m_orient( 0.0f, 0.0f, 0.0f, 1.0f ) //< identity quaternion
    , m_renderModel( 0.3f )
{
}

void Aeroplane::SyncRenderModel()
{
    m_renderModel.Update( m_pos, m_orient );  
}


void Aeroplane::SetHDG( float HDG_DEG )
{
    const Vector3<float> upDir( 0.0f, 1.0f, 0.0f );
    m_orient.FromAxisAngle( upDir, Deg2Rad( HDG_DEG ) );
}

void Aeroplane::AddRotation( const Quaternion<float>& rotation )
{
    m_orient = m_orient * rotation;
}

const Matrix3<float> Aeroplane::GetOrientationMatrix( void ) const
{
    Matrix3<float> orient;
    orient.FromQuat( m_orient );
    return orient;
}

const Matrix4<float> Aeroplane::GetTransformMatrix( void ) const
{
    Matrix4<float> tm;
    tm.CalculateTransformMatrix( Vector4<float>( m_pos.GetX(),
                                                 m_pos.GetY(),
                                                 m_pos.GetZ(), 0.0f), m_orient );
      return tm;
}

void Aeroplane::Draw()
{
    SyncRenderModel();
    m_renderModel.Draw();
}

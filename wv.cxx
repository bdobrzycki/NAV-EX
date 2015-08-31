#include "wv.h"

//using namespace GrapheneMath;

WV::WV( float dirDegM, float speedKts ) // dirDegM - from where the wind is blowing, deg magnetic
   : c_colorBlue( 0.0f, 0.0f, 1.0f )
   , c_dirDegM( dirDegM )
   {
       const Vector3<float> dirNorth( 1.0f, 0.0f, 0.0f ); //< North is x axis.
       const Vector3<float> downDir( 0.0f, -1.0f, 0.0f ); //< Clockwise rotation.
       Quaternion<float> rot;
       rot.FromAxisAngle( downDir, Deg2Rad( c_dirDegM ) );
       m_wind = rot.RotateFast( dirNorth );
       m_wind = m_wind.ScalarMult( speedKts );
       
       // At this point the wind direction is 'from where the wind is blowing'.
       // To use it as velocity vector for dynamics calculations 
       // it's direction needs to be swapped to the opposite to make it indicate 
       // 'where the wind blows'.
       m_wind = m_wind.ScalarMult( -1.0f );
       
       // debug draw
       m_vArrows.resize( c_numOfVArrows );
       const Vector3<float> pos( 10.0f, 0.0f, 0.0f );
       Vector3<float> dir = m_wind;
       dir.Normalise();
       const float drawScale = 5.0f;
       dir = dir.ScalarMult( drawScale ) ; //<< wind not in scale
       m_line.Set( pos, pos + dir, c_colorBlue );
       m_vArrows[0].Set( pos + dir.ScalarMult(0.4f), dir, downDir.ScalarMult( -1.0f ), c_colorBlue, drawScale * 0.05f );
       m_vArrows[1].Set( pos + dir.ScalarMult(0.6f), dir, downDir.ScalarMult( -1.0f ), c_colorBlue, drawScale * 0.05f );
       m_vArrows[2].Set( pos + dir.ScalarMult(0.8f), dir, downDir.ScalarMult( -1.0f ), c_colorBlue, drawScale * 0.05f );  
   }
   
   const float WV::GetMaxDriftAngleDeg( float speedKt )
   {
      // 1st way to calc - analiticly
      return Rad2Deg( atan( ( m_wind.Mag() / speedKt) ));
      // flying 60kt with x wind 1kt the drift angle = 1 DEG
      //return ( m_wind.Mag() / speedKt ) * 60.0f;
   }
 
   void WV::Draw() const
   { 
      m_line.Draw();
      for( unsigned int i = 0; i < c_numOfVArrows; ++i ) {
          m_vArrows[i].Draw();
      }
   }
   
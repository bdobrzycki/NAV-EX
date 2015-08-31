#include "navleg.h"

NavLeg::NavLeg( Vector3<float> startPos,
             float          TR,
             float          distanceNM )
           : m_startPos( startPos )
           , m_TR( TR )
           , m_distanceNM( distanceNM )
           , c_colorGreen( 0.0f, 1.0f, 0.0 )
     {
         const Vector3<float> dirN( 1.0f, 0.0f, 0.0f );
         const Vector3<float> downDir( 0.0f, -1.0f, 0.0f );
         Quaternion<float> rotToTR;
         rotToTR.FromAxisAngle( downDir, Deg2Rad( static_cast<float>( m_TR ) ) );
         m_endPos = rotToTR.RotateFast( dirN );
         m_endPos = m_endPos.ScalarMult( m_distanceNM );
         /*std::cout << "end " << m_endPos.GetX()
                   <<  " "   << m_endPos.GetY()
                   << " "    << m_endPos.GetZ() << "\n";*/
         m_endPos += m_startPos;
         
         // Graphics
         m_startLocator.SetPosition( m_startPos );
         m_startLocator.SetRadius( 1.0f );
         m_startLocator.SetColor( c_colorGreen );
         
         m_endLocator.SetPosition( m_endPos );
         m_endLocator.SetRadius( 1.0f );
         m_endLocator.SetColor( c_colorGreen );
         
         m_leg.Set( m_startPos, m_endPos, c_colorGreen );
         
         const Vector3<float> halfPos = m_startPos + (m_endPos - m_startPos).ScalarMult(0.5f);
         Vector3<float> dir = m_endPos - m_startPos;
         dir.Normalise();
         m_vArrow1.Set( halfPos + dir.ScalarMult(m_distanceNM * 0.1f), dir, downDir.ScalarMult( -1.0f ), c_colorGreen, m_distanceNM * 0.1f);     
         m_vArrow2.Set( halfPos - dir.ScalarMult(m_distanceNM * 0.1f), dir, downDir.ScalarMult( -1.0f ), c_colorGreen, m_distanceNM * 0.1f);     
     }
     
     void NavLeg::Draw()
     {
         m_startLocator.Draw();
         m_endLocator.Draw();
         m_leg.Draw();
         m_vArrow1.Draw();
         m_vArrow2.Draw();
     }
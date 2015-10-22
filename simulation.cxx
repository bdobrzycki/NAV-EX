#include <cstdlib> //rand
#include "simulation.h" 

using namespace GrapheneMath;

Simulation::Simulation()
    :  zeroVec( 0.0f, 0.0f, 0.0f )
    ,  xAxis( 10.0f, 0.0f, 0.0f )
    ,  yAxis( 0.0f, 10.0f, 0.0f )
    ,  zAxis( 0.0f, 0.0f, 10.0f )
    ,  colorRed(   1.0f, 0.0f, 0.0f )
    ,  colorGreen( 0.0f, 1.0f, 0.0f )
    ,  colorBlue(  0.0f, 0.0f, 1.0f )
    ,  colorCyan(  0.0f, 1.0f, 1.0f )
    ,  colorWhite( 1.0f, 1.0f, 1.0f )
    ,  xAxisLine( zeroVec, xAxis, colorRed )
    ,  yAxisLine( zeroVec, yAxis, colorGreen )
    ,  zAxisLine( zeroVec, zAxis, colorBlue )
    ,  m_leg01( Vector3<float>( 0.0f, 0.0f, 0.0f ), 45.0f, 27.0f )
    ,  m_time( 0.0f )
    ,  m_timeDelta(0.0f)
    ,  m_dsp(' ')
{
}

void Simulation::Initialise()
{
      m_atmosphereModel.PrintAtmosphericConditions( m_convert.FeetToMeters( 2700.0f ) );

      // set up simulation

      // Generte random wind.
      m_triangle.W = (rand()/(float)(RAND_MAX)) * 360.0f; // from DEG
      m_triangle.V =(rand()/(float)(RAND_MAX)) * 50.0f;  // kts

      m_triangle.TAS = (rand()/(float)(RAND_MAX)) * 120.0f;  // kts

      m_triangle.TR = m_leg01.getTrack();

      m_wv.Set( m_triangle.W, m_triangle.V ); // --<<<--

      // solve triangle
      m_triangleSolver.solve( m_triangle );
}

void Simulation::Update()
{
      m_C152.SetPosition( m_leg01.getStartPos() );
      // HDG needs to be T
      m_C152.SetHDG(  m_triangle.HDG ); // set from solved triangle 

      // Measure framerate (fps):
      double currentTime = glfwGetTime(); //< The current value, in seconds, or zero if an error occurred.
      static double prvTime = 0.0;
      m_timeDelta = currentTime - prvTime; //< time of a single frame
      prvTime = currentTime;
      const int fps = static_cast<const int>( 1.0 / m_timeDelta ); 
       
      glfwSwapInterval( 1 ); //VSync (0 = off, 1 = on)
      // 1 -block buffer swapping until the monitor has done at least one vertical
      // retrace since the last buffer swap. This caps your framerate at your
      // monitor's refresh rate.

      m_dsp.Write(0, 3, "FPS", fps, "" );
      m_dsp.Write(0, 4, "Time:", m_time, "[s]" );
      m_dsp.Write(0, 5, "Time:", m_time / 60.0f, "[min]" );

      // Const vel in aircraft reference frame.
      const Vector3<float> TAS( m_triangle.TAS, 0.f, 0.0f ); // TRUE AIRSPEED 95 kts = 95 NM/h
      Vector3<float> v (0.f, 0.f, 0.f);  //95 kts = 95 NM/h
      v += TAS;
      // Convert the aircraft velocity from local aircraft's to global space. 
      v = m_C152.GetOrientationMatrix() * v;
      
      // Add global wind velocity to aircraft's global velocity.
      v = v + m_wv.GetWV();

      m_dsp.GetStream() << "V/W:  " << m_wv.GetDirFromDegT() << " [°T] / " << m_wv.GetWV().Mag() << " [kts] ";
      m_dsp.WriteFromStream(0, 8);
      
      const float maxDriftAngleDeg = m_wv.GetMaxDriftAngleDeg( v.Mag() );   
      m_dsp.Write(50, 8, "MAX DRIFT: ", maxDriftAngleDeg, "[°]" );

      const Quaternion<float> orient = m_C152.GetOrientation();
      m_dsp.Write(0, 9, "HDG: ", Rad2Deg( orient.GetAngle() ), "[°T]" );

      m_dsp.Write(30, 9, "TAS: ", TAS.Mag(), "[kts]" ); 
      
      // Obtain real track from simulation.
      const Vector3<float> NorthT( 1.0f, 0.0f, 0.0f );
      Vector3<float> vNorm = v;
      vNorm.Normalise();
      float realTR = Rad2Deg( vNorm.GetAngle( NorthT ) ); // GetAngle returns angle in range 0 - 180 DEG;
      const Vector3<float> EastT( 0.0f, 0.0f, 1.0f );
      realTR = ( v.Dot( EastT ) < 0.0f ) ? (360.0f - realTR) : realTR; //< make sure the angle is relative to N measured clockwise
      m_dsp.Write(0, 10, "TR:  ", realTR, "[°T]" );
      
      m_dsp.Write(30, 10, "GS:  ", v.Mag(), "[kts]" ); 

      static float dstTraveledNM =  0.0f;
      dstTraveledNM += ( v.ScalarMult( m_timeDelta / 3600.0f ) ).Mag();
      m_dsp.Write(0, 12, "DST TR: ", dstTraveledNM, "[NM]" ); 
      
      //triangle solution
      m_dsp.GetStream() << "-- T R I A N G L E --";
      m_dsp.WriteFromStream(0, 14);
      m_dsp.GetStream() << "V/W:  " << m_triangle.W << " [°T] / " << m_triangle.V << " [kts] ";
      m_dsp.WriteFromStream(0, 15);
      m_dsp.GetStream() << "HDG/TAS:  " << m_triangle.HDG << " [°T] / " << m_triangle.TAS << " [kts] ";
      m_dsp.WriteFromStream(0, 16);
      m_dsp.GetStream() << "TR/GS:  " << m_triangle.TR << " [°T] / " << m_triangle.GS << " [kts] ";
      m_dsp.WriteFromStream(0, 17);
 
      // vel is in [kts] = [NM/h], the timestep in [s] hence it needs to be converted
      // to [h] - therefore the division by 3600 
      m_C152.SetPosition( m_C152.GetPosition() + v.ScalarMult( m_timeDelta / 3600.0f ) );

      m_C152.SetVelocity( v ); //< vel is in [kts] = [NM/h]

      m_dsp.SetRefreshRateHz( 1.0f ); //< every 1 sec
      m_dsp.Refresh( m_time );

      // TODO Add test to verify if the sim is still a real time sim.
      // Measure the time to execute the update function and compare against the
      // time of frame (already calculated).

      m_time += m_timeDelta;
      
      
}

void Simulation::Draw()
{
    // Draw World reference frame.
    xAxisLine.Draw();
    yAxisLine.Draw();
    zAxisLine.Draw();

    m_leg01.Draw();
    m_wv.Draw();

    GLLine vel ( 0.0, m_C152.GetVelocity(), colorRed );
    vel.Draw();

    m_C152.Draw();
}

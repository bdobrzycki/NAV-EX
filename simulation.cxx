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
    ,  m_leg01( Vector3<float>( 0.0f, 0.0f, 0.0f ), 270.0f, 19.0f )
    ,  m_wv( 250.0f, 30.0f/*3600.0f*/ ) // --<<<--
    ,  m_time( 0.0f )
    ,  m_timeDelta(0.0f)
    ,  m_dsp(' ')
{
}

void Simulation::Initialise()
{
      m_atmosphereModel.PrintAtmosphericConditions( m_convert.FeetToMeters( 2700.0f ) );

      m_C152.SetPosition( m_leg01.getStartPos() );
      m_C152.SetHDG( /*m_leg01.getTrack()*/ 263.8f );

      m_triangle.getData();
}

void Simulation::Update()
{
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
      const Vector3<float> TAS( 95.f, 0.f, 0.f ); // TRUE AIRSPEED 95 kts = 95 NM/h
      Vector3<float> v (0.f, 0.f, 0.f);  //95 kts = 95 NM/h
      v += TAS;
      // Convert the aircraft velocity from local aircraft's to global space. 
      v = m_C152.GetOrientationMatrix() * v;
      
      // Add global wind velocity to aircraft's global velocity.
      v = v + m_wv.GetWV();

      m_dsp.GetStream() << "V/W: " << m_wv.GetDirectionDeg() << "°/" << m_wv.GetWV().Mag() << " [kts] ";
      m_dsp.WriteFromStream(0, 8);
      
      const float maxDriftAngleDeg = m_wv.GetMaxDriftAngleDeg( v.Mag() );   
      m_dsp.Write(0, 9, "MAX DRIFT: ", maxDriftAngleDeg, "" );
      m_dsp.Write(0, 12, "TAS: ", TAS.Mag(), "[kts]" ); 
      m_dsp.Write(0, 13, "GS: ", v.Mag(), "[kts]" ); 

      static float dstTraveledNM =  0.0f;
      dstTraveledNM += ( v.ScalarMult( m_timeDelta / 3600.0f ) ).Mag();
      m_dsp.Write(0, 15, "DST TR: ", dstTraveledNM, "[NM]" ); 
  
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
      
      // HACK
      calcTriangle();
}

void Simulation::calcTriangle()
{
    //////////////////////////////////////////////////////////////////////////////////
      static const double PI = 3.141592653589793238462643383279502884197169399375105820974944;

      // law of cosines
      // c2 = a2 + b2 - 2ab cos( C )
      // b2 = a2 + c2 - 2ac cos( B )
      // a2 = b2 + c2 - 2bc cos( A )

      // lengths
      // TAS2 = V2 + GS2 -2 V GS cos (<(W TR))
      // V2   = TAS2 + GS2 - 2 TAS GS cos (<(HDG TR ))
      // GS2  = TAS2 + V2 -2 TAS V cos (<(HDG W))

      // angles
      //<(HDG TR)
      // Transforming V2 = TAS2 + GS2 - 2 TAS GS cos (<(HDG TR )) I got:
      // <(HDG TR) = acos( (-V*V + TAS*TAS + GS*GS) / ( 2.0 * TAS * GS ) );

      double W = 260.0f; // from where
      W -= 180.0;        // to
      double V = 15.0;
      double TR = 296.0;
      double GS = 120.0;
      double TAS = sqrt( V*V + GS*GS - 2.0*V*GS * cos( ((TR-W) * PI) / 180.0 ));
      //std::cout << "TAS " << TAS << "\n";

      // HDG = TR - <(HDG TR)
      double angleRad = 0.0; //<(HDG TR)
      angleRad = acos( (-V*V + TAS*TAS + GS*GS) / ( 2.0 * TAS * GS ) );
      double HDG = TR - ((angleRad * 180.0) / PI);
      //std::cout << "HDG " << HDG << "\n";


      // TODO
      // |0|0|HDG|TAS|W|V|TR|GS| (8 BIT mask)

      // basic (most common example)
      // |0|0| 0 | 1 |1|1|1 |0 | (00011110) 
      // vector (W/V), dir(TR), length(TAS) 
      TR = 270.0;
      TAS = 95.0;
      W = 250.0;
      V = 30.0;
      HDG = 0.0; //<?
      GS = 0.0; //<?
      
      W = ( (W - 180.0) < 0 ) ? (360.0 - W) : (W - 180.0);
      double W_TR = ( W - TR < 0.0) ? 360.0 + (W - TR) : (W - TR);
      W_TR *= PI/180.0f; // to RAD
      //std::cout << "W_TR " << W_TR << "\n";
      // from delta (two solutions, one is negative, choose positive)
      const double GS1 = ( 2.0 * V * cos(W_TR) + sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
      const double GS2 = ( 2.0 * V * cos(W_TR) - sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
      //std::cout << "GS1 " << GS1 << "\n";
      //std::cout << "GS2 " << GS2 << "\n";
      GS = (GS1 > 0) ? GS1 : GS2;
      m_dsp.Write(0, 17, "__GS", GS, "[kts]" ); 
      double HDG_TR_RAD = acos( (-V*V + TAS*TAS + GS*GS) / ( 2.0 * TAS * GS ) );
      double HDG_TR_DEG = ((HDG_TR_RAD * 180.0) / PI);
      HDG = TR - HDG_TR_DEG; //< might not be true for all cases
      m_dsp.Write(0, 18, "__HDG", HDG, "[°T]" );
      //////////////////////////////////////////////////////////////////////////////////
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

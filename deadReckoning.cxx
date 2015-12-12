#include <cstdlib> //rand
#include "deadReckoning.h"

using namespace GrapheneMath;

DeadReckoning::DeadReckoning()
    :  m_time( 0.0f )
    ,  m_timeDelta(0.0f)
    ,  m_dsp(' ')
{
}

DeadReckoning::~DeadReckoning()
{
   std::cout << "Kill dead reckoning module..." << "\n";
}

void DeadReckoning::Initialise()
{
      //m_wv.Set( m_triangle.W, m_triangle.V ); // --<<<--
      
      //m_C152.SetPosition( m_leg01.getStartPos() );
      
      // HDG needs to be T
      //m_C152.SetHDG(  m_triangle.HDG ); //< set from solved triangle 
}

void DeadReckoning::Update()
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

      m_dsp.Write(0, 3, "FPS", fps, " " );
      m_dsp.Write(0, 4, "Time:", m_time, "[s]" );
      m_dsp.Write(0, 5, "Time:", m_time / 60.0f, "[min]" );
      m_dsp.SetRefreshRateHz( 1.0f ); //< every 1 sec
      m_dsp.Refresh( m_time );

      m_time += m_timeDelta;
}

void DeadReckoning::Draw()
{
    // Draw World reference frame.
    //xAxisLine.Draw();
    //yAxisLine.Draw();
    //zAxisLine.Draw();

    //m_wv.Draw();
    //m_C152.Draw();
}

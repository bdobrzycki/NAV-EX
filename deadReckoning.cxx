#include <cstdlib> //rand
#include "deadReckoning.h"

using namespace GrapheneMath;

DeadReckoning::DeadReckoning()
    :  m_wv( Vector3<float>( 0.0f, 0.0f, 0.0f ) )
    ,  m_time( 0.0f )
    ,  m_timeDelta( 1.0f / 60.0f ) // sec
    ,  m_timer( 0.0f )
    ,  m_dsp(' ')
    ,  m_V( 0.0f )
{
}

DeadReckoning::~DeadReckoning()
{
   std::cout << "Kill dead reckoning module..." << "\n";
}

void DeadReckoning::getInputParams()
{
    float input;
    std::cout << "W [°] (from): ";
    std::cin >> input;
    m_W = std::max( std::min ( input, 360.0f ), 0.0f );

    std::cout << "V [kt]: ";
    std::cin >> input;
    m_V = std::max( input, 0.0f );

    m_wv.Set( m_W, m_V ); // --<<<--

    std::cout << "TAS [kt]: ";
    std::cin >> input;
    m_TAS = std::max( input, 0.0f );
}

void DeadReckoning::evaluateAnswer()
{
      m_dsp.GetStream() << "huj";
      m_dsp.WriteFromStream(0, 12);
      m_dsp.Refresh( m_time + 1.0f );
}

void DeadReckoning::Initialise()
{
    if( m_time > 0.0f )
    {
        evaluateAnswer();
    }

    getInputParams();

    m_C152.SetPosition( Vector3<GLfloat>(0.0f, 0.0f, 0.0f) );
    m_C152.SetHDG( 90.0f ); // DEG

    m_timer = 5.0f; // sec
    
    m_dsp.SetRefreshRateHz( 1.0f ); //< every 1 sec
}

void DeadReckoning::Update()
{
    if( m_timer < 0.0f )
    {
        Initialise();
    }
    else
    {
      const int fps = static_cast<const int>( 1.0 / m_timeDelta );

      glfwSwapInterval( 1 ); //VSync (0 = off, 1 = on)
      // 1 -block buffer swapping until the monitor has done at least one vertical
      // retrace since the last buffer swap. This caps your framerate at your
      // monitor's refresh rate.

      m_dsp.Write(0, 3, "FPS", fps, " " );
      m_dsp.Write(0, 4, "Time left:", static_cast<const int>(m_timer), "[s]" );

      m_dsp.GetStream() << "W/V:  " << m_wv.GetDirFromDegT() << " [°] / " << m_wv.GetWV().Mag() << " [kt] ";
      m_dsp.WriteFromStream(0, 7);

      m_dsp.Write(0, 8, "TAS:  ", m_TAS, "[kt]" );

      m_dsp.GetStream() << "                        ";
      m_dsp.WriteFromStream(0, 12);

      m_dsp.Refresh( m_time );

      m_time += m_timeDelta;
      m_timer -= m_timeDelta;
    }
}

void DeadReckoning::Draw()
{
    static const Vector3<GLfloat> zeroVec( 0.0f, 0.0f, 0.0f );
    static const Vector3<GLfloat> xAxis( 1.0f, 0.0f, 0.0f );
    static const Vector3<GLfloat> yAxis( 0.0f, 1.0f, 0.0f );
    static const Vector3<GLfloat> zAxis( 0.0f, 0.0f, 1.0f );

    static const Vector3<GLfloat> colorRed(   1.0f, 0.0f, 0.0f );
    static const Vector3<GLfloat> colorGreen( 0.0f, 1.0f, 0.0f );
    static const Vector3<GLfloat> colorBlue(  0.0f, 0.0f, 1.0f );
    static const Vector3<GLfloat> colorCyan(  0.0f, 1.0f, 1.0f );
    static const Vector3<GLfloat> colorWhite( 1.0f, 1.0f, 1.0f );

    static const GLLine xAxisLine( zeroVec, xAxis, colorRed);
    static const GLLine yAxisLine( zeroVec, yAxis, colorGreen);
    static const GLLine zAxisLine( zeroVec, zAxis, colorBlue);

    // Draw World reference frame.
    xAxisLine.Draw();
    yAxisLine.Draw();
    zAxisLine.Draw();

    m_wv.Draw();
    m_C152.Draw();
}

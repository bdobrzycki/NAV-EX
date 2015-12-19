#include <cstdlib> //rand
#include <string>
#include "deadReckoning.h"

using namespace GrapheneMath;

const Vector3<GLfloat> HelperLines::Colors::c_redColor( 1.0f, 0.0f, 0.0f );
const Vector3<GLfloat> HelperLines::Colors::c_greenColor( 0.0f, 1.0f, 0.0f );
const Vector3<GLfloat> HelperLines::Colors::c_blueColor( 0.0f, 0.0f, 1.0f );
const Vector3<GLfloat> HelperLines::Colors::c_chartreuseColor( 0.5f, 1.0f, 0.0f );
const Vector3<GLfloat> HelperLines::Colors::c_lightChartreuseColor( 0.498f, 0.7f, 0.0f );

const Vector3<GLfloat>& HelperLines::Colors::getColorFromIndex( const ColorIndex& idx )
{
    switch( idx )
    {
        case ColorIndex::eRed             : return c_redColor;
        case ColorIndex::eGreen           : return c_greenColor;
        case ColorIndex::eBlue            : return c_blueColor;
        case ColorIndex::eChartreuse      : return c_chartreuseColor;
        case ColorIndex::eLightChartreuse : return c_lightChartreuseColor;
    }
}

const Vector3<GLfloat> HelperLines::c_zeroVec( 0.0f, 0.0f, 0.0f );
const Vector3<GLfloat> HelperLines::c_xAxis  ( 1.0f, 0.0f, 0.0f );
const Vector3<GLfloat> HelperLines::c_yAxis  ( 0.0f, 1.0f, 0.0f );
const Vector3<GLfloat> HelperLines::c_zAxis  ( 0.0f, 0.0f, 1.0f );



DeadReckoning::DeadReckoning()
    :  m_wv( Vector3<float>( 0.0f, 0.0f, 0.0f ) )
    ,  m_time( 0.0f )
    ,  m_timeDelta( 1.0f / 60.0f ) // sec
    ,  m_timer( 0.0f )
    ,  m_timerSet( 0.0f )
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
    
    std::cout << "MAX TIME [sec]: ";
    std::cin >> input;
    m_timerSet = std::max( input, 0.0f );
}

void DeadReckoning::evaluateAnswer()
{
    // Display results.
    const float MAX_DRIFT_DEG = m_wv.GetMaxDriftAngleDeg( m_TAS );
    m_dsp.GetStream() << "MAX DRIFT (atan): " << static_cast<int>(MAX_DRIFT_DEG) << " [°]";
    m_dsp.WriteFromStream(0, 12);

    m_dsp.GetStream() << "          (1/60): " << static_cast<int>( ( m_V / m_TAS ) * 60.0f) << " [°]";
    m_dsp.WriteFromStream(0, 13);

    // DA (drift angle)
    Vector3<float> windTo = m_wv.GetWV();
    windTo.Normalise();
    const Vector3<float> lateralAxis( -1.0f, 0.0f, 0.0f );
    float dot = windTo.Dot( lateralAxis );
    std::string s = ( dot < 0.0f ) ? "PORT" : "STARBOARD";
    s = ( fabs( dot ) < 0.001f ) ? "NO DRIFT" : s; // special case
    m_dsp.GetStream() << "DA              : " << static_cast<int>( MAX_DRIFT_DEG * fabs( dot ) ) << " [°]  " << s;
    m_dsp.WriteFromStream(0, 14);

    // head / tail wind
    const Vector3<float> longitudinalAxis( 0.0f, 0.0f, 1.0f );
    dot = windTo.Dot( longitudinalAxis );
    s = ( dot < 0.0f ) ? "HEAD" : "TAIL";
    s = ( fabs( dot ) < 0.001f ) ? "GS is TAS" : s; // special case
    m_dsp.GetStream() << "WIND            : " << static_cast<int>( m_V * fabs( dot ) ) << " [kt] " << s;
    m_dsp.WriteFromStream(0, 15);

    m_dsp.Refresh( m_time + 1.0f );
}

void DeadReckoning::Initialise()
{
    if( m_time > 0.0f )
    {
        evaluateAnswer();
    }

    // Make all helper lines invisible.
    m_helperLines.m_lineVisibility.m_mask = 0;

    getInputParams();
    makeReferenceFrame();
    makeHelperLines();

    m_C152.SetPosition( Vector3<GLfloat>(0.0f, 0.0f, 0.0f) );
    m_C152.SetHDG( 90.0f ); // DEG

    m_timer = m_timerSet; // sec

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

      m_dsp.Write(0, 8, "TAS: ", m_TAS, "[kt]" );

      m_dsp.Refresh( m_time );

      m_time += m_timeDelta;
      m_timer -= m_timeDelta;
    }
}

void DeadReckoning::Draw()
{
    m_wv.Draw();
    m_C152.Draw();
    m_helperLines.draw();
}

void DeadReckoning::makeReferenceFrame()
{
    m_helperLines.set( LineIdx::eRefFrameX, HelperLines::c_zeroVec, HelperLines::c_xAxis, ColorIdx::eRed, DrawStyle::eSolid );
    m_helperLines.m_lineVisibility.m_mask |= Mask::eRefFrameX;

    m_helperLines.set( LineIdx::eRefFrameY, HelperLines::c_zeroVec, HelperLines::c_yAxis, ColorIdx::eGreen, DrawStyle::eSolid );
    m_helperLines.m_lineVisibility.m_mask |= Mask::eRefFrameY;
    
    m_helperLines.set( LineIdx::eRefFrameZ, HelperLines::c_zeroVec, HelperLines::c_zAxis, ColorIdx::eBlue, DrawStyle::eSolid );
    m_helperLines.m_lineVisibility.m_mask |= Mask::eRefFrameZ;
}

void DeadReckoning::makeHelperLines()
{
    makeLateralLines();
}

void DeadReckoning::makeLateralLines()
{
    static const float scale = 15.0f;

    Vector3<float> windTo = m_wv.GetWV();
    windTo.Normalise();

    const Vector3<float> lateralAxis( -1.0f, 0.0f, 0.0f );
    const Vector3<float> longitudinalAxis( 0.0f, 0.0f, 1.0f );

    float windToDotLateral      = windTo.Dot( lateralAxis );
    float windToDotLongitudinal = windTo.Dot( longitudinalAxis );

    // lateral axis (reference)
    m_helperLines.set( LineIdx::eLateralAxis,
                       HelperLines::c_zeroVec,
                       lateralAxis.ScalarMult( windToDotLateral * scale),
                       ColorIdx::eChartreuse,
                       DrawStyle::eSolid );
                       
    m_helperLines.m_lineVisibility.m_mask |= Mask::eLateralAxis;

    const Vector3<float> dirNorth( 1.0f, 0.0f, 0.0f ); //< North is x axis.
    const Vector3<float> rotDir( 0.0f, 1.0f, 0.0f );   //< Counterclockwise rotation.
    Quaternion<float> rot;

    float angleLateralAxis45 = GrapheneMath::Deg2Rad( 45.0f );
    float angleLateralAxis60 = GrapheneMath::Deg2Rad( 60.0f );

    // Quadrant starts from I(+,+), quadrant numbering goes counterclockwise.
    if( windToDotLateral > 0.0f ) // III or IV quadrant
    {
        if( windToDotLongitudinal > 0.0f ) // IV quadrant
        {
            angleLateralAxis45 += GrapheneMath::PI;
            angleLateralAxis60 += GrapheneMath::PI;
        }
        else                               // III quadrant
        {
            angleLateralAxis45 = GrapheneMath::PI - angleLateralAxis45;
            angleLateralAxis60 = GrapheneMath::PI - angleLateralAxis60;
        }
    }
    else  // I or II quadrant
    {
        if( windToDotLongitudinal > 0.0f ) // I quadrant
        {
            angleLateralAxis45 = -angleLateralAxis45;
            angleLateralAxis60 = -angleLateralAxis60;
        }
        else                               // II quadrant
        {
            // angles are set already
        }
    }

    // 45 DEG helper line
    rot.FromAxisAngle( rotDir, angleLateralAxis45 );
    Vector3<float> helperDir = rot.RotateFast( dirNorth );

    m_helperLines.set( LineIdx::eLateralAxis45,
                       HelperLines::c_zeroVec,
                       helperDir.ScalarMult( scale ),
                       ColorIdx::eLightChartreuse,
                       DrawStyle::eSolid );

    m_helperLines.m_lineVisibility.m_mask |= Mask::eLateralAxis45; 

    // 60 DEG helper line
    rot.FromAxisAngle( rotDir, angleLateralAxis60 );
    helperDir = rot.RotateFast( dirNorth );

    m_helperLines.set( LineIdx::eLateralAxis60,
                       HelperLines::c_zeroVec,
                       helperDir.ScalarMult( scale ),
                       ColorIdx::eLightChartreuse,
                       DrawStyle::eStipple );

    m_helperLines.m_lineVisibility.m_mask |= Mask::eLateralAxis60;
}

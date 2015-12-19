#ifndef __DEAD_RECKONING_H__
#define __DEAD_RECKONING_H__

#include <assert.h>
#include <cstdint>
#include <GLFW/glfw3.h>

// App
#include "applicationModule.h"

// Math
#include "mathUtils.h"
#include "quat.h"
#include "matrix4.h"
#include "glLine.h"
#include "xlocator.h"
#include "varrow2d.h"
#include "wv.h"
#include "aeroplane.h"
#include "alphanumdisplay.h"

using namespace GrapheneMath;

class HelperLines
{
public:

    struct LineIdx
    {
        enum Idx : int8_t
        {
            eRefFrameX = 0,
            eRefFrameY,
            eRefFrameZ,
            eLateralAxis,
            eLongitudinalAxis,
            eLateralAxis45,
            eLateralAxis60,
            eLongitudinalAxis45,
            eLongitudinalAxis60,
            eNumLines
        };
    };

    typedef LineIdx::Idx LineIndex;

    struct LineVisibility
    {
        enum Mask : int16_t
        {
            eRefFrameX          = 1 << LineIdx::eRefFrameX,
            eRefFrameY          = 1 << LineIdx::eRefFrameY,
            eRefFrameZ          = 1 << LineIdx::eRefFrameZ,
            eLateralAxis        = 1 << LineIdx::eLateralAxis,
            eLongitudinalAxis   = 1 << LineIdx::eLongitudinalAxis,
            eLateralAxis45      = 1 << LineIdx::eLateralAxis45,
            eLateralAxis60      = 1 << LineIdx::eLateralAxis60,
            eLongitudinalAxis45 = 1 << LineIdx::eLongitudinalAxis45,
            eLongitudinalAxis60 = 1 << LineIdx::eLongitudinalAxis60,
        };

        int16_t m_mask = ~0; //< all lines visible

    } m_lineVisibility;

    struct LineDrawStyle
    {
        enum Style : int16_t
        {
            eSolid   = 1 << 0,
            eStipple = 1 << 1,
        };
    };

    struct Colors
    {
        struct ColorIdx
        {
            enum Idx : int8_t
            {
              eRed = 0,
              eGreen,
              eBlue,
              eChartreuse,
              eLightChartreuse,
            };
        };

        typedef ColorIdx::Idx ColorIndex;

        static const Vector3<GLfloat> c_redColor;
        static const Vector3<GLfloat> c_greenColor;
        static const Vector3<GLfloat> c_blueColor;
        static const Vector3<GLfloat> c_chartreuseColor;
        static const Vector3<GLfloat> c_lightChartreuseColor;

        static const Vector3<GLfloat>& getColorFromIndex( const ColorIndex& );
    };

    static const Vector3<GLfloat> c_zeroVec;
    static const Vector3<GLfloat> c_xAxis;
    static const Vector3<GLfloat> c_yAxis;
    static const Vector3<GLfloat> c_zAxis;

private:
    struct GLStyleLine
    {
        GLLine                m_line;
        LineDrawStyle::Style  m_style;
    };

    std::vector<GLStyleLine>  m_lines;

public:
    HelperLines()
    {
        m_lines.resize( LineIndex::eNumLines );
    }

    void set( LineIndex               lineIdx,
              const Vector3<GLfloat>& from,
              const Vector3<GLfloat>& to,
              Colors::ColorIndex      colorIdx,
              LineDrawStyle::Style    style )
    {
        m_lines[ lineIdx ].m_line.Set( from, to, Colors::getColorFromIndex( colorIdx ) );
        m_lines[ lineIdx ].m_style = style;
    }

    void draw()
    {
        int16_t bit;
        for( uint8_t i = 0; i < m_lines.size(); i++ )
        {
            bit = 1 << i;
            if( m_lineVisibility.m_mask & bit )
            {
                switch( m_lines[i].m_style )
                {
                    case LineDrawStyle::Style::eSolid :
                      m_lines[i].m_line.Draw();
                      break;

                    case LineDrawStyle::Style::eStipple :
                      m_lines[i].m_line.DrawStipple();
                      break;

                    default:
                      m_lines[i].m_line.Draw();
                      break;
                }
            }
        }
    }
};

class DeadReckoning : public ApplicationModule
{
    typedef HelperLines::Colors::ColorIndex ColorIdx;
    typedef HelperLines::LineVisibility::Mask Mask;
    typedef HelperLines::LineIdx LineIdx;
    typedef HelperLines::LineDrawStyle::Style DrawStyle;

private:
    Aeroplane       m_C152;
    WV              m_wv;
    float           m_time;
    float           m_timeDelta;
    float           m_timer;
    float           m_timerSet;
    AlphanumDisplay m_dsp;
    float           m_W; //< wind direction from
    float           m_V; //< wind speed
    float           m_TAS;

    HelperLines     m_helperLines;

    void getInputParams();
    void evaluateAnswer();
    void makeReferenceFrame();
    void makeHelperLines();
    void makeLateralLines();

public:
    DeadReckoning();
    virtual ~DeadReckoning();

    // Application Module overrides.
    void Initialise();
    void Update();
    void Draw();
};



#endif //__DEAD_RECKONING_H__

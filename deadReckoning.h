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

    struct ColorIdx
    {
        enum Idx : int8_t
        {
            eRed = 0,
            eGreen,
            eBlue,
            eChartreuse,
            eNumColors
        };
    };

    typedef ColorIdx::Idx ColorIndex;

    static const Vector3<GLfloat> c_zeroVec;
    static const Vector3<GLfloat> c_xAxis;
    static const Vector3<GLfloat> c_yAxis;
    static const Vector3<GLfloat> c_zAxis;

private:
    std::vector<GLLine>             m_lines;
    std::vector< Vector3<GLfloat> > m_colors;

public:
    HelperLines()
    {
        m_lines.resize( LineIndex::eNumLines );
        m_colors.resize( ColorIndex::eNumColors );
    }

    void set( LineIndex               lineIdx,
              const Vector3<GLfloat>& from,
              const Vector3<GLfloat>& to,
              ColorIndex              colorIdx )
    {
        m_lines[lineIdx].Set( from, to, m_colors[colorIdx] );
    }

    void draw()
    {
        for( uint8_t i = 0; i < m_lines.size(); ++i )
        {
            m_lines[i].Draw();
        }
    }
};

class DeadReckoning : public ApplicationModule
{
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
    void makeHelperLines();
    void drawHelperLines();

public:
    DeadReckoning();
    virtual ~DeadReckoning();

    // Application Module overrides.
    void Initialise();
    void Update();
    void Draw();
};



#endif //__DEAD_RECKONING_H__

#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <assert.h>
#include <GLFW/glfw3.h>

// Math
#include "mathUtils.h"
#include "quat.h"
#include "matrix4.h"
#include "glLine.h"
#include "xlocator.h"
#include "varrow2d.h"
#include "wv.h"
#include "navleg.h"
#include "triangle.h"
#include "aeroplane.h"

// Flight sim
#include "atmosphere.h"
#include "alphanumdisplay.h"

using namespace GrapheneMath;

class Simulation
{
private:
    const Vector3<GLfloat> zeroVec;
    const Vector3<GLfloat> xAxis;
    const Vector3<GLfloat> yAxis;
    const Vector3<GLfloat> zAxis;
    
    const Vector3<GLfloat> colorRed;
    const Vector3<GLfloat> colorGreen;
    const Vector3<GLfloat> colorBlue;
    const Vector3<GLfloat> colorCyan;
    const Vector3<GLfloat> colorWhite;
    
    const GLLine xAxisLine;
    const GLLine yAxisLine;
    const GLLine zAxisLine;
    
    Atmosphere    m_atmosphereModel;
    UnitConverter m_convert;
    Aeroplane     m_C152;
    NavLeg        m_leg01;
    WV            m_wv;
    float         m_time;
    float         m_timeDelta;

    TriangleOfVelocitiesSolver   m_triangleSolver;
    TriangleOfVelocities         m_triangle;

    AlphanumDisplay m_dsp;
    
public:
    Simulation();
    void Initialise();
    void Update();
    void Draw();

    void calcTriangle();
};

#endif //__SIMULATION_H__

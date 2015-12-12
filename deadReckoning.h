#ifndef __DEAD_RECKONING_H__
#define __DEAD_RECKONING_H__

#include <assert.h>
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

class DeadReckoning : public ApplicationModule
{
private:
    Aeroplane       m_C152;
    WV              m_wv;
    float           m_time;
    float           m_timeDelta;
    float           m_timer;
    AlphanumDisplay m_dsp;
    float           m_W; //< wind direction from
    float           m_V; //< wind speed
    float           m_TAS;

    void getInputParams();
    void evaluateAnswer();

public:
    DeadReckoning();
    virtual ~DeadReckoning();

    // Application Module overrides.
    void Initialise();
    void Update();
    void Draw();
};



#endif //__DEAD_RECKONING_H__

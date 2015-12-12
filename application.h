#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <assert.h>
#include <vector>

#include "simulation.h"
#include "deadReckoning.h"

class Application
{
private:
    std::vector<ApplicationModule*>  m_appModules;

public:
    Application();
    ~Application();

    void Initialise();
    void Update();
    void Draw();
};

#endif //__APPLICATION_H__

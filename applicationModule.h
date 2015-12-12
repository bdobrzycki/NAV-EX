#ifndef __APPLICATION_MODULE_H__
#define __APPLICATION_MODULE_H__

class ApplicationModule
{
public:
    virtual void Initialise() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual ~ApplicationModule(){};
};

#endif //__APPLICATION_MODULE_H__

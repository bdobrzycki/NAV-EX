#include "application.h" 

Application::Application()
{
    m_appModules.resize(0);
    m_appModules.push_back( new Simulation() );
    m_appModules.push_back( new DeadReckoning() );
}

Application::~Application()
{
    for( size_t i = 0; i < m_appModules.size(); ++i )
    {
        delete m_appModules[i];
    }
}

void Application::Initialise()
{
    //m_appModules[0]->Initialise();
    m_appModules[1]->Initialise();
}

void Application::Update()
{
    //m_appModules[0]->Update();
    m_appModules[1]->Update();
}

void Application::Draw()
{
    //m_appModules[0]->Draw();
    m_appModules[1]->Draw();
}



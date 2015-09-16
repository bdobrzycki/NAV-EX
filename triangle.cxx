#include "triangle.h" 

 TriangleOfVelocities::TriangleOfVelocities()
    : m_mask( 0 )
    {}

void TriangleOfVelocities::getData()
{
    std::cout << "HDG " << "\n";
    std::cin >> m_data.HDG;
    m_mask |= eHDG;
}
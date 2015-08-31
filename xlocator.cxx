#include "xlocator.h"

using namespace GrapheneMath;

void XLocator::Initialise()
{   
    X.resize( numOfLines );
      
    offsets.resize( numOfLines );
      
    for (unsigned int i = 0; i < numOfLines; ++i)
    {
        offsets.push_back( Vector3<float>(0.0f, 0.0f, 0.0f) );
    }
}

void XLocator::Set(const Vector3<float>& pos, float radius, const Vector3<float>& color)
{
  SetPosition( pos );
  SetRadius( radius );
  SetColor( color );
}
  
void XLocator::UpdateOffsetsFromRadius( float radius )
{
  offsets[0].SetX( radius );
  offsets[1].SetY( radius );
  offsets[2].SetZ( radius );
}
  
XLocator::XLocator()
    : radius(0.1f)
{
    Initialise();
    Set(Vector3<float>(0.0f, 0.0f, 0.0f), radius, Vector3<float>(0.0f, 1.0f, 1.0f));
}
 
XLocator::XLocator( const Vector3<float>& pos, float _radius, const Vector3<float>& color )
    : radius(_radius)
{
    Initialise();
    Set( pos, radius, color );
}
  
void XLocator::SetPosition( const Vector3<float>& pos )
{
    UpdateOffsetsFromRadius( radius );
                           
    for (unsigned int i = 0; i < numOfLines; ++i)
    {
        X[i].SetFrom(pos - offsets[i]);
        X[i].SetTo(pos + offsets[i]);
    }
}
  
void XLocator::SetRadius(float radius)
{
    UpdateOffsetsFromRadius( radius );
                              
    for (unsigned int i = 0; i < numOfLines; ++i)
    {
        X[i].SetFrom(X[i].GetFrom() - offsets[i]);
        X[i].SetTo(X[i].GetTo() + offsets[i]);     
    }
}
  
void XLocator::SetColor( const Vector3<float> & color )
{
      for (unsigned int i = 0; i < numOfLines; ++i)
      {
        X[i].SetColor(color);
      }
}
  
void XLocator::Draw() const
{ 
    for( unsigned int i = 0; i < numOfLines; ++i ) {
        X[ i ].Draw();
    }
}

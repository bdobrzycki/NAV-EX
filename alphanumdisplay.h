#ifndef __ALPHANUM_DISPLAY_H__
#define __ALPHANUM_DISPLAY_H__

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

class AlphanumDisplay
{
private:

  struct Display //< keep the enumeration scoped
  {
    enum
    {
      eWidth  = 76,
      eHeight = 26,
    }; 
  };

  const std::string   c_helloMessage;
  std::string         m_displayGrid;   
  char                m_displayBckg;     //< display background char
  float               m_refreshRate;     //< Hz
  float               m_lastRefreshTime; //< s
  std::ostringstream  m_ostr;

public:
  AlphanumDisplay( char displayBckg = ' ' );
  
  // Convert numbers to strings.
  template<typename T>
  static std::string NumberToString( const T input )
  {
      std::ostringstream ostr;
      ostr << input;
      return ostr.str();
  }
  
  inline void SetRefreshRateHz( float refreshRate ) { m_refreshRate = refreshRate; }
  void Clear();

  // Write to display.
  template<typename T>
  void Write( int column, int row, const std::string& name, const T& value, const std::string& unit )
  {
      std::ostringstream ostr;
      ostr << name << " " << value << " " << unit << " ";
      m_displayGrid.replace( (Display::eWidth * row) + column, ostr.str().length(), ostr.str() );
  }

  std::ostringstream& GetStream();
  void WriteFromStream( int column, int row );

  // display to console
  void Refresh( float time );
};

#endif //__ALPHANUM_DISPLAY_H__

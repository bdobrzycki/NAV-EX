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

  const std::string c_helloMessage;
  std::string       m_displayGrid;   
  char              m_displayBckg;     //< display background char
  float             m_refreshRate;     //< Hz
  float             m_lastRefreshTime; //< s

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
  void Write( int column, int row, const std::string& text );
  
  // Print display to console.
  void Print( float time );
};

#endif //__ALPHANUM_DISPLAY_H__


#include "alphanumdisplay.h"

AlphanumDisplay::AlphanumDisplay( char displayBckg )
  :  c_helloMessage("Alphanumeric Display v1.0")
  ,  m_refreshRate( 10.0f )
  ,  m_lastRefreshTime( 0.0f )
  {
    m_displayBckg = displayBckg;
    Clear();
  }
  
  void AlphanumDisplay::Clear()
  {
    const int charCount = Display::eWidth * Display::eHeight; 
    
    // Fill in the backgrond.
    m_displayGrid.assign( charCount, m_displayBckg );
    
    // Add end of lines at the end of every line.
    for( int i = 0; i < Display::eHeight; ++i )
    {
        m_displayGrid[ (i + 1) * Display::eWidth - 1 ] = '\n';
    }
    
    // Hello message.
    Write( Display::eWidth - c_helloMessage.length() - 1, 1, c_helloMessage );
    
    std::ostringstream ostr;
    ostr << "Resolution: " << Display::eWidth << " x " << Display::eHeight 
         << ", refresh rate: " << m_refreshRate << " Hz";
    Write( 0, 1, ostr.str() );
  }
  
  // Write to display.
  void AlphanumDisplay::Write( int column, int row, const std::string& text )
  {
      m_displayGrid.replace( (Display::eWidth * row) + column, text.length(), text );
  }
  
  // Print display to console.
  void AlphanumDisplay::Print( float time )
  {
      // 60 Hz -> 60 updates -> 1 [s]
      //          1  updates -> x [s]
      // x[s] = 1 updates * 1 [s] / 60 updates = 1/60 [s] i.e. update every 0.01666[s]
      if ( (time - m_lastRefreshTime) > ( 1.0f / m_refreshRate ) )
      {
        std::cout << m_displayGrid << std::flush;
        m_lastRefreshTime = time; 
      }
  }


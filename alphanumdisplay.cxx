
#include "alphanumdisplay.h"

AlphanumDisplay::AlphanumDisplay( char displayBckg )
  :  c_helloMessage("Alphanumeric Display v1.0")
  ,  m_refreshRate( 10.0f )
  ,  m_lastRefreshTime( 0.0f )
  {
    m_ostr.str("");
    m_ostr.clear();

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
    GetStream() << c_helloMessage;
    WriteFromStream(Display::eWidth - c_helloMessage.length() - 1, 1);

    GetStream() << "Resolution: " << Display::eWidth << " x " << Display::eHeight 
                << ", refresh rate: " << m_refreshRate << " Hz";
    WriteFromStream(0, 1);
  }
  
  //-----------------------------------------------------------------------------
  // Returns reference to the cleared member ostringstream.
  std::ostringstream& AlphanumDisplay::GetStream()
  {
      m_ostr.str("");
      m_ostr.clear();
      return m_ostr;
  }
 
  // Example of usage in Client code:
  // 1. Get the osstr reference:
  // this.GetStream() << "V/W: " << m_wv.GetDirectionDeg() << "°/" << m_wv.GetWV().Mag() << " [kts] ";
  // 2. write:
  // this.WriteFromStream(0, 8);
  void AlphanumDisplay::WriteFromStream( int column, int row )
  {
      m_displayGrid.replace( (Display::eWidth * row) + column, m_ostr.str().length(), m_ostr.str() );
  }

  // display to console
  void AlphanumDisplay::Refresh( float time )
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


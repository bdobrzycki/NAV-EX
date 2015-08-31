#ifndef __ATMOSPHERE_H__
#define __ATMOSPHERE_H__

#include <math.h>
#include <assert.h>
#include <iostream>

class UnitConverter
{
public:
    double CelciusToKelvin (double tempC) const
    {
        return ( tempC + 273.15 );
    }
    
    double KelvinToCelcius (double tempK ) const
    {
        return ( tempK - 273.15 );
    }
    
    double MetersToFeet( double meters ) const
    {
        if( meters <= 0.0 )
        {
            return 0.0;
        }
        else
        {
            return ( 3.280839895 * meters );
        } 
    }
    
    double FeetToMeters( double feet ) const
    {
        if( feet <= 0.0 )
        {
            return 0.0;
        }
        else
        {
            return feet / 3.280839895;
        }
    }
    
    double NauticalMilesToMeters( double nm ) const
    {
        return nm * 1.852;
    }
    
    double MetersToNauticalMiles( double m ) const
    {
        return m / 1.852;
    }
    
    double MetersToKilometers( double m ) const
    {
        return m / 1000.0;
    }
    
    double KilometersToMeters( double km ) const
    {
        return km * 1000.0;
    }

};

/*
  Computation of atmospheric temperature, pressure, density
  based on NASA implementation of the US Standard Atmosphere 1976
  http://ccmc.gsfc.nasa.gov/modelweb/atmos/us_standard.html
  only valid for troposphere (sea-level to 36,000 ft)
  p. 99, Principles of Flight Simulation, David Allerton
*/

// Atmospheric model.
class Atmosphere
{
private:
  static constexpr double Rs = 8314.4621; //< (N.m / kmol.K) ideal gass constant  
                                          //  1 kmol (kilomole) = 1000 mol (moles)
  
  // Newton metres and joules are "dimensionally equivalent" in the sense that
  // they have the same expression in SI base units:
  // 1 N.m = 1 kg.m2 / s2
  // 1 J   = 1 kg.m2 / s2
  
  static constexpr double M0 = 28.9644; //< (kg / kmol) mean molecular mass of air
   
  // Oxygen O has a 16 atomic unit mass. 
  // Oxygen is diatomic in air: O2 therefore it has 16x2 = 32 atomic unit mass.
  // 1 mole of O => 16 g
  // 1 mole of O2 => 32 g   
  
  // The molecular mass of a substance is the mass of one molecule of that substance
  // and is expressed in the terms of the molecular mass unit u
  // (equal to 1/12 the mass of one atom of carbon-12). The molecular mass can be
  // calculated as the sum of the atomic masses of all the atoms of any molecule.
  
  static constexpr double g0 = 9.80665;   //< m / s2 - gravity acceleration at 45.5425 deg lat.
  static constexpr double r0 = 6356766.0; //< m - earth radius at g0
  static constexpr double p0 = 101325.0;  //< Pa - air pressure at g0
  static constexpr double T0 = 288.15;    //< K - standard sea-level temperature
  static constexpr double Td = 273.15;    //< K - zero degrees C
  static constexpr double Tl = 0.0065;
  static constexpr double Gamma = 1.4;    //< Ratio of Specific heats for an ideal diatomic gas.
  
  UnitConverter m_unitConverter;
  
  public:
 
    // The geometric height of an aircraft (altitude) also has geopotential
    // height to take to account of the reduction of gravitational force on the
    // air particles where it is flying. The geopotential altitude should be used
    // to calculate temperature, pressure.
    // and density. 
    // altitude in meters (m)
    // returns geopotential altitude in meters (m)
    double GetGeopotentialAltitude( double altitude ) const
    {
      assert( altitude >= 0.0 );
      return ( r0 * altitude ) / ( r0 + altitude ); 
    }
    
    // T0 is standard sea-level temperature [K]
    // 0.0065 [°C / m] is the temperature laps rate (constant through the troposphere)
    double getTemperature( double altitude ) const
    {
      assert( altitude >= 0.0 );
      return ( T0 - 0.0065 * GetGeopotentialAltitude( altitude ) ); 
    } 
    
    void PrintAtmosphericConditions( double altitude ) const
    {
        std::cout << "------NASA implementation of the US Standard Atmosphere 1976------\n";
        std::cout << "Altitude: " << altitude << " [m] " << m_unitConverter.MetersToFeet( altitude ) << " [ft]" << "\n";  
        
        // The geopotential altitude should be used to calculate temperature, pressure and density.
        const double H = GetGeopotentialAltitude( altitude );
        std::cout << "Geopotential Altitude [m]: " << H << "\n"; 
        
        //
        const double T = getTemperature( altitude );
        std::cout << "Temperature: " << T << " [K] " << m_unitConverter.KelvinToCelcius( T ) << " [°C]" << "\n"; 
    }

};





#endif //__ATMOSPHERE_H__

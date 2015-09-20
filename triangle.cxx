#include "triangle.h" 

const double TriangleOfVelocities::THREE_SIXTY_DEG = 360.0;
const double TriangleOfVelocities::ONE_EIGHTY_DEG  = 180.0;

TriangleOfVelocities::TriangleOfVelocities()
    : m_mask( 0 )
    {}

void TriangleOfVelocities::getData()
{
    std::cout << "HDG [DEG T]" << "\n";
    std::cin >> m_data.HDG;
    m_mask |= eHDG;

    // etc.
}

void TriangleOfVelocities::solve()
{
    solveVecDirLength();
}

// law of cosines
// c2 = a2 + b2 - 2ab cos( C )
// b2 = a2 + c2 - 2ac cos( B )
// a2 = b2 + c2 - 2bc cos( A )

// lengths squared
// TAS2 = V2 + GS2 -2 V GS cos (<(W TR))
// V2   = TAS2 + GS2 - 2 TAS GS cos (<(HDG TR ))
// GS2  = TAS2 + V2 -2 TAS V cos (<(HDG W))

// Test 01
// TR  = 150.0;
// TAS = 100.0;
// W   = 0.0;
// V   = 30.0;
//
// >>>> GS  = 125;
// >>>> HDG = 141;

// Test 02
// TR  = 290.0;
// TAS = 174.0;
// W   = 240.0;
// V   = 40.0;
//
// >>>> GS  = 145;
// >>>> HDG = 280;

    
void TriangleOfVelocities::solveVecDirLength()
{
    std::cout << "Triangle solve()" << "\n";
    std::cout << "----------------" << "\n";

    // basic (most common example)
    // |0|0| 0 | 1 |1|1|1 |0 | (00011110)
    // vector (W/V), dir(TR), length(TAS)
    double TR  = 150;
    double TAS = 100.0;
    double W   = 0.0;
    double V   = 30.0;
    double HDG = 0.0; //<?
    double GS  = 0.0; //<?

    // Reverse Wind direction, handling edge cases.
    if( abs( W - ONE_EIGHTY_DEG ) < NUM_TOLERANCE ) //< wind is FROM 180 T (S)
    {
        W = 0.0; // TO N
    }
    else if( abs( W ) < NUM_TOLERANCE ) //< wind is FROM 0 T (N)
    {
        W = ONE_EIGHTY_DEG; // TO S
    }
    else
    {
         W = ( W < ONE_EIGHTY_DEG ) ? ( W + ONE_EIGHTY_DEG ) : ( W - ONE_EIGHTY_DEG ); // TO
    }
    
    std::cout << "W(to) " << W <<" [°T]" << "\n";

    double W_TR = ( W - TR < 0.0 ) ? 360.0 + (W - TR) : (W - TR);
    W_TR = Deg2Rad( W_TR ); // to RAD

    // from delta (two solutions, one is negative, choose positive)
    const double GS1 = ( 2.0 * V * cos(W_TR) + sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
    const double GS2 = ( 2.0 * V * cos(W_TR) - sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
    //std::cout << "GS1 " << GS1 << "\n";
    //std::cout << "GS2 " << GS2 << "\n";
    
    // Select the solution with positive GS.
    GS = ( GS1 > 0 ) ? GS1 : GS2;
    std::cout << "GS " << GS << " [kts]" << "\n";
    
    double HDG_TR_RAD = acos( (-V*V + TAS*TAS + GS*GS) / ( 2.0 * TAS * GS ) ); //< this is the drift angle DA
    double HDG_TR_DEG = Rad2Deg( HDG_TR_RAD );

    // To work out whether to add or subtract the drift angle to the track to compensate for wind,
    // we can use the angle between the track and the wind: <)W,TR
    // Simplest method is to find the min angle from W and TR.
    // E.g.: W < TR, then if we subtract min angle from W and TR we offset W to 0 DEG, and TR by W.
    // Now it is easy to work out if the drift is left or right.  
 
    bool leftDrift = false;
    
    if( W < TR )
    {
        if( TR-W < ONE_EIGHTY_DEG )
        {
            std::cout << "DA " << HDG_TR_DEG << " [°] L (port)" << "\n";
            leftDrift = true;
        }
        else
        {
            std::cout << "DA " << HDG_TR_DEG << " [°] R (port)" << "\n";
            leftDrift = false;
        }
    }
    else // W > TR
    {
        if( W-TR < ONE_EIGHTY_DEG )
        {
            std::cout << "DA " << HDG_TR_DEG << " [°] R (port)" << "\n";
            leftDrift = false;
        }
        else
        {
            std::cout << "DA " << HDG_TR_DEG << " [°] L (port)" << "\n";
            leftDrift = true;
        }
    }

    HDG = ( leftDrift ) ? ( TR + HDG_TR_DEG ) : ( TR - HDG_TR_DEG );

    // Wrap around to 360.
    HDG = ( HDG > THREE_SIXTY_DEG ) ? ( HDG - THREE_SIXTY_DEG) : HDG;
    HDG = ( HDG < 0.0 ) ? ( HDG + THREE_SIXTY_DEG) : HDG;

    std::cout << "HDG " << HDG <<" [°T]" << "\n";
    std::cin >> W;
}
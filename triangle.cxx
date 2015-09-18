#include "triangle.h" 

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

void TriangleOfVelocities::solveVecDirLength()
{
    std::cout << "Triangle solve()" << "\n";
    std::cout << "----------------" << "\n";

    // basic (most common example)
    // |0|0| 0 | 1 |1|1|1 |0 | (00011110)
    // vector (W/V), dir(TR), length(TAS)
    double TR = 290.0;
    double TAS = 174.0;
    double W = 240.0;
    double V = 40.0;
    double HDG = 0.0; //<?
    double GS = 0.0;  //<?

    // Reverse Wind direction, handling edge cases.
    if( abs( W - 180.0 ) < NUM_TOLERANCE ) //< wind is FROM 180 T (S)
    {
        W = 0.0; // TO N
    }
    else if( abs( W ) < NUM_TOLERANCE ) //< wind is FROM 0 T (N)
    {
        W = 180.0; // TO S
    }
    else
    {
         W = ( W < 180.0 ) ? ( W + 180.0 ) : ( W - 180.0 ); // TO
    }

    double W_TR = ( W - TR < 0.0 ) ? 360.0 + (W - TR) : (W - TR);
    W_TR = Deg2Rad( W_TR ); // to RAD
    
    
    //std::cout << "W_TR " << W_TR << "\n";
    // from delta (two solutions, one is negative, choose positive)
    const double GS1 = ( 2.0 * V * cos(W_TR) + sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
    const double GS2 = ( 2.0 * V * cos(W_TR) - sqrt( 4.0 * V * V* cos( W_TR ) * cos( W_TR ) - 4.0*V*V + 4.0*TAS*TAS )) / 2.0;
    //std::cout << "GS1 " << GS1 << "\n";
    //std::cout << "GS2 " << GS2 << "\n";
    GS = (GS1 > 0) ? GS1 : GS2;
    std::cout << "GS " << GS << " [kts]" << "\n";
    double HDG_TR_RAD = acos( (-V*V + TAS*TAS + GS*GS) / ( 2.0 * TAS * GS ) ); //< this is the drift angle DA
    double HDG_TR_DEG = Rad2Deg( HDG_TR_RAD );
    // Polar to Cartesian
    const double TRx = GS*cos( TR );
    const double TRy = GS*sin( TR );
    const double WVx = V*cos( W );
    const double WVy = V*sin( W );
    // 2D cross product
    const double X = TRx*WVy - TRy*WVx; 
    //std::cout << "X " << X << "\n";
    HDG = ( X < 0.0f ) ? (TR + HDG_TR_DEG) : (TR - HDG_TR_DEG);
    std::cout << "HDG " << HDG <<" [°T]" << "\n";
    std::cin >> W;
}
//
//  ASolverTest.h
//  pb_solvers_code
//
//  Created by Lisa Felberg on 10/8/15.
//  Copyright © 2015 Lisa Felberg. All rights reserved.
//

#ifndef ASolverTest_h
#define ASolverTest_h

#include "ASolver.h"

class ASolverTest
{
public :
  void RunASolverTest()
  {
    Constants const_;
    vector< Molecule > mol_;
    
    mol_.clear( );
    Pt pos[2] = { Pt( 0.0, 0.0, 0.0 ), Pt( 0.0, 0.0, 5.0 ) };
    for (int molInd = 0; molInd < 2; molInd ++ )
    {
      int M = 1;
      double a = 1.0;
      vector<double> charges(1);
      vector<Pt> posCharges(1);
      
      charges[0] = 1.0;
      posCharges[0] = pos[molInd];
      
      Molecule molNew = Molecule( M, a, charges, posCharges );
      mol_.push_back( molNew );
    }
    
    const int vals           = nvals;
    BesselConstants bConsta  = BesselConstants( 2*vals );
    BesselCalc bCalcu        = BesselCalc( 2*vals, &bConsta);
    SHCalcConstants SHConsta = SHCalcConstants( 2*vals );
    SHCalc SHCalcu           = SHCalc( 2*vals, &SHConsta );
    System sys               = System( const_, mol_ );
    ReExpCoeffsConstants re_exp_consts (sys.get_consts().get_kappa(), sys.get_lambda(), nvals);
    
    ASolver ASolvTest        = ASolver( 2, vals, &bCalcu, &SHCalcu, &sys, &re_exp_consts);
    
  }
  
};



#endif /* ASolverTest_h */

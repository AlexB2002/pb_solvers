//
//  main.cpp
//  pb_solvers_code
//
//  Created by David Brookes on 9/24/15.
//  Copyright © 2015 David Brookes. All rights reserved.
//

#include <iostream>
#include "Constants.h"
#include "MyMatrix.h"
#include "ASolver.h"
#include "BesselCalc.h"
#include "SHCalc.h"
#include "System.h"
#include "util.h"
#include <memory>
#include "setup.h"
#include "readinput.h"
#include "BD.h"

using namespace std;

int main(int argc, const char * argv[])
{
  string input_file = argv[0];
  CSetup setup ;
  
  readInputFile(input_file, setup);
  
  Constants consts = setup.setup_constants();
  System sys = setup.setup_system(consts);
  
}










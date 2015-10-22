//
//  System.cpp
//  pb_solvers_code
//
//  Created by David Brookes on 9/28/15.
//  Copyright © 2015 David Brookes. All rights reserved.
//

#include "System.h"


Molecule::Molecule(int M, double a, vector<double> qs, vector<Pt> pos)
:M_(M), a_(a), qs_(qs), pos_(pos)
{
  // calculate the center of the molecule (first moment of charge):
  double xc, yc, zc;
  xc = 0;
  yc = 0;
  zc = 0;
  for (int i = 0; i < M_; i++)
  {
    xc += qs[i] * pos_[i].x();
    yc += qs[i] * pos_[i].y();
    zc += qs[i] * pos_[i].z();
  }
  xc /= M_;
  yc /= M_;
  zc /= M_;
  
  center_ = Pt(xc, yc, zc);
  
}



System::System(Constants consts, const vector<Molecule>& mols)
:consts_(consts), molecules_(mols), N_((int) mols.size())
{
  lambda_ = 0;
  for (int i = 0; i < N_; i++)
  {
    lambda_ += get_ai(i);
  }
  lambda_ /= N_;
}
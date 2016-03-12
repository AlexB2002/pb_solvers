//
//  EnergyForce.h
//  pb_solvers_code
//
//  Created by David Brookes on 11/18/15.
//  Copyright © 2015 David Brookes. All rights reserved.
//

#ifndef EnergyForce_h
#define EnergyForce_h

#include <stdio.h>
#include <memory>
#include "ASolver.h"

using namespace std;



/*
 Class for calculating the energy of molecules in the system given
 an ASolver object
 */
class EnergyCalc
{
protected:
  shared_ptr<VecOfMats<cmplx>::type> _A_;
  shared_ptr<VecOfMats<cmplx>::type> _L_;
  int N_;  // number of molecules
  int p_;  // max number of poles
  shared_ptr<Constants> _const_;
  
  /*
   Enum for the units of energy
   */
  enum WhichUnit { INTER, KCALMOL, KT, JMOL };
  
  MyVector<double> omega_;  // result of energy calculation, internal units
  
public:
  EnergyCalc() { }
  
  EnergyCalc(shared_ptr<VecOfMats<cmplx>::type> _A,
             shared_ptr<VecOfMats<cmplx>::type> _L,
             shared_ptr<Constants> _const, int N, int p);
  
  EnergyCalc(ASolver asolv);
  
//  EnergyCalc(ASolver asolv, Constants consts, int p);
  
  // fill omega_
  void calc_energy();
  
  // get the energy for a specific molecule:
  double get_omega_i_int(int i)  { return omega_[i]; }
  // get all energy:
  MyVector<double> get_omega_int() { return omega_; }
  
  // energy in kCal/mol:
  double get_omega_i_kcal(int i)
  { return _const_->convert_int_to_kcal_mol(omega_[i]); }
  MyVector<double> get_omega_kcal()
  {
    MyVector<double> omeg(N_);
    for (int n = 0; n < N_; n++)
      omeg[n] = _const_->convert_int_to_kcal_mol(omega_[n]);
    return omeg;
  }
  
  // energy in kT:
  double get_omega_i_kT(int i)
  { return _const_->convert_int_to_kT(omega_[i]); }
  MyVector<double> get_omega_kT()
  {
    MyVector<double> omeg(N_);
    for (int n = 0; n < N_; n++)
      omeg[n] = _const_->convert_int_to_kT(omega_[n]);
    return omeg;
  }
  
  // energy in joules/mol:
  double get_omega_i_jmol(int i)
  { return _const_->convert_int_to_jmol(omega_[i]); }
  MyVector<double> get_omega_jmol()
  {
    MyVector<double> omeg(N_);
    for (int n = 0; n < N_; n++)
      omeg[n] = _const_->convert_int_to_jmol(omega_[n]);
    return omeg;
  }
};

/*
 Class for calculating the forces on molecules in the system given
 an ASolver object
 */
class ForceCalc
{
protected:
  shared_ptr<VecOfMats<cmplx>::type> _A_;
  shared_ptr<VecOfMats<cmplx>::type> _L_;
  
  shared_ptr< MyMatrix<VecOfMats<cmplx>::type > > _gradA_;
  shared_ptr< MyVector<VecOfMats<cmplx>::type > > _gradL_;
  
  double epsS_;
  int N_;
  int p_;
  shared_ptr<Constants> _const_;
  
  VecOfVecs<double>::type F_;
  
public:
  ForceCalc() { }
  
  ForceCalc(shared_ptr<VecOfMats<cmplx>::type> _A,
            shared_ptr<MyMatrix<VecOfMats<cmplx>::type > > _gradA,
            shared_ptr<VecOfMats<cmplx>::type> _L,
            shared_ptr<MyVector<VecOfMats<cmplx>::type > > _gradL,
            shared_ptr<Constants> con, int N, int p);
  
  ForceCalc(ASolver asolv);
  
  void calc_force();  // fill F_
  
  MyVector<double> get_fi(int i)     { return F_[i]; }
  VecOfVecs<double>::type get_F()    { return F_; }
  
};

/*
 Class for calculating the torque on every molecule in the system
 */
class TorqueCalc
{
protected:
  
  // outer vector has an entry for every molecule. Inner vector is the torque
  // on that molecule
  VecOfVecs<double>::type tau_;
  
  shared_ptr<SHCalc> _shCalc_;
  shared_ptr<BesselCalc> _bCalc_;
  shared_ptr< MyVector<VecOfMats<cmplx>::type > > _gradL_;
  
  shared_ptr<Constants> _consts_;
  shared_ptr<System> _sys_;
  shared_ptr<VecOfMats<cmplx>::type> _gamma_;
  
//  double epsS_;
  int N_;
  int p_;

//  /*
//   Calculate H vector (eq 42 and 43 in Lotan 2006)
//   */
//  VecOfMats<cmplx>::type calc_H(int i);
  
public:
  TorqueCalc() { }
  
  TorqueCalc(shared_ptr<SHCalc> _shCalc,
             shared_ptr<BesselCalc> _bCalc,
             shared_ptr<MyVector<VecOfMats<cmplx>::type> > _gradL,
             shared_ptr<VecOfMats<cmplx>::type> _gamma,
             shared_ptr<Constants> _consts,
             shared_ptr<System> sys, int p);
  
  TorqueCalc(ASolver asolv);
  
  void calc_tau();  // fill tau_
  
  /*
   Calculate H vector (eq 42 and 43 in Lotan 2006)
   */
  VecOfMats<cmplx>::type calc_H(int i);
  
  MyVector<double> get_taui(int i)     { return tau_[i]; }
  VecOfVecs<double>::type get_Tau()    { return tau_; }
  
  /*
   Calculate inner product of two matrices as defined in equation 29 of Lotan
   2006
   */
  double lotan_inner_prod(MyMatrix<cmplx> U, MyMatrix<cmplx> V, int p)
  {
    double ip = 0;
    int n, m, mT;
    for (n = 0; n < p; n++)
    {
      for (m = -n; m <= n; m++)
      {
        mT = (m < 0) ? -1*m : m;
        ip += U(n, mT+p_).real()*V(n, mT+p_).real()
               + U(n, mT+p_).imag()*V(n, mT+p_).imag();
      }
    }
    return ip;
  }
  
};

#endif /* EnergyForce_h */

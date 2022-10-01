/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-09-30 23:03:40  
 * @Description: 
 */
#pragma once

#include "db/ctrlsrc.hpp"

class Vccs: public CtrlSrc
{

public:
  Vccs(const std::string &name):CtrlSrc(name) { }  
  virtual ~Vccs() {}
  
  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

};


void Vccs::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{ 
  // value * (vc+ - vc-) + other current
  // current flow out is positive
  G.add(_pnode, _ctrl_pnode, _value);
  G.add(_pnode, _ctrl_nnode, -_value);
  // current flow out is negative
  G.add(_nnode, _ctrl_pnode, -_value);
  G.add(_nnode, _ctrl_nnode, _value);
}
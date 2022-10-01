/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-09-30 22:46:09  
 * @Description: 
 */
#pragma once

#include <string>

#include "db/ctrlsrc.hpp"


class Vcvs: public CtrlSrc 
{
public:
  Vcvs(const std::string &name):CtrlSrc(name) { }
  virtual ~Vcvs() {}
  
  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

  void setAux(int n) { _aux_node = n; }
  int getAux() { return _aux_node; }

private:
  int _aux_node;

};

void Vcvs::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U) {
  // additional current from controlled source
  // current flow out is positive
  G.add(_pnode, _aux_node, 1);
  // current flow out is negative
  G.add(_nnode, _aux_node, -1);
  // v+ - v- = value * (vc+ - vc-)
  // v+ - v- + value * (vc- - vc+) = 0
  G.add(_aux_node, _pnode, 1);
  G.add(_aux_node, _nnode, -1);
  G.add(_aux_node, _ctrl_pnode, -_value);
  G.add(_aux_node, _ctrl_nnode, _value);
}
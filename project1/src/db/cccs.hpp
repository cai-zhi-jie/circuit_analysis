/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-09-30 23:03:40  
 * @Description: 
 */
#pragma once

#include "db/ctrlsrc.hpp"

class Cccs: public  CtrlSrc
{
public:
  Cccs(const std::string &name):CtrlSrc(name), _exist_ctrl_aux(true), 
      _ctrl_aux_node(0), _ctrl_value(0), _ctrl_name(""){}
  virtual ~Cccs() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

  void setCtrlAux(int n) { _ctrl_aux_node = n; } 
  int getCtrlAux() const { return _ctrl_aux_node; } 
  void setCtrlName(const std::string &d) { _ctrl_name = d; }
  std::string getCtrlName() const { return _ctrl_name; }
  void setExistCtrlAux(bool b) { _exist_ctrl_aux = b; }
  bool getExistCtrlAux() { return _exist_ctrl_aux; }
  void setCtrlValue(double v) {  _ctrl_value = v; }
  double setCtrlValue() {  return _ctrl_value; }


private:
  bool _exist_ctrl_aux;
  int _ctrl_aux_node;
  double _ctrl_value;
  std::string _ctrl_name;

};

void Cccs::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U) {
  // Icccs = F * Ictrl
  // current flow out is positive
  G.add(_pnode, _ctrl_aux_node, _value);
  // current flow out is negative
  G.add(_nnode, _ctrl_aux_node, -_value);
  // if the control source is not a voltage source
  // adding aux_node for it (like R\C)
  // (eg: Vcp - Vcn  = Ictrl * Rctrl_)
  if(_exist_ctrl_aux == false) {
     if(_ctrl_name[0]=='R') { 
      // (V+ - V-) = R * iR
       G.add(_ctrl_aux_node, _ctrl_pnode, 1);
       G.add(_ctrl_aux_node, _ctrl_nnode, -1);
       G.add(_ctrl_aux_node, _ctrl_aux_node, -_ctrl_value);
     } else if(_ctrl_name[0]=='C') {
      // C * (V^+ - V^-) = iC
       C.add(_ctrl_aux_node, _ctrl_pnode, 1);
       C.add(_ctrl_aux_node, _ctrl_nnode, -1);
       G.add(_ctrl_aux_node, _ctrl_aux_node, -1.0/_ctrl_value);
     }
    
  }  
}


/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-09-30 23:03:40  
 * @Description: 
 */
#pragma once

#include "db/ctrlsrc.hpp"

class Ccvs: public  CtrlSrc
{
public:
  Ccvs(const std::string &name): CtrlSrc(name), _exist_ctrl_aux(true), 
      _ctrl_aux_node(0), _ctrl_value(0), _ctrl_name(""){}
  virtual ~Ccvs() {}

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

void Ccvs::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U) {
  // Vp - Vn = value * Ictrl
  G.add(_aux_node, _pnode, 1);
  G.add(_aux_node, _nnode, -1);
  G.add(_aux_node, _ctrl_aux_node, -_value);
  // current flow out is negative
  G.add(_pnode, _aux_node, -1);
  // current flow out is positive
  G.add(_nnode, _aux_node, 1);
  // deal with the control aux node
  if (_exist_ctrl_aux == false)  {
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
/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 12:40:07
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 14:14:38
 */
#pragma once

#include "db/device.hpp"
// Vname PNode Nnode Type Value
class Vsrc : public Device
{
public:
  Vsrc(const std::string &name = "", const std::string type = "")
      : Device(name), _type(type) {}
  virtual ~Vsrc() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

  void setType(std::string t) { _type = t; }
  std::string getType() const { return _type; }
  void setSrcIdx(int id) { _src_idx = id; }
  int getSrcIdx(int id) const { return _src_idx; }
  
private:
  int _src_idx;
  std::string _type;
};

void Vsrc::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  if (_pnode != 0) {
    G.add(_pnode, _aux_node, -1);
    G.add(_aux_node, _pnode, 1);
  }
  if (_nnode != 0) {
    G.add(_nnode, _aux_node, 1);
    G.add(_aux_node, _nnode, -1);
  }
  B.add(_aux_node, _src_idx, 1);
  I.add(_src_idx, 1, _value);
  U.add(_src_idx, 1, _name);
  std::cout << _name << " " << _pnode << " " << _nnode << " " << _aux_node << " " << _src_idx << " " << _value << std::endl;
}

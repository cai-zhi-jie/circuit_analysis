/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 13:14:21
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 14:14:20
 */
#pragma once

#include "db/device.hpp"

class Resistor : public Device
{
public:
  Resistor(const std::string &name) : Device(name) {}
  virtual ~Resistor() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);
};

void Resistor::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  
  if (_nnode == 0 && _pnode == 0) {
    // short to "0" GROUND
  } else if (_nnode == 0 || _pnode == 0) {
    int nid = _nnode + _pnode - 1;
    G.add(nid, nid, 1.0 / _value);
  } else {
    G.add(_pnode, _pnode, 1.0 / _value);
    G.add(_nnode, _nnode, 1.0 / _value);
    G.add(_pnode, _nnode, -1.0 / _value);
    G.add(_nnode, _pnode, -1.0 / _value);
  }
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}
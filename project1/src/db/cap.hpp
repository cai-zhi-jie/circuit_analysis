/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-27 14:09:23
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-30 23:25:32
 */
#pragma once

#include "db/device.hpp"

class Capacitor : public Device
{
public:
  Capacitor(const std::string& name) : Device(name) {}
  virtual ~Capacitor() {}

  virtual void stamp(Matrix& C, Matrix& G, Matrix& B, Matrix& I, Mat<std::string>& U);
};

void Capacitor::stamp(Matrix& C, Matrix& G, Matrix& B, Matrix& I, Mat<std::string>& U)
{
  if (_nnode == 0 && _pnode == 0) {
    // short to "0" GROUND
  } else if (_nnode == 0 || _pnode == 0) {
    int nid = _nnode + _pnode;
    C.add(nid, nid, _value);
  } else {
    C.add(_pnode, _pnode, _value);
    C.add(_nnode, _nnode, _value);
    C.add(_pnode, _nnode, -_value);
    C.add(_nnode, _pnode, -_value);
  }
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}

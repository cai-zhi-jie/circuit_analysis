/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-27 14:10:56
 * @Description: 
 */

#pragma once

#include "db/device.hpp"

class Inductor : public Device
{
public:
  Inductor(const std::string &name) : Device(name) {}
  virtual ~Inductor() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

private:
};

void Inductor::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  // to be implemented
  if (_nnode != 0 || _pnode != 0) {
    C.add(_aux_node, _aux_node, _value);
  }
  if (_pnode != 0) {
    G.add(_pnode, _aux_node, 1);
    G.add(_aux_node, _pnode, -1);
  }
  if (_nnode != 0) {
    G.add(_nnode, _aux_node, -1);
    G.add(_aux_node, _nnode, 1);
  }
  // std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _aux_node << " " << _value << std::endl;
}
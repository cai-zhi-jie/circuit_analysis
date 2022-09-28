/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-27 14:10:56
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 14:14:11
 */

#pragma once

#include "db/device.hpp"

class Inductor : public Device
{
public:
  Inductor(const std::string &name) : Device(name) {}
  virtual ~Inductor() {}

  void setAux(int s) { _aux_node = s; }
  int getAux() const { return _aux_node; }

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B);

private:
  int _aux_node;
};

void Inductor::stamp(Matrix &C, Matrix &G, Matrix &B)
{
  // to be implemented
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}
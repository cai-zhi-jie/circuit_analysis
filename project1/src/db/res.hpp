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

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B);
};

void Resistor::stamp(Matrix &C, Matrix &G, Matrix &B)
{
  // to be implemented
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}
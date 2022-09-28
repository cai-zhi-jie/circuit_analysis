/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-27 14:09:23
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 13:06:45
 */
#pragma once

#include "db/device.hpp"

class Capacitor : public Device
{
public:
  Capacitor(const std::string &name) : Device(name) {}
  virtual ~Capacitor() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B);
};

void Capacitor::stamp(Matrix &C, Matrix &G, Matrix &B)
{
  // to be implemented
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}

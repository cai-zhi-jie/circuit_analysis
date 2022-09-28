/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-27 10:43:39
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 13:07:26
 */
#pragma once

#include <iostream>
#include <string>

#include "utils/mat.hpp"

class Device
{
public:
  Device(const std::string &name)
      : _name(name), _pnode(0), _nnode(0), _value(0) {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B) = 0;

  virtual ~Device() {}

  void setPnode(int p) { _pnode = p; }
  int getPnode() const { return _pnode; }
  void setNnode(int p) { _nnode = p; }
  int getNnode() const { return _nnode; }
  void setValue(double d) { _value = d; }
  double getValue() const { return _value; }
  void setName(const std::string &n) { _name = n; }
  std::string getName() const { return _name; }

protected:
  std::string _name;
  int _pnode;
  int _nnode;

  /// \brief value
  double _value;
};

/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 12:54:11
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 14:14:15
 */

#pragma once

#include "db/device.hpp"

// Iname PNode Nnode Type Value
class Isrc : public Device
{
public:
  Isrc(const std::string &name) : Device(name) {}
  virtual ~Isrc() {}

  void setType(std::string t) { _type = t; }
  std::string getType(std::string t) const { return _type; }

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B);

private:
  std::string _type;
};

void Isrc::stamp(Matrix &C, Matrix &G, Matrix &B)
{
  // to be implemented
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}

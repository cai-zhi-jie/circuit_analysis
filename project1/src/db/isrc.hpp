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
  void setSrcIdx(int id) { _src_idx = id; }
  int getSrcIdx(int id) const { return _src_idx; }
  
  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

private:
  int _src_idx;
  std::string _type;
};

void Isrc::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  if (_pnode != 0) {
    B.add(_pnode, _src_idx, -1);
  }
  if (_nnode != 0) {
    B.add(_nnode, _src_idx, 1);
  }
  I.add(_src_idx, 1, _value);
  U.add(_src_idx, 1, _name);
  std::cout << _name << " " << _pnode << " " << _nnode << " " << _src_idx << " " << _value << std::endl;
}

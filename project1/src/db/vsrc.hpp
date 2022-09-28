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
  Vsrc(const std::string &name = "", const std::string type = "", const int aux = -1)
      : Device(name), _aux_node(aux), _type(type) {}
  virtual ~Vsrc() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B);

  void setAux(int s) { _aux_node = s; }
  int getAux() const { return _aux_node; }
  void setType(std::string t) { _type = t; }
  std::string getType() const { return _type; }

private:
  int _aux_node;
  std::string _type;
};

void Vsrc::stamp(Matrix &C, Matrix &G, Matrix &B)
{
  // to be implemented
  std::cout << _name << ' ' << _pnode << ' ' << _nnode << ' ' << _value << std::endl;
}

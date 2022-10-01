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
      : _name(name), _pnode(0), _nnode(0), _value(0),
        _aux_node(0), _aux_node_name(""), _exist_aux_node(false) {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U) = 0;

  virtual ~Device() {}

  void setPnode(int p) { 
    if (p < 0) {
      std::cout << "ERROR: negtive node index!" << std::endl;
      p = 0;
    }
    _pnode = p;
  }
  int getPnode() const { return _pnode; }
  void setNnode(int n) {
    if (n < 0) {
      std::cout << "ERROR: negtive node index!" << std::endl;
      n = 0;
    }
    _nnode = n; 
  }
  int getNnode() const { return _nnode; }
  void setValue(double d) { _value = d; }
  double getValue() const { return _value; }
  void setName(const std::string &n) { _name = n; }
  std::string getName() const { return _name; }
  void setAux(int s) { _aux_node = s; _exist_aux_node = true; }
  int getAux() const { return _exist_aux_node ? _aux_node : -1; }
  void setAuxName(std::string s) { _aux_node_name = s; }
  std::string getAuxName() const { return _aux_node_name; }
  bool existAux() { return _exist_aux_node; }

protected:
  std::string _name;
  int _pnode;
  int _nnode;
  double _value;
  
  bool _exist_aux_node;
  int _aux_node;
  std::string _aux_node_name;
};

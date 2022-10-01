/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-09-30 23:03:40  
 * @Description: 
 */
#pragma once

#include "db/device.hpp"

class CtrlSrc: public Device
{
public:
  CtrlSrc(const std::string &name):Device(name) {}
  virtual ~CtrlSrc() {}

  void setCtrlPnode(int p) { 
    if (p < 0) {
      std::cout << "ERROR: negtive node index!" << std::endl;
      p = 0;
    }
    _ctrl_pnode = p;
  }
  int getCtrlPnode() { return _ctrl_pnode; }
  void setCtrlNnode(int n) { 
    if (n < 0) {
      std::cout << "ERROR: negtive node index!" << std::endl;
      n = 0;
    }
    _ctrl_nnode = n; 
  }
  int getCtrlNnode() { return _ctrl_nnode; }
  void setCtrlName(std::string n) { _ctrl_name = n; }
  std::string getCtrlName() { return _ctrl_name; }

protected:
  int _ctrl_nnode;
  int _ctrl_pnode;
  std::string _ctrl_name;

  /* data */
};
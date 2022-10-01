/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-28 14:14:43 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-30 23:45:40
 */
#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>


#include "db/cap.hpp"
#include "db/cccs.hpp"
#include "db/ccvs.hpp"
#include "db/ctrlsrc.hpp"
#include "db/device.hpp"
#include "db/ind.hpp"
#include "db/isrc.hpp"
#include "db/mut.hpp"
#include "db/res.hpp"
#include "db/vsrc.hpp"
#include "db/vccs.hpp"
#include "db/vcvs.hpp"

#include "utils/mat.hpp"

class DB
{
public:
  DB() {}
  ~DB() {
    for (auto d : _dev_list) {
      delete d;
    }
  }

  void show();
  void setup();
  void output(std::string fn);
  
  std::vector<Device*> _dev_list;
  std::map<std::string,std::set<int>> _dev_idx;
  std::map<std::string, int> _dev_name2dev_idx;
  std::map<std::string, int> _node_list;
  // _aux_node_list["Type"]["node name"] = node_idx
  std::map<std::string, std::map<std::string, int>> _aux_node_list;
 
  std::vector<int> _probe_list;
  std::set<std::string> _probe_set;
 
  Matrix _C;
  Matrix _G;
  Matrix _B;
  Matrix _LT;
  Matrix _I;
  Mat<std::string> _X;
  Mat<std::string> _Y;
  Mat<std::string> _U;

  int _num_in;
  int _num_out;
  std::vector<int> _src_list;

  int _num_node;
  int _num_act_node;
  int _num_aux_node;
  
};

void DB::setup() {
  // C·X' + G·X = B·U
  // Y = LT·X
  // X===node(unknown)
  // Y===output
  // U===input(name)
  // I===input(value)
  _C = Matrix(_num_node - 1, _num_node - 1, "C");
  _G = Matrix(_num_node - 1, _num_node - 1, "G");
  _B = Matrix(_num_node - 1, _num_in, "B");
  _I = Matrix(_num_in, 1, "I");
  _LT = Matrix(_num_out, _num_node - 1, "LT");
  _X = Mat<std::string>(_num_node - 1, 1, "X");
  _Y = Mat<std::string>(_num_out, 1, "Y");
  _U = Mat<std::string>(_num_in, 1, "U");
  for (auto p : _node_list) {
    _X.add(p.second, 1, "V<" + p.first + ">");
  }
  for (auto pp : _aux_node_list) {
    for (auto p : pp.second) {
      _X.add(p.second, 1, p.first);
    }
  }
  for (int i = 0; i < _probe_list.size(); i++) {
    _Y.add(i + 1, 1, _X.get(_probe_list[i], 1));
  }
  for(auto d : _dev_list){
    d->stamp(_C, _G, _B, _I, _U);
  }
  show();
}

void DB::show() {
  std::cout << "=====node list=====" << std::endl;
  for (auto p : _node_list) {
    std::cout << p.second << "\t" << p.first << std::endl;
  }
  std::cout << "=====aux node list=====" << std::endl;
  for (auto l : _aux_node_list) {
    std::cout << l.first << std::endl;
    for (auto p : l.second) {
      std::cout << p.second << "\t" << p.first << std::endl;
    }
  }
  std::cout << "=====dev list=====" << std::endl;
  for (auto d : _dev_list) {
    std::cout << d->getName() << " " << d->getPnode() << " " << d->getNnode() << " " << d->getValue() << std::endl;   
  }
}

void DB::output(std::string fn) {
  _C.output(fn);
  _G.output(fn);
  _B.output(fn);
  _LT.output(fn);
  _X.output(fn, 12);
  _Y.output(fn, 12);
  _U.output(fn, 12);
  _I.output(fn);
}
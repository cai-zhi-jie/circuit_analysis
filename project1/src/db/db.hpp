/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-28 14:14:43 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 16:23:27
 */
#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>


#include "db/cap.hpp"
#include "db/device.hpp"
#include "db/ind.hpp"
#include "db/isrc.hpp"
#include "db/mut.hpp"
#include "db/res.hpp"
#include "db/vsrc.hpp"

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
  
  std::vector<Device*> _dev_list;
  std::map<std::string, int> _node_list;
  std::map<std::string, int> _aux_node_list;
 
  std::vector<int> _probe_list;
  std::set<std::string> _probe_set;
 
  Matrix _C;
  Matrix _G;
  Matrix _B;
  Matrix _LT;

  int _num_in;
  int _num_out;
  
};


void DB::show() {
  std::cout << "=====node list=====" << std::endl;
  for (auto p : _node_list) {
    std::cout << p.second << "\t\t" << p.first << std::endl;
  }
  std::cout << "=====aux node list=====" << std::endl;
  for (auto p : _aux_node_list) {
    std::cout << p.second << "\t\t" << p.first << std::endl;
  }
  std::cout << "=====dev list=====" << std::endl;
  for (auto d : _dev_list) {
    std::cout << d->getName() << " " << d->getPnode() << " " << d->getNnode() << " " << d->getValue() << std::endl;   
  }
}
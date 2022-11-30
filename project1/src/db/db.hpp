/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-28 14:14:43 
 * @Description: 
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
  void output(std::string fn, bool sparse_output = false);
  
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

  Matrix subC;
  Matrix subL;
  Matrix subG;
  Matrix subE;
  Matrix subET;
  Mat<std::string> subV;
  Mat<std::string> subI;

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
  
  subC = Matrix(_num_act_node - 1, _num_act_node - 1, "subC");
  subL = Matrix(_num_aux_node, _num_aux_node, "subL");
  subG = Matrix(_num_act_node - 1, _num_act_node - 1, "subG");
  subE = Matrix(_num_act_node - 1, _num_aux_node, "subE");
  subET = Matrix(_num_aux_node, _num_act_node - 1, "sub-E^T");
  subV = Mat<std::string>(_num_act_node - 1, 1, "subV");
  subI = Mat<std::string>(_num_aux_node, 1, "subI");
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
    _LT.add(i + 1, _probe_list[i], 1);
  }
  for(auto d : _dev_list){
    d->stamp(_C, _G, _B, _I, _U);
  }
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

void DB::output(std::string fn, bool sparse_output) {
  // output full matrix in txt
  std::string fn_tmp = fn + "_full.txt";
  std::remove(fn_tmp.c_str());
  _C.output(fn_tmp);
  _G.output(fn_tmp);
  _B.output(fn_tmp);
  _LT.output(fn_tmp);
  _X.output(fn_tmp, 12);
  _Y.output(fn_tmp, 12);
  _U.output(fn_tmp, 12);
  _I.output(fn_tmp);
  // output sparse matrix in binary
  fn_tmp = fn + "_binary.dat";
  std::remove(fn_tmp.c_str());
  _C.binary_output(fn_tmp);
  _G.binary_output(fn_tmp);
  _B.binary_output(fn_tmp);
  _LT.binary_output(fn_tmp);
  _X.binary_output(fn_tmp);
  _Y.binary_output(fn_tmp);
  _U.binary_output(fn_tmp);
  _I.binary_output(fn_tmp);
  // output sparse matrix in txt
  if (sparse_output) {
    fn_tmp = fn + "_sparse.txt";
    std::remove(fn_tmp.c_str());
    _C.sparse_output(fn_tmp);
    _G.sparse_output(fn_tmp);
    _B.sparse_output(fn_tmp);
    _LT.sparse_output(fn_tmp);
    _X.sparse_output(fn_tmp);
    _Y.sparse_output(fn_tmp);
    _U.sparse_output(fn_tmp);
    _I.sparse_output(fn_tmp);
  }
  // output sub matrix
  fn_tmp = fn + "_sub_matrix.txt";
  std::remove(fn_tmp.c_str());
  _C.getSubMatrix(1, 1, _num_act_node-1, _num_act_node-1, subC);
  _C.getSubMatrix(_num_act_node, _num_act_node, _num_aux_node, _num_aux_node, subL);
  _G.getSubMatrix(1, 1, _num_act_node-1, _num_act_node-1, subG);
  _G.getSubMatrix(1, _num_act_node, _num_act_node-1, _num_aux_node, subE);
  _G.getSubMatrix(_num_act_node, 1, _num_aux_node, _num_act_node-1, subET);
  _X.getSubMatrix(1, 1, _num_act_node-1, 1, subV);
  _X.getSubMatrix(_num_act_node, 1, _num_aux_node, 1, subI);

  subC.output(fn_tmp);
  subL.output(fn_tmp);
  subG.output(fn_tmp);
  subE.output(fn_tmp);
  subET.output(fn_tmp);
  subV.output(fn_tmp);
  subI.output(fn_tmp);

  fn_tmp = fn + "_zero.txt";
  std::remove(fn_tmp.c_str());
  Matrix zero1(0,0,"zero1");
  Matrix zero2(0,0,"zero2");
  Matrix zero3(0,0,"zero3");
  _C.getSubMatrix(1, _num_act_node, _num_act_node-1, _num_aux_node, zero1);
  _C.getSubMatrix(_num_act_node, 1, _num_aux_node, _num_act_node-1, zero2);
  _G.getSubMatrix(_num_act_node, _num_act_node, _num_aux_node, _num_aux_node, zero3);
  zero1.output(fn_tmp);
  zero2.output(fn_tmp);
  zero3.output(fn_tmp);
}
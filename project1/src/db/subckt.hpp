/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 12:55:18
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-30 23:48:00
 */
#pragma once

#include <fstream>

#include "db/db.hpp"
#include "utils/util.hpp"

class Subckt
{
public:
  Subckt() {}
  ~Subckt() {}

  void instantiate(std::string xline, std::string filename, std::shared_ptr<DB> _db);
};

void Subckt::instantiate(std::string xline, std::string filename, std::shared_ptr<DB> _db) {
  // Xname N1 N2 .. SUBNAME
  // initialize external port node in X commane
  std::vector<std::string> tokens;
  std::string delims(" \n\r()\t");
  std::string tmp;
  while ((tmp = tokenizer(xline, delims)) != "") {
    tokens.emplace_back(tmp);
  }
  if (tokens.size() <= 2) {
    std::cout << "ERROR: syntax error, to instantiate a sub-circuit, the command must have more than two tokens!" << std::endl;
    return ;
  }
  std::string instance_name = tokens.front();
  std::string subckt_name = tokens.back();
  std::vector<int> ext_port_node_list;
  for (int i = 1; i < static_cast<int>(tokens.size()) - 1; i++) {
    if (tokens[i] != "0") {
    } 
    if (_db->_node_list.find(tokens[i]) == _db->_node_list.end()) {
      _db->_node_list[tokens[i]] = _db->_node_list.size();
    }
    ext_port_node_list.emplace_back(_db->_node_list[tokens[i]]);
  }
  
  std::ifstream fin(filename);
  
  std::string line;
  while(std::getline(fin, line)) {
    std::string tmp;
    capitalize(line);
    tmp = tokenizer(line, delims);
    // find the target SUBCKT
    if (tmp != ".SUBCKT" || tokenizer(line, delims) != subckt_name) continue;
    // initialize local port node in SUBCKT
    std::map<std::string, int> local_port_node_list;
    int i = 0;
    while ((tmp = tokenizer(line, delims)) != "") {
      local_port_node_list[tmp] = i++;
    }
    auto add_node = [&](std::string n)->int{
      if (n == "0") return 0;
      if (local_port_node_list.find(n) == local_port_node_list.end()) {
        // is internal node
        n = n + "#" + instance_name;
        if (_db->_node_list.find(n) == _db->_node_list.end()) {
          _db->_node_list[n] = _db->_node_list.size();
        }
        return _db->_node_list[n];
      } else {
        return ext_port_node_list[local_port_node_list[n]];
      }
    };
    
    while (std::getline(fin, line)) {
      capitalize(line);
      char first_letter = line[0];
      if (first_letter == '*' || line.size() == 0) continue;
      std::string whole_line = line;
      std::vector<std::string> tmp_tokens;
      while((tmp = tokenizer(line, delims)) != "") {
        tmp_tokens.emplace_back(tmp);
      }
      if (tmp_tokens.size() == 0) continue;
      std::string dev_name = tmp_tokens[0] + "#" + instance_name;
      if (first_letter == 'R'){
        Resistor* r = new Resistor(dev_name);
        r->setPnode(add_node(tmp_tokens[1]));
        r->setNnode(add_node(tmp_tokens[2]));
        r->setValue(to_double(tmp_tokens[3]));
        _db->_dev_idx["R"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
      } else if (first_letter == 'C'){
        Capacitor* r = new Capacitor(dev_name);
        r->setPnode(add_node(tmp_tokens[1]));
        r->setNnode(add_node(tmp_tokens[2]));
        r->setValue(to_double(tmp_tokens[3]));
        _db->_dev_idx["C"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
      } else if (first_letter == 'I'){
        Isrc* r = new Isrc(dev_name);
        r->setPnode(add_node(tmp_tokens[1]));
        r->setNnode(add_node(tmp_tokens[2]));
        tmp = tokenizer(line, delims);
        std::string tmp2 = tokenizer(line, delims);
        if (tmp_tokens.size() == 5) {
          r->setType(tmp_tokens[3]);
        }
        r->setValue(to_double(tmp_tokens.back()));
        _db->_dev_idx["I"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
        (_db->_num_in)++;
      } else if (first_letter == 'V'){
        Vsrc* r = new Vsrc(dev_name);
        r->setPnode(add_node(tmp_tokens[1]));
        r->setNnode(add_node(tmp_tokens[2]));
        if (tmp_tokens.size() == 5) {
          r->setType(tmp_tokens[3]);
        }
        r->setValue(to_double(tmp_tokens.back()));
        _db->_dev_idx["V"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
        (_db->_num_in)++;
      } else if (first_letter == 'L'){
        Inductor* r = new Inductor(dev_name);
        r->setPnode(add_node(tmp_tokens[1]));
        r->setNnode(add_node(tmp_tokens[2]));
        r->setValue(to_double(tmp_tokens[3]));
        _db->_dev_idx["L"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
      } else if (first_letter == 'K'){
        Mutual* r = new Mutual(dev_name);
        r->setInd1(tmp_tokens[1] + "#" + instance_name);
        r->setInd2(tmp_tokens[2] + "#" + instance_name);
        r->setValue(to_double(tmp_tokens[3]));
        _db->_dev_idx["K"].emplace(_db->_dev_list.size());
        _db->_dev_list.emplace_back(r);
      } else if (first_letter == 'X') {
        Subckt s;
        s.instantiate(whole_line , filename, _db);
      } else if (first_letter == 'E') {
        Vcvs* e = new Vcvs(dev_name);
        e->setPnode(add_node(tokens[1]));
        e->setNnode(add_node(tokens[2]));
        e->setCtrlPnode(add_node(tokens[3]));  
        e->setCtrlNnode(add_node(tokens[4]));   
        e->setValue(to_double(tokens[5]));
        _db->_dev_idx["E"].emplace(_db->_dev_list.size());
        _db->_dev_list.push_back(e);
      } else if (first_letter == 'G'){
      Vccs* g = new Vccs(dev_name);
      g->setPnode(add_node(tokens[1]));
      g->setNnode(add_node(tokens[2]));
      g->setCtrlPnode(add_node(tokens[3]));  
      g->setCtrlNnode(add_node(tokens[4]));   
      g->setValue(to_double(tokens[5]));
      _db->_dev_idx["G"].emplace(_db->_dev_list.size());
      _db->_dev_list.push_back(g);
    } else if (first_letter == 'H') {
      Ccvs* h = new Ccvs(dev_name);
      h->setPnode(add_node(tokens[1]));
      h->setNnode(add_node(tokens[2]));
      h->setCtrlName(tokens[3] + "#" + instance_name);
      h->setValue(to_double(tokens[4]));
    } else if (first_letter == 'F') {
      Cccs*  f = new Cccs(dev_name);
      f->setPnode(add_node(tokens[1]));
      f->setNnode(add_node(tokens[2]));
      f->setCtrlName(tokens[3] + "#" + instance_name);
      f->setValue(to_double(tokens[4]));    
    } else {
        if (tmp_tokens[0] == ".END" || tmp_tokens[0] == ".ENDS") {
          break;
        } else if (tmp_tokens[0] == ".SUBCKT") {
          // skip sub-circuit data since it's solved in "X" by the class Subckt
          while (getline(fin, line)) {
            tmp = tokenizer(line, delims);
            if(tmp == ".ENDS")
              break;
          }
        } else if (tmp_tokens[0] == ".PROBE" /*|| token == ".PRINT"*/) {
          tmp = tokenizer(line, delims);
          while (!tmp.empty()){
            if (tmp == "V") {
              tmp = tokenizer(line, delims);
              continue;
            }
            // make sure the probe node is within the node list
            if (_db->_node_list.find(tmp) != _db->_node_list.end()) {
              // remove duplicated probe node
              if (_db->_probe_set.find(tmp) == _db->_probe_set.end()) {
                _db->_probe_list.emplace_back(_db->_node_list[tmp]);
                _db->_probe_set.emplace(tmp);
                (_db->_num_out)++;
              }
            }
            tmp = tokenizer(line, delims);
          }
        }
      }
    }
    break;
  }
  fin.close(); 
  return ;
}
/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 10:48:08 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 22:53:52
 */
#pragma once

#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// #include "db/cap.hpp"
// #include "db/device.hpp"
// #include "db/ind.hpp"
// #include "db/isrc.hpp"
// #include "db/mut.hpp"
// #include "db/res.hpp"
// #include "db/vsrc.hpp"

// #include "utils/mat.hpp"
#include "db/db.hpp"
#include "db/subckt.hpp"
#include "utils/util.hpp"

class Stamp {
public:
  Stamp() { _db = std::make_shared<DB>(); }
  ~Stamp() {}

  void parse(char* filename);
  void output(char* filename);
  void setup();
 
private:
  std::shared_ptr<DB> _db;
};

void Stamp::setup(){
  int n = static_cast<int>(_db->_node_list.size()) - 1;
  _db->_C = Matrix(n,n);
  _db->_G = Matrix(n,n);
  _db->_B = Matrix(n, _db->_num_in);
  _db->_LT = Matrix(_db->_num_out, n);
  for(size_t i=0; i < _db->_dev_list.size(); ++i){
    _db->_dev_list[i]->stamp(_db->_C, _db->_G, _db->_B);
  }
  _db->show();
}


/// \brief Output the system matrix to disk
///
/// @param filename output file name
///
/// This function will write the system matrix to the disk. 
/// Binary file format is preferred over the ASCII one. Sparse
/// matrix structure is preferred over the dense one. 
/// 
/// \todo Please fill in this function. 
void Stamp::output(char* filename)
{
  
}

void Stamp::parse(char* filename)
{
  std::ifstream ifid(filename);
  _db->_num_in = 0;
  _db->_num_out = 0;
	
  std::string line;
  std::string delims(" \n\r()\t");
  _db->_node_list["0"] = 0;
  
  auto add_node = [](std::map<std::string, int>& nl, std::string& name)->int{
    if(nl.find(name) == nl.end()) {
      nl[name] = nl.size();
    }
    return nl[name];
  };
  while(std::getline(ifid, line)) {
    std::string tmp;
    capitalize(line);
    // skip comment
    char first_letter = line[0];
    std::string whole_line = line;
    if (first_letter == '*') continue;
    std::vector<std::string> tokens;
    while ((tmp = tokenizer(line,delims)) != "") {
      tokens.emplace_back(tmp);
    }
    if (tokens.size() == 0) continue;

    if (first_letter == 'R'){
      Resistor* r = new Resistor(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      r->setValue(to_double(tokens[3]));
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'C'){
      Capacitor* r = new Capacitor(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      r->setValue(to_double(tokens[3]));      
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'I'){
      Isrc* r = new Isrc(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      if (tokens.size() == 5) {
        r->setType(tokens[3]);
      }
      r->setValue(to_double(tokens.back()));
      _db->_dev_list.emplace_back(r);
      (_db->_num_in)++;
    } else if (first_letter == 'V'){
      Vsrc* r = new Vsrc(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      if (tokens.size() == 5) {
        r->setType(tokens[3]);
      }
      r->setValue(to_double(tokens.back()));
      _db->_dev_list.emplace_back(r);
      (_db->_num_in)++;
    } else if (first_letter == 'L'){
      Inductor* r = new Inductor(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      r->setValue(to_double(tokens[3]));
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'K'){
      Mutual* r = new Mutual(tokens[0]);
      r->setInd1(tokens[1]);
      r->setInd2(tokens[2]);
      r->setValue(to_double(tokens[3]));
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'X') {
      Subckt s;
      s.instantiate(whole_line , filename, _db);
    } else {
      // netlist file ends
      if (tokens[0] == ".END") {
        break;
      } else if (tokens[0] == ".SUBCKT") {
        // skip sub-circuit data since it's solved in "X" by the class Subckt
        while (getline(ifid,line)) {
          std::string token = tokenizer(line, delims);
          if(token == ".ENDS")
            break;
        }
      } else if (tokens[0] == ".PROBE" /*|| token == ".PRINT"*/) {
        std::string token = tokenizer(whole_line, delims);
        while (!token.empty()){
          if (token == "V") {
            token = tokenizer(whole_line, delims);
            continue;
          }
          // make sure the probe node is within the node list
          if (_db->_node_list.find(token) != _db->_node_list.end()) {
            // remove duplicated probe node
            if (_db->_probe_set.find(token) == _db->_probe_set.end()) {
              _db->_probe_list.emplace_back(_db->_node_list[token]);
              _db->_probe_set.emplace(token);
              (_db->_num_out)++;
            }
          }
          token = tokenizer(whole_line, delims);
        }
      }
    }
  }
  ifid.close();

  //aux_node
  for (auto d : _db->_dev_list) {
    std::string dev_name = d->getName();
    if (dev_name[0] == 'L') {
      Inductor* l = dynamic_cast<Inductor*>(d);
      std::string tmp = "i:" + dev_name;
      add_node(_db->_aux_node_list, tmp);
      l->setAux(_db->_aux_node_list[tmp]);
    } else if(dev_name[0] == 'V') {
      Vsrc* v = dynamic_cast<Vsrc*>(d);
      std::string tmp = "i:" + dev_name;
      add_node(_db->_aux_node_list, tmp);
      v->setAux(_db->_aux_node_list[tmp]);
    }

  }

}

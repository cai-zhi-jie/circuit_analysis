/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 10:48:08 
 * @Description: 
 */
#pragma once

#include <cmath>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


#include "db/db.hpp"
#include "db/subckt.hpp"
#include "utils/util.hpp"

class Stamp {
public:
  Stamp() { _db = std::make_shared<DB>(); }
  ~Stamp() {}

  void parse(char* filename);
  void output(char* filename, bool sparse_output = false);
  void setup();
 
private:
  std::shared_ptr<DB> _db;
};

void Stamp::setup(){
  _db->setup();
}

void Stamp::output(char* filename, bool sparse_output)
{
  std::remove(filename);
  _db->output(filename, sparse_output);
}

void Stamp::parse(char* filename)
{
  std::ifstream ifid(filename);
  _db->_num_in = 0;
  _db->_num_out = 0;
	
  std::string line;
  std::string delims(" \n\r()\t");
  _db->_node_list["0"] = 0;
  
  auto add_node = [](std::map<std::string, int>& nl, std::string name)->int{
    if (name == "0") return 0;
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
      _db->_dev_idx["R"].emplace(_db->_dev_list.size());
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'C'){
      Capacitor* r = new Capacitor(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      r->setValue(to_double(tokens[3]));  
      _db->_dev_idx["C"].emplace(_db->_dev_list.size());    
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'I'){
      Isrc* r = new Isrc(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      if (tokens.size() == 5) {
        r->setType(tokens[3]);
      }
      r->setValue(to_double(tokens.back()));
      _db->_dev_idx["I"].emplace(_db->_dev_list.size());
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
      _db->_dev_idx["V"].emplace(_db->_dev_list.size());
      _db->_dev_list.emplace_back(r);
      (_db->_num_in)++;
    } else if (first_letter == 'L'){
      Inductor* r = new Inductor(tokens[0]);
      r->setPnode(add_node(_db->_node_list, tokens[1]));
      r->setNnode(add_node(_db->_node_list, tokens[2]));
      r->setValue(to_double(tokens[3]));
      _db->_dev_idx["L"].emplace(_db->_dev_list.size());
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'K'){
      Mutual* r = new Mutual(tokens[0]);
      r->setInd1(tokens[1]);
      r->setInd2(tokens[2]);
      r->setValue(to_double(tokens[3]));
      _db->_dev_idx["K"].emplace(_db->_dev_list.size());
      _db->_dev_list.emplace_back(r);
    } else if (first_letter == 'X') {
      Subckt s;
      s.instantiate(whole_line , filename, _db);
    } else if (first_letter == 'G'){
      Vccs* g = new Vccs(tokens[0]);
      g->setPnode(add_node(_db->_node_list, tokens[1]));
      g->setNnode(add_node(_db->_node_list, tokens[2]));
      g->setCtrlPnode(add_node(_db->_node_list, tokens[3]));  
      g->setCtrlNnode(add_node(_db->_node_list, tokens[4]));   
      g->setValue(to_double(tokens[5]));
      _db->_dev_idx["G"].emplace(_db->_dev_list.size());
      _db->_dev_list.push_back(g);
    } else if (first_letter == 'E') {
      Vcvs* e = new Vcvs(tokens[0]);
      e->setPnode(add_node(_db->_node_list, tokens[1]));
      e->setNnode(add_node(_db->_node_list, tokens[2]));
      e->setCtrlPnode(add_node(_db->_node_list, tokens[3]));  
      e->setCtrlNnode(add_node(_db->_node_list, tokens[4]));   
      e->setValue(to_double(tokens[5]));
      _db->_dev_idx["E"].emplace(_db->_dev_list.size());
      _db->_dev_list.push_back(e);
    } else if (first_letter == 'H') {
      Ccvs* h = new Ccvs(tokens[0]);
      h->setPnode(add_node(_db->_node_list, tokens[1]));
      h->setNnode(add_node(_db->_node_list, tokens[2]));
      h->setCtrlName(tokens[3]);
      h->setValue(to_double(tokens[4]));
    } else if (first_letter == 'F') {
      Cccs*  f = new Cccs(tokens[0]);
      f->setPnode(add_node(_db->_node_list, tokens[1]));
      f->setNnode(add_node(_db->_node_list, tokens[2]));
      f->setCtrlName(tokens[3]);
      f->setValue(to_double(tokens[4]));      
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


  for (int i = 0; i < _db->_dev_list.size(); i++) {
    _db->_dev_name2dev_idx[_db->_dev_list[i]->getName()] = i;
  }
  // post process
  _db->_num_node = _db->_num_act_node = _db->_node_list.size();
  for (auto id : _db->_dev_idx["L"]) {
    Inductor* l = dynamic_cast<Inductor*>(_db->_dev_list[id]);
    std::string tmp = "I<" + l->getName() + ">";
    add_node(_db->_aux_node_list["L"], tmp);
    _db->_aux_node_list["L"][tmp] += _db->_num_node;
    l->setAuxName(tmp);
    l->setAux(_db->_aux_node_list["L"][tmp]);
  }
  _db->_num_node += _db->_aux_node_list["L"].size();
  int src_id = 1;
  for (auto id : _db->_dev_idx["I"]) {
    Isrc* i = dynamic_cast<Isrc*>(_db->_dev_list[id]);
    i->setSrcIdx(src_id++);
  }
  for (auto id : _db->_dev_idx["V"]) {
    Vsrc* v = dynamic_cast<Vsrc*>(_db->_dev_list[id]);
    std::string tmp = "I<" + v->getName() + ">";
    add_node(_db->_aux_node_list["V"], tmp);
    _db->_aux_node_list["V"][tmp] += _db->_num_node;
    v->setAuxName(tmp);
    v->setAux(_db->_aux_node_list["V"][tmp]);
    v->setSrcIdx(src_id++);
  }
  _db->_num_node += _db->_aux_node_list["V"].size();
  for (auto id : _db->_dev_idx["E"]) {
    Vcvs* e = dynamic_cast<Vcvs*>(_db->_dev_list[id]);
    std::string tmp = "I<" + e->getName() + ">";
    add_node(_db->_aux_node_list["E"], tmp);
    _db->_aux_node_list["E"][tmp] += _db->_num_node;
    e->setAux(_db->_aux_node_list["E"][tmp]);
  }
  _db->_num_node += _db->_aux_node_list["E"].size();

  // get device by name
  for (auto id : _db->_dev_idx["K"]) {
    Mutual* m = dynamic_cast<Mutual*>(_db->_dev_list[id]);
    Inductor* l1 = dynamic_cast<Inductor*>(_db->_dev_list[_db->_dev_name2dev_idx[m->getInd1()]]);
    m->setAuxPos(l1->getAux());
    Inductor* l2 = dynamic_cast<Inductor*>(_db->_dev_list[_db->_dev_name2dev_idx[m->getInd2()]]);
    m->setAuxNeg(l2->getAux());
    m->setMutValue(m->getValue() * sqrt(l1->getValue() * l2->getValue()));
  }

  for (auto id : _db->_dev_idx["H"]) {
    Ccvs *h = dynamic_cast<Ccvs*>(_db->_dev_list[id]);
    std::string tmp = "I<" + h->getName() + ">";
    add_node(_db->_aux_node_list["H"], tmp);
    _db->_aux_node_list["H"][tmp] = (_db->_num_node)++;
    h->setAuxName(tmp);
    h->setAux(_db->_aux_node_list["H"][tmp]);

    int cid = _db->_dev_name2dev_idx[h->getCtrlName()];
    Device* d = _db->_dev_list[cid];
    h->setCtrlValue(d->getValue());
    if (d->existAux() == false) {
      h->setExistCtrlAux(false);

      tmp = "I<" + d->getName() + ">";
      std::string type(1,d->getName()[0]);
      add_node(_db->_aux_node_list[type], tmp);
      _db->_aux_node_list[type][tmp] = (_db->_num_node)++;
      d->setAuxName(tmp);
      d->setAux(_db->_aux_node_list[type][tmp]);
    } else {
      h->setExistCtrlAux(true);
    }
    h->setCtrlAux(d->getAux());
  }
  
  for (auto id : _db->_dev_idx["F"]) {
    Cccs *f = dynamic_cast<Cccs*>(_db->_dev_list[id]);

    int cid = _db->_dev_name2dev_idx[f->getCtrlName()];
    Device* d = _db->_dev_list[cid];
    f->setCtrlValue(d->getValue());
    if (d->existAux() == false) {
      f->setExistCtrlAux(false);
      std::string tmp = "I<" + d->getName() + ">";
      std::string type(1,d->getName()[0]);
      add_node(_db->_aux_node_list[type], tmp);
      _db->_aux_node_list[type][tmp] = (_db->_num_node)++;
      d->setAuxName(tmp);
      d->setAux(_db->_aux_node_list[type][tmp]);
    } else {
      f->setExistCtrlAux(true);
    }
    f->setCtrlAux(d->getAux());
  }
    
}

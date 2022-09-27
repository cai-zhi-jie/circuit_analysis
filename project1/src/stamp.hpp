/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 10:48:08 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-27 16:37:47
 */
#pragma once

#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "cap.hpp"
#include "device.hpp"
#include "ind.hpp"
#include "isrc.hpp"
#include "mat.hpp"
#include "mut.hpp"
#include "res.hpp"
#include "util.hpp"
#include "vsrc.hpp"

/// \class Stamp 
///
/// \brief Stamp engine
/// 
/// The class Stamp is actually the syamp engine. It contains necessary data
/// structures for completing the work. 
class Stamp{
public:
  Stamp() {}
  ~Stamp() {}

  void parse(char* filename);
  void output(char* filename);
  void setup();
 
private:
  std::vector<Device*> _dev_list;
  std::map<std::string, int> _node_list;
 
  /// Probe list is built up from the .PROBE card in the SPICE netlist.
  /// It specifies which nodes needs to be examined. And system matrix
  /// \f$ L^T \f$ is built from it. 
  std::vector<int> _probe_list;
 
  /// Matrix C holds the contribution from capacitors and inductors.
  Matrix _C;

  /// Matrix G holds conductance.
  Matrix _G;

  /// Matrix B is the input incident matrix
  Matrix _B;

  /// Matrix L^T is the output incident matrix
  Matrix _LT;

  int _num_in;
  int _num_out;
};


/// \brief Initialize the system matrix and carry out the stamping
///
/// This function is most important one because it setups up the liear system
/// \f[
/// \left\{\begin{array}{ccc}
///   C\dot{x}+Gx &=& Bu \\
///   y &=& L^T x \end{array}\right.
/// \f]
/// and fills in the elements in the system matrices. 
/// 
/// The matrix class \e Matrix needs to be designed by you.
/// Sparse matrix structure is preferred than the dense one since
/// the matrix will be very large while most of its elemets would be zero. 
/// 
/// In the skeleton program, the devices are enumerated and
/// they contributes to the system matrices by calling thier own stamping function.
/// 
/// \todo 
/// = Some devices such as mutual inductance depends on other devices. You need to 
///  take care of this situation. 
/// - We also have to enumerate the probe list to build the
/// \f$ L^T \f$ matrix. 
/// - SPICE file support multi-line command with a \e '+' at the beginning of the line.
/// Please consider how to support this. 
/// 
void Stamp::setup(){
  int n = static_cast<int>(_node_list.size()) - 1;
  _C = Matrix(n,n);
  _G = Matrix(n,n);
  _B = Matrix(n, _num_in);
  _LT = Matrix(_num_out, n);
  
  for(size_t i=0; i < _dev_list.size(); ++i){
    _dev_list[i]->stamp(_C,_G,_B);
  }
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


/// \brief Parsing the SPICE netlist
///
/// @param filename SPICE filename
///
/// This is the first step of STAMP. It reads in the SPICE file and
/// extracts device and node information from it. The devices and nodes
/// are stored in \e _dev_list and \e _node_list for later usage. 
/// The parser reads in the SPICE netlist line by line. It breaks each line into
/// string tokens according to the delimiters, and then processes each token case by case.
/// Notice the using of \e tokenizer and \e captilizer. These two functions break the
/// line and convert them to upper case. (SPICE file are not case-sensitive)
///
/// \todo Currently, only limited devices cards for SPICE are considered. You can 
/// Further develop this fucntion to  accomodate more SPICE elements or cards such as 
/// subcircuit or controlled sources. 
void Stamp::parse(char* filename)
{
  std::ifstream ifid(filename);
  _num_in = 0;
  _num_out = 0;
	
  std::string line;
  std::string delims(" \n\r()\t");
  int k = 0;
  _node_list["0"] = 0;

  while(std::getline(ifid, line)) {
    std::string tmp;
    capitalize(line);
    // skip comment
    if (line[0] == '*') continue;
    if (line[0] == 'R'){
      // name
      tmp = tokenizer(line, delims);
      Resistor* r = new Resistor(tmp);

      // pnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setPnode(_node_list[tmp]);

      // nnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setNnode(_node_list[tmp]);

      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
    } else if (line[0] == 'C'){
      // name
      tmp = tokenizer(line, delims);
      Capacitor* r = new Capacitor(tmp);

      // pnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setPnode(_node_list[tmp]);

      // nnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setNnode(_node_list[tmp]);

      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
    } else if (line[0] == 'I'){
      // name
      tmp = tokenizer(line, delims);
      Isrc* r = new Isrc(tmp);

      // pnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setPnode(_node_list[tmp]);

      // nnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setNnode(_node_list[tmp]);

      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
      ++_num_in;
    } else if (line[0] == 'V'){
      // name
      tmp = tokenizer(line, delims);
      Vsrc* r = new Vsrc(tmp);
          
      // aux current
      tmp = "i:"+tmp;
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setAux(_node_list[tmp]);

      // pnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setPnode(_node_list[tmp]);

      // nnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setNnode(_node_list[tmp]);
          
      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
      ++_num_out;
    } else if (line[0] == 'L'){
      // name
      tmp = tokenizer(line, delims);
      Inductor* r = new Inductor(tmp);
          
      // aux current
      tmp = "i:"+tmp;
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setAux(_node_list[tmp]);

      // pnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setPnode(_node_list[tmp]);

      // nnode
      tmp = tokenizer(line, delims);
      if(_node_list.find(tmp) == _node_list.end())
        {
          _node_list[tmp] = ++k;
        }
      r->setNnode(_node_list[tmp]);
          
      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
    } else if (line[0] == 'K'){
      // name
      tmp = tokenizer(line, delims);
      Mutual* r = new Mutual(tmp);
          
      // pnode
      tmp = tokenizer(line, delims);
      r->setInd1(tmp);

      // nnode
      tmp = tokenizer(line, delims);
      r->setInd2(tmp);
          
      // value
      tmp = tokenizer(line, delims);
      r->setValue(to_double(tmp));

      // add to device list
      _dev_list.push_back(r);
    } else {
      std::string token = tokenizer(line, delims);
      // netlist file ends
      if (token == ".END") {
        return;
      }
      // specify output node
      else if (token == ".PROBE" /*|| token == ".PRINT"*/) {
        token = tokenizer(line, delims);
        while(!token.empty()){
          if(token == "V")
            {
        token = tokenizer(line, delims);
        continue;
            }
          _probe_list.push_back(_node_list[token]);
          ++_num_out;
          token = tokenizer(line, delims);
        }
      }
    }
  } // end while
  ifid.close();
}

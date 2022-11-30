/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 13:08:08
 * @Description: 
 */

#pragma once

#include "db/device.hpp"
class Mutual : public Device
{
public:
  Mutual(const std::string &name) : Device(name) {}
  virtual ~Mutual() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

  int getAuxInd1() const { return _aux_node_ind1; }
  void setAuxInd1(int s) { _aux_node_ind1 = s; }
  int getAuxInd2() const { return _aux_node_ind2; }
  void setAuxInd2(int s) { _aux_node_ind2 = s; }
  std::string getInd1() const { return _ind1; }
  void setInd1(const std::string &l) { _ind1 = l; }
  std::string getInd2() const { return _ind2; }
  void setInd2(const std::string &l) { _ind2 = l; }
  double getMutValue() const { return _mut_value; }
  void setMutValue(double m) { _mut_value = m; }

private:
  double _mut_value;
  int _aux_node_ind1;
  int _aux_node_ind2;
  std::string _ind1;
  std::string _ind2;
  
};

void Mutual::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  C.add(_aux_node_ind1, _aux_node_ind2, -_mut_value);
  C.add(_aux_node_ind2, _aux_node_ind1, -_mut_value);
  // std::cout << _name << ' ' << _ind1 << ' ' << _ind2 << ' ' << _aux_node_ind1 << " " << _aux_node_ind2 << " " << _mut_value << " " << _value << std::endl;
}

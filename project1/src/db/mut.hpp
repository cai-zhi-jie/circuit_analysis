/*
 * @Author: Zhijie Cai
 * @Date: 2022-09-28 13:08:08
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 13:13:46
 */

#pragma once

#include "db/device.hpp"

/// \class Mutual
///
/// \brief The mutual inductance between two inductors
///
/// An Mutual instance corrsponds to the "k1 l1 l2 0.5" card in the SPICE netlist
/// It records the information of the mutual inductance. Differnt from other devices
/// in the SPICE netlist, the mutual inductance doesn't have the concept of pins.
/// Instead, it records the name of two inductors which will also appear in the
/// the netlist. However, it is not sure that the two inductors will appear before or
/// after the mutual indectance card. so we have to record the name of these two inductors.
/// Additionally, we also have to record currents through the inductors as auxiliary vairiables.
class Mutual : public Device
{
public:
  Mutual(const std::string &name) : Device(name) {}
  virtual ~Mutual() {}

  virtual void stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U);

  int getAuxPosNode() const { return _aux_node_pos; }
  void setAuxPos(int s) { _aux_node_pos = s; }
  int getAuxNegNode() const { return _aux_node_neg; }
  void setAuxNeg(int s) { _aux_node_neg = s; }
  std::string getInd1() const { return _ind1; }
  void setInd1(const std::string &l) { _ind1 = l; }
  std::string getInd2() const { return _ind2; }
  void setInd2(const std::string &l) { _ind2 = l; }
  double getMutValue() const { return _mut_value; }
  void setMutValue(double m) { _mut_value = m; }

private:
  double _mut_value;
  int _aux_node_pos;
  int _aux_node_neg;
  std::string _ind1;
  std::string _ind2;
  
};

void Mutual::stamp(Matrix &C, Matrix &G, Matrix &B, Matrix& I, Mat<std::string>& U)
{
  
  C.add(_aux_node_pos, _aux_node_neg, -_mut_value);
  C.add(_aux_node_neg, _aux_node_pos, -_mut_value);
  std::cout << _name << ' ' << _ind1 << ' ' << _ind2 << ' ' << _aux_node_pos << " " << _aux_node_neg << " " << _mut_value << " " << _value << std::endl;
}

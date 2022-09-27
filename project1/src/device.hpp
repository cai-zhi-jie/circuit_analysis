/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 10:43:39 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-27 14:15:58
 */
#pragma once

#include <iostream>
#include <string>

#include "mat.hpp"

/// \class Device 
///
/// \brief The base class for various devices
///
/// This is an abstract base class for all the devices in the circuit. For a device
/// it must have a name, have some pins and some values. Most important of all, it must
/// have a \e stamp function which shows its contribution to the system matrix. 
/// Different devices such as resistors and capacitors have different stamping functions.
/// So they will all overwrites the \e stamp pure virtual function here. 
///
/// If you want to add some new devices to this engine, please inherit from Device. And
/// make sure to implement the \e stamp function. 
class Device {
public:
  Device(const std::string & name): _name(name),_pnode(0),_nnode(0),_value(0) 
  {}

  /// \brief pure virtual fucntion for stamping
  ///
  /// @param C system matrix \f$ C \f$
  /// @param G system matrix \f$ G \f$
  /// @param B system matrix \f$ B \f$
  ///
  virtual void stamp(Matrix& C, Matrix& G, Matrix& B) = 0;

  /// \brief destructor
  virtual ~Device() {}

  /// \brief Set the positive node number
  ///
  /// @param p node number
  ///
  void setPnode(int p) {_pnode = p;}

  /// \brief Get the positive node number
  ///
  ///
  /// @return positive node number of this device
  ///
  int pnode() const {return _pnode;}

  /// \brief Set the negative node number
  ///
  /// @param p node number
  ///
  void setNnode(int p) {_nnode = p;}

  /// \brief Get the negative node number
  ///
  ///
  /// @return negative node number of this device
  ///
  int nnode() const {return _nnode;}  

  /// \brief Set the value of this device
  ///
  /// @param d value
  ///
  /// The value could be capacitance, conductance or mutual inductance coefficient 
  /// and etc. Store what you want. 
  void setValue(double d) {_value = d;}

  /// \brief Get the value of this device
  ///
  ///
  /// @return value
  ///
  double value() const {return _value;}

  /// \brief Set the name of this device
  ///
  /// @param n device name
  ///
  void setName(const std::string& n) {_name = n;}

  /// \brief Get the name of this device
  ///
  ///
  /// @return name of this device
  ///
  std::string name() const {return _name;}

protected:
  /// \brief device name
  std::string _name;

  /// \brief positive node number
  int _pnode;

  /// \brief negative node number
  int _nnode;

  /// \brief value
  double _value;
};



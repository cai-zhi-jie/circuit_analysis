///
/// @file   res.h
/// @author Yinghai
/// @date   Fri Sep 12 14:17:02 2008
/// 
/// @brief  header of the Resistor class
/// 
/// 
///
#pragma once

#include "device.hpp"

/// \class Resistor
///
/// \brief The resistor
///
/// A Resisitor instance corrsponds to the "r1 n1 n2 3k" card in the SPICE netlist
/// It records the information of the resisitor. 
class Resistor: public Device{
public:
  /// \brief Constructor
  /// \param name name of the resistor
  Resistor(const std::string& name):Device(name) {}

  /// \brief stamping function of the resistor
  ///
  /// @param C system matrix \f$ C \f$
  /// @param G system matrix \f$ G \f$
  /// @param B system matrix \f$ B \f$
  ///
  /// \todo You have to fills in each stamp function. 
  /// 
  virtual void stamp(Matrix& C, Matrix& G, Matrix& B);

  /// \brief Destructor
  virtual ~Resistor() {}
};

void Resistor::stamp(Matrix& C, Matrix& G, Matrix& B)
{
  // to be implemented
  std::cout<<_name<<' '<<_pnode<<' '<<_nnode<<' '<<_value<<std::endl;
}
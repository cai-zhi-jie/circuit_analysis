///
/// @file   isrc.h
/// @author Yinghai
/// @date   Fri Sep 12 14:35:29 2008
/// 
/// @brief  header of the Current source class
/// 
/// 
///
#pragma once

#include "device.hpp"

/// \class Isrc
///
/// \brief The current source
///
/// An Isrc instance corrsponds to the "i1 n1 n2 1" card in the SPICE netlist
/// It records the information of the current source. 
class Isrc: public Device{
public:
  /// \brief Constructor
  /// \param name name of the current source
  Isrc(const std::string& name):Device(name) {}

  /// \brief stamping function of the current source
  ///
  /// @param C system matrix \f$ C \f$
  /// @param G system matrix \f$ G \f$
  /// @param B system matrix \f$ B \f$
  ///
  /// \todo You have to fills in each stamp function. 
  ///
  virtual void stamp(Matrix& C, Matrix& G, Matrix& B);

  /// \brief Destructor
  virtual ~Isrc() {}
};

void Isrc::stamp(Matrix& C, Matrix& G, Matrix& B)
{
  // to be implemented
  std::cout<<_name<<' '<<_pnode<<' '<<_nnode<<' '<<_value<<std::endl;
}

/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 14:09:23 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-27 14:10:28
 */
#pragma once

#include "device.hpp"


/// \class Capacitor
///
/// \brief The capacitor
///
/// A Capacitor instance corrsponds to the "c1 n1 n2 3p" card in the SPICE netlist
/// It records the information of the capacitor. 
class Capacitor: public Device{
public:
  /// \brief Constructor
  /// \param name name of the capacitor
  Capacitor(const std::string& name):Device(name) {}
  
  /// \brief stamping function of the capacitor
  ///
  /// @param C system matrix \f$ C \f$
  /// @param G system matrix \f$ G \f$
  /// @param B system matrix \f$ B \f$
  ///
  /// \todo You have to fills in each stamp function. 
  /// 
  virtual void stamp(Matrix& C, Matrix& G, Matrix& B);

  /// \brief Destructor
  virtual ~Capacitor() {}
};

void Capacitor::stamp(Matrix& C, Matrix& G, Matrix& B)
{
  // to be implemented
  std::cout<<_name<<' '<<_pnode<<' '<<_nnode<<' '<<_value<<std::endl;
}



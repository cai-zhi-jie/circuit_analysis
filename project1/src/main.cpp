/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 11:46:34 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-28 14:57:08
 */

#include <iostream>

#include "stamp.hpp"

int main(int argc, char* argv[]){
	
  if(argc < 3){
    std::cout<<"usage: stamp input_file output_file"<<std::endl;
    return 1;
  }

  Stamp s;
  
  /// phase 1: parsing the netlist
  s.parse(argv[1]);

  /// phase 2: stamping
  s.setup();
  
  /// phase 3: output
  s.output(argv[2]);
	
  return 0;
}

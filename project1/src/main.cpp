/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 11:46:34 
 * @Description: 
 */

#include <iostream>

#include "stamp.hpp"

int main(int argc, char* argv[]){
	
  if(argc < 3){
    std::cout<<"usage: stamp input_file output_file"<<std::endl;
    return 1;
  }
  bool sparse_output = false;
  for(int i = 3; i < argc; i++){
    auto option = static_cast<std::string>(argv[i]);
    if(option == "-sparse_output") sparse_output = true;
  }
  Stamp s;
  
  /// phase 1: parsing the netlist
  s.parse(argv[1]);

  /// phase 2: stamping
  s.setup();
  
  /// phase 3: output
  s.output(argv[2], sparse_output);
	
  return 0;
}

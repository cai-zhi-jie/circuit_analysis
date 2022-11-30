/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 11:46:34 
 * @Description: 
 */

#include <iostream>

#include "stamp.hpp"
#include "utils/log.hpp"

int main(int argc, char* argv[]){
	
  if(argc < 2){
    std::cout<<"usage: stamp input_file"<<std::endl;
    return 1;
  }
  std::string input_fn(argv[1]);
  auto idx_begin = input_fn.find_last_of("/") + 1;
  auto idx_end = input_fn.find_last_of(".");
  std::string design_name = input_fn.substr(idx_begin, idx_end - idx_begin);
  // std::cout << design_name << std::endl;
  bool sparse_output = true;
  // for(int i = 2; i < argc; i++){
  //   auto option = static_cast<std::string>(argv[i]);
  //   if(option == "-sparse_output") sparse_output = true;
  // }
  Stamp s;
  
  /// phase 1: parsing the netlist
  info::phase("Parse "+ input_fn, [&]() { s.parse(input_fn); });

  /// phase 2: stamping
  info::phase("Setup database", [&]() { s.setup(); });
  
  /// phase 3: output
  info::phase("Output matrix", [&]() { s.output(design_name, sparse_output); });
	
  return 0;
}

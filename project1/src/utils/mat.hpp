/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 11:07:09 
 * @Last Modified by: Zhijie Cai
 * @Last Modified time: 2022-09-27 16:12:35
 */
#pragma once

#include <cstring>
#include <map>

/// - Dynamic allocation of the memory according to the size of the circuit.
/// - Expandable. Each time we stamp an element, the entries of the matrix may grow.
/// - Flexible and efficient. Most of the entries in the matrix would be zero and we don't need to store them. 
///
/// My advice is: Using orthogonal list data structure. \n
template<class T>
class Mat{
public:
  Mat(int nrow = 0, int ncol = 0) :_row_cnt(nrow),_col_cnt(ncol) {}
  ~Mat() {}
  
  T insert(int r, int c, T v);

  void set_row_num(int rn) { _row_cnt = rn;}
  int get_row_num() const {return _row_cnt;}
  void set_col_num(int cn) { _col_cnt = cn;}
  int get_col_cnt() const {return _col_cnt;}
private:
  
  int _row_cnt;
  int _col_cnt;
  std::map<int, std::map<int, T>> _matrix;
};

template<class T>
T Mat<T>::insert(int r, int c, T v) {
  if (r < 0 || r >= _row_cnt || c < 0 || c >= _col_cnt) {
    std::cout << "ERROR: Matrix Insertion Index Out Of Range!" << std::endl;
    return T(-1);
  }
  _matrix[r][c] += v;
  return _matrix[r][c];
}

typedef Mat<double> Matrix;


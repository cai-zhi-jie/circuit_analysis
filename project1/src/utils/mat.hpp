/*
 * @Author: Zhijie Cai 
 * @Date: 2022-09-27 11:07:09 
 * @Description: 
 */
#pragma once

#include <fstream>
#include <iomanip>
#include <map>
#include <string>
template<class T>
class Mat{
public:
  Mat(int nrow = 0, int ncol = 0,std::string name = "") 
      :_num_row(nrow),_num_col(ncol), _name(name) {}
  ~Mat() {}
  
  T add(int r, int c, T v);
  T get(int r, int c) { return _matrix[r][c]; }

  void set_row_num(int rn) { _num_row = rn;}
  int get_row_num() const {return _num_row;}
  void set_col_num(int cn) { _num_col = cn;}
  int get_col_num() const {return _num_col;}

  void output(std::string fn, int width = 8, int prec = 3);
  void sparse_output(std::string fn);
private:
  
  int _num_row;
  int _num_col;
  std::string _name;
  std::map<int, std::map<int, T>> _matrix;
};

template<class T>
T Mat<T>::add(int r, int c, T v) {
  if (r <= 0 || r > _num_row || c <= 0 || c > _num_col) {
    //std::cout << "ERROR: Matrix Insertion Index Out Of Range!" << std::endl;
    return T();
  }
  _matrix[r][c] += v;
  return _matrix[r][c];
}

template<class T>
void Mat<T>::output(std::string fn, int width, int prec) {
  std::ofstream fout(fn, std::ios::app);
  fout << "** Matrix " << _name << " (Size: " << _num_row << " x"  << _num_col << " ) **" << std::endl;
  auto print_zero = [&](int count, bool newline) {
    for (int j = 0; j < count; j++) {
      //fout << std::setw(width) << 0;
      fout << std::setw(width) << std::setprecision(prec) << 0.0 << "\t";
    } 
    if (newline) fout << std::endl;
  };
  int pr = 1;
  for (auto& rp : _matrix) {
    int rid = rp.first;
    auto& row = rp.second;
    // fill the blank row in the front
    for (int i = 0; i < rid - pr; i++) {
      print_zero(_num_col, true);
    }
    pr = rid + 1;
    int pc = 1;
    for (auto& e : row) {
      int cid = e.first;
      // fill the blank element
      print_zero(cid - pc ,false);
      fout << std::setw(width) << std::setprecision(prec) << e.second << "\t";
      pc = cid + 1;
    }
    print_zero(_num_col - pc + 1 ,true);
  }
  // fill the blank row in the back
  for (int i = pr; i <= _num_row; i++) {
    print_zero(_num_col, true);
  }
  fout.close();
}


template<class T>
void Mat<T>::sparse_output(std::string fn) {
  std::ofstream fout(fn, std::ios::app);
  fout << "** Matrix " << _name << " (Size: " << _num_row << " x"  << _num_col << " ) **" << std::endl;

  for (auto& rp : _matrix) {
    int rid = rp.first;
    auto& row = rp.second;
    for (auto& e : row) {
      int cid = e.first;
      fout << "(" << rid << ", " << cid << ") " << e.second << std::endl;
    }
  }
  fout.close();
}


typedef Mat<double> Matrix;


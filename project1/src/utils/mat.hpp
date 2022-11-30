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
  
  void clear();
  T add(int r, int c, T v);
  T get(int r, int c) { return _matrix[r][c]; }

  void setRowNum(int rn) { _num_row = std::max(0, rn);}
  int getRowNum() const {return _num_row;}
  void setColNum(int cn) { _num_col = std::max(0, cn);}
  int getColNum() const {return _num_col;}
  void setName(std::string n) { _name = n;}
  std::string getName() const {return _name;}

  void getSubMatrix(int start_row, int star_col, int row_num, int col_num, Mat<T>& sub_mat);

  void output(std::string fn, int width = 8, int prec = 3);
  void sparse_output(std::string fn);
  void binary_output(std::string fn);
private:
  
  int _num_row;
  int _num_col;
  std::string _name;
  std::map<int, std::map<int, T>> _matrix;
};

template<class T>
void Mat<T>::clear() {
  int _num_row = 0;
  int _num_col = 0;
  _matrix.clear();  
}

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

template<class T>
void Mat<T>::binary_output(std::string fn)
{
  std::ofstream fout(fn, std::ios::binary | std::ios::app);

  int non_zero_num = 0;
  for (auto r : _matrix) {
    non_zero_num += r.second.size();
  }
  // std::cout << _num_row << " " << _num_col  << " " << non_zero_num << std::endl;
  fout.write((char*)&_num_row, sizeof(_num_row));
  fout.write((char*)&_num_col, sizeof(_num_col));
  fout.write((char*)&non_zero_num, sizeof(non_zero_num));

  for (auto r : _matrix)  {
    int rid = r.first;
    for(auto e : r.second) {
      int cid = e.first;
      auto value = e.second;
      fout.write((char*)&rid, sizeof(rid));
      fout.write((char*)&cid, sizeof(cid));
      fout.write((char*)&value, sizeof(value));
      // std::cout << rid  << "," << cid << "=" << value << std::endl;
    }
  }


  fout.close();  
}



template<class T>
void Mat<T>::getSubMatrix(int start_row, int start_col, int row_num, int col_num, Mat<T>& sub_mat) {
  sub_mat.clear();
  sub_mat.setRowNum(row_num);
  sub_mat.setColNum(col_num);
  int end_row = std::min(start_row + row_num, _num_row + 1);
  int end_col = std::min(start_col + col_num, _num_col + 1);
  auto in_range = [&](int r, int s, int e)->bool{
    if (r >= s && r < e) {
      return true;
    } else {
      return false;
    }
  };
  for (auto r : _matrix) {
    int rid = r.first;
    if (!in_range(rid, start_row, end_row)) continue;
    for (auto c : r.second) {
      int cid = c.first;
      if (!in_range(cid, start_col, end_col)) continue;
      auto value = c.second;
      sub_mat.add(rid - start_row + 1, cid - start_col + 1, value);
    }
  }
}

typedef Mat<double> Matrix;


#include "mex.h"
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <math.h>
#include <vector>

using namespace std;

double process_unit(const string& value)
{
  char scale = '-';
  double scaled_value;
  istringstream iss(value);

  if(! (iss >> scaled_value))
    {
      mexErrMsgTxt("Error: it should be a number here.");
      mexErrMsgTxt( value.c_str() );
    }

  iss >> scale;
  
  switch(scale)
    {
    case 'a':
      scaled_value *= 1e-18;
      break;
      
    case 'f':
      scaled_value *= 1e-15;
      break;
      
    case 'p':
      scaled_value *= 1e-12;
      break;
    case 'n':
      scaled_value *= 1e-9;
      break;
    case 'u':
      scaled_value *= 1e-6;
      break;
    case 'm':
      scaled_value *= 1e-3;
      break;
    case 'k':
      scaled_value *= 1e3;
      break;
    case 'x':
      scaled_value *= 1e6;
      break;
    case 'g':
      scaled_value *= 1e9;
      break;
    case '-':
      scaled_value *= 1.0;
      break;
    default:
      scaled_value *= 1;
      break;
      
    }
  return scaled_value;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    char *input_buf;
    int buflen;
    list<double> t_vec;
    vector<list<double> > val_vec(256);


    // check input/output numbers
    if(nrhs != 1)
        mexErrMsgTxt("Please input the name of the data file");
    else if(nlhs != 1)
        mexErrMsgTxt("Only one output is allowed");
    
    // Make sure input is a string
    if(mxIsChar(prhs[0]) != 1)
        mexErrMsgTxt("Input should be a string.");
    
    if(mxGetM(prhs[0]) != 1)
        mexErrMsgTxt("Output must be a matrix");
    
    // Get the length of the input string
    buflen = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;
    
    input_buf = (char *) mxCalloc(buflen, sizeof(char));
    
    mxGetString(prhs[0], input_buf, buflen);
      
    // Ok, now start parsing the .lis file
    
    string line;
    istringstream iss;
    ifstream inf(input_buf);
    int cnt_tmp=0;
    string s1, s2;
    double t1, t2;
    int read_flag = 0;
    int t_flag = 0;
    int x_cnt = 0;
    int i;
    int j;

    
    if(!inf)
    {
        mexErrMsgTxt(".lis file doesn't exist");
    }
    
   
    while(!inf.eof())
    {
        iss.clear();
        getline(inf, line);
        iss.str(line);
        
        if(line == "x")
        {
            cnt_tmp = 0;
            read_flag = 1;
        }
        
        if(line == "y")
        {
	    t_flag = 1;
	    read_flag = 0;
	    x_cnt += j;
        }        
        
        if(read_flag && cnt_tmp >= 4)
        {
	  iss >> s1;
	  if(!t_flag)
	    {
	      t1 = process_unit(s1);
	      t_vec.push_back(t1);
	    }

	  j = 0;
	  while(iss >> s2)
	    {
	      t2 = process_unit(s2);
	      val_vec[x_cnt+j].push_back(t2);
	      ++j;
	    }
        }
         
        ++cnt_tmp;             
    }
    
    int lt = t_vec.size();
    int nn = x_cnt;
    plhs[0] = mxCreateDoubleMatrix(lt, nn+1, mxREAL);
    double * data_ptr = mxGetPr(plhs[0]);
    
    list<double>::iterator iter1, iter2;
    
    for(i = 0, iter1 = t_vec.begin(); i < lt; ++i, ++iter1)
    {
        data_ptr[i] = *iter1;
    }

    for(j = 0; j < nn; ++j)
      {
	for(i = 0, iter2 = val_vec[j].begin(); i < lt; ++i, ++iter2)
	  data_ptr[lt*(j+1)+i] = *iter2;
      }

    inf.close();  
}


///
/// @file   util.h
/// @author Yinghai
/// @date   Fri Sep 12 13:55:42 2008
/// 
/// @brief  Header of the utility funtions
/// 
/// 
///
#pragma once

#include <string>
#include <cctype>
#include <cstdlib>

/// \brief this function converts string to upper case
///
/// @param token string to be converted
///
void capitalize(std::string &token) {
  for (size_t i = 0; i< token.length(); ++i) {
    token[i] = toupper(token[i]);
  }
}

/// \brief This function breaks the string into tokens with the given delimiters.
/// 
/// \param str the long string to be break
/// \param de delimiters
/// @return first token extracted
///
/// This function is a convenient C++ verion of \e strtok. Given the delimiters,
/// it will extract the first token from the string. 
/// For example, \e tokenizer("Hello+$World","+$") will return the string \e Hello.
/// You have run it several times in order to extract more tokens. If no more token
/// can be extracted, it will return an empty string. Using this, you can decide whether
/// a line is finished. 
/// In boost library, there is a more powerful tokening tool. If you'rer interested,
/// please see to the doc of boost.
std::string tokenizer(std::string& line, const std::string& delims){
	std::string toke;

	// find the beginning position of first token
	std::string::size_type idx_begin = line.find_first_not_of(delims);

	if (idx_begin != std::string::npos) {
		std::string::size_type idx_end = line.find_first_of(delims, idx_begin);

		// last word
		if (idx_end == std::string::npos) {
			idx_end = line.length();
		}

		// extract the first token and erase it from the input string
		toke = line.substr(idx_begin, idx_end - idx_begin);
		capitalize(toke);
		line.erase(0, idx_end - 0);
	} // end if

	return toke;
}

/// \brief This function converts string to a double considering the units
///
/// In SPICE, units are used. So \e 9ns should equal to \e 1e-9. This function
/// just help you take care of such things. 
double to_double(std::string str) {
	std::string delims = "0123456789+-E.";
	std::string::size_type fig_end = str.find_first_not_of(delims);
	if (fig_end == std::string::npos) {
		return atof(str.c_str());	
	}
	else {
		double fig = atof(str.substr(0, fig_end).c_str());
		double scale = 1.0;
		std::string unit = str.substr(fig_end, str.length() - fig_end);
		if (unit == "F") {
      scale = 1e-15;
    } else if (unit == "P") {
			scale = 1e-12;	
		} else if (unit == "N" || unit == "NS") {
			scale = 1e-9;	
		}	else if (unit == "U") {
			scale = 1e-6;	
		}	else if (unit == "M") {
			scale = 1e-3;	
		}	else if (unit == "K") {
			scale = 1e3;
		}	else if (unit == "MEG"){
			scale = 1e6;     
		} else if (unit == "G") {
      scale = 1e9;
    } else if (unit == "T") {
      scale = 1e12;
    } else {}
		return fig*scale;
	}	
}



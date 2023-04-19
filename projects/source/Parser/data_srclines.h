#pragma once

#include <string>
#include <vector>

namespace Parser {


	typedef struct {
		std::string name;  // name of file (with path)
	} data_srcfiles;
	extern std::vector<data_srcfiles> v_data_srcfiles;


	typedef struct {
		short file_idx;    // index into g_vsrcfiles
		short line_number; // line in file
		std::string line;  // line content
	} data_srclines;
	extern std::vector<data_srclines> v_data_srclines;


} // namespace Parser

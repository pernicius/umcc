#pragma once

#include <string>
#include <vector>

namespace Parser {


	struct data_srcfiles {
		std::string name;  // name of file (with path)
	};
	extern std::vector<data_srcfiles> g_data_srcfiles;


	struct data_srclines {
		short file_idx;    // index into g_vsrcfiles
		short line_number; // line in file
		std::string line;  // line content
	};
	extern std::vector<data_srclines> g_data_srclines;


} // namespace Parser

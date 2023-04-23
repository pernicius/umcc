#pragma once

#include <string>
#include <vector>

namespace Parser {


	typedef struct {
		std::string name;  // name of file (with path)
	} data_sourcefiles;
	extern std::vector<data_sourcefiles> data_srcfiles;


	typedef struct {
		short file_idx;    // index into g_vsrcfiles
		short line_number; // line in file
		std::string line;  // line content
	} data_sourcelines;
	extern std::vector<data_sourcelines> data_srclines;


} // namespace Parser

#pragma once

#include <string>
#include <vector>


struct SrcFiles {
	std::string name;  // name of file (including relative path to project)
};
extern std::vector<SrcFiles> g_vSrcFiles;


struct SrcLines {
	short file_idx;    // index into g_vsrcfiles
	short line_number; // line in file
	std::string line;  // line content
};
extern std::vector<SrcLines> g_vSrcLines;

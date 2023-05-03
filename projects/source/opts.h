#pragma once


namespace config {
	extern std::string infile;
	extern std::string outfile;
	extern std::string format;
	extern bool debug;
	extern bool silent;
}


bool parseopts(int argc, const char* argv[]);

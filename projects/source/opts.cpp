#include "pch.h"

#include "opts.h"

#include "cxxopts.hpp"
#include "colored_cout.h"


// global configuration
namespace config {
	std::string infile;
	std::string outfile = "rom";
	bool debug = false;
	bool silent = false;
}


bool parseopts(int argc, const char* argv[])
{
	bool show_help = false;

	cxxopts::Options options("umcc", "Universal Microcode Compiler");

	// setup cxxopts
	options.add_options()
		("d,debug",   "Display debug information.")
		("help",      "Display this information.")
		("s,silent",  "Don't print any informational messages.")
//		("size",      "Size of the generated binary rom file in kilobytes.", cxxopts::value<int>())
		("v,version", "Display version information.")
		;
	options.custom_help("[options]");
	options.add_options("files")
		("infile",  "", cxxopts::value<std::string>())
		("outfile", "", cxxopts::value<std::string>()
			->default_value("rom.hex"))
		;
	options.positional_help("infile [outfile]");
	options.parse_positional({ "infile", "outfile" });
	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);

	// [optional] enable debug output
	if (result.count("debug"))
		config::debug = true;

	// [optional] enable silent mode
	if (result.count("silent"))
		config::silent = true;

	// [required] input file
	if (result.count("infile"))
		config::infile = result["infile"].as<std::string>();
	else
		show_help = true;

	// [optional] output file(s)
	if (result.count("outfile"))
		config::outfile = result["outfile"].as<std::string>();

#ifdef _DEBUG
	// only during development!!!
	
	// overwrite cmd options...
	show_help = false;
	config::debug = true;
	if (config::infile.length() == 0)
		config::infile = "../ucode_test/main.ucode";
#endif

	// debug output: config
	if (config::debug)
	{
		for (const auto& option : result.unmatched())
			std::cout << "Unknown option: '" << option << "' " << std::endl;
		std::cout << "config:" << std::endl;
		std::cout << "  silent mode: " << (config::silent ? "yes" : "no") << std::endl;
		std::cout << "  infile:  " << config::infile << std::endl;
		std::cout << "  outfile: " << config::outfile << std::endl;
	}

	// print version information
	if (result.count("version"))
	{
		std::cout << "Universal Microcode Compiler 0.0.1" << std::endl;
		std::cout << "Copyright (C) 2023 Lennart Molnar <" << clr::cyan << "pernicius@web.de" << clr::reset << ">" << std::endl;
		std::cout << "License GPLv3+: GNU GPL version 3 or later <" << clr::cyan << "http://gnu.org/licenses/gpl.html" << clr::reset << ">" << std::endl;
		std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
		std::cout << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
		std::cout << "" << std::endl;

		// force quit
		return false;
	}

	// print help message and quit
	if (result.count("help") or show_help)
	{
		std::cout << options.help({ "" }) << std::endl;
		std::cout << "Report bugs to <" << clr::cyan << "pernicius@web.de" << clr::reset << "> or go to:" << std::endl;
		std::cout << "<" << clr::cyan << "https://github.com/pernicius/uucc" << clr::reset << ">." << std::endl;
		std::cout << "" << std::endl;

		// force quit
		return false;
	}

	return true;
}

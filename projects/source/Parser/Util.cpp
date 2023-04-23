#include "pch.h"

#include "Util.h"

#include <string>

#include "colored_cout.h"
#include "data_source.h"

namespace Parser {


	void PrintLine(token_data& token, std::string msg)
	{
		// TODO: PrintLine()
	}


	bool PrintLineError(token_data& token, const std::string& msg)
	{
		// [ERROR] file:line:pos: msg
		std::cout << clr::red << "[ERROR] " << clr::reset
		          << data_srcfiles[data_srclines[token.line_idx].file_idx].name << ":"
		          << data_srclines[token.line_idx].line_number << ":"
		          << token.pos_start << ": "
		          << clr::red << msg << clr::reset << std::endl;

		// source: line number
		std::string line_num = "  " + std::to_string(data_srclines[token.line_idx].line_number) + " ";
		std::cout << line_num << "| ";
		// source: pre-error
		std::cout << data_srclines[token.line_idx].line.substr(0, token.pos_start);
		// source: error
		std::cout << clr::red << data_srclines[token.line_idx].line.substr(token.pos_start, token.pos_len) << clr::reset;
		// source: post-error
		std::cout << data_srclines[token.line_idx].line.substr(token.pos_start + token.pos_len) << std::endl;

		// marker
		short x;
		for (x = 0; x < line_num.length(); x++)
			std::cout << " ";
		std::cout << "| ";
		for (x = 0; x < token.pos_start; x++)
			std::cout << " ";
		std::cout << clr::red << "^";
		for (x = 0; x < token.pos_len - 1; x++)
			std::cout << "~";
		std::cout << clr::reset << std::endl;

		return false;
	}


	bool PrintLineWarning(token_data& token, const std::string& msg)
	{
		// TODO: PrintLineWarning()
		return false;
	}


} // namespace Parser

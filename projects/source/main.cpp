#include "pch.h"

#include "Parser/Lexer.h"
#include "Parser/Parser.h"

#include <chrono>


int main()
{
	bool result;

	std::string startfile = "..\\ucode_test\\main.ucode";

	// Lexer
	auto lex_start = std::chrono::high_resolution_clock::now();
	Parser::Lexer lex;
	result = lex.GenerateTokens(startfile);
	auto lex_stop = std::chrono::high_resolution_clock::now();
	auto lex_time = std::chrono::duration_cast<std::chrono::microseconds>(lex_stop - lex_start).count();

	// Parser
	auto pars_start = std::chrono::high_resolution_clock::now();
	Parser::Parser pars;
	result = pars.ParseTokens();
	auto pars_stop = std::chrono::high_resolution_clock::now();
	auto pars_time = std::chrono::duration_cast<std::chrono::microseconds>(pars_stop - pars_start).count();

	auto sum_time = std::chrono::duration_cast<std::chrono::microseconds>(pars_stop - lex_start).count();
	std::cout << "Done (" << sum_time << "us)" << std::endl;
	std::cout << " - Lexer:  " << lex_time << "us" << std::endl;
	std::cout << " - Parser: " << pars_time << "us" << std::endl;
	return 0;
}

#include "pch.h"

#include "Parser/Lexer.h"

#include <chrono>


int main()
{
	bool result;

	std::string startfile = "..\\ucode_test\\main.ucode";

	auto lex_start = std::chrono::high_resolution_clock::now();
	Parser::Lexer lex;
	result = lex.GenerateTokens(startfile);
	auto lex_stop = std::chrono::high_resolution_clock::now();
	auto lex_time = std::chrono::duration_cast<std::chrono::microseconds>(lex_stop - lex_start).count();

	std::cout << "Done (";
	std::cout << "Lexer: " << (lex_time/1000) << "." << (lex_time - (lex_time/1000*1000)) << "ms";
	std::cout << ")" << std::endl;
	return 0;
}

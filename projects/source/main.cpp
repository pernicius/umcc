#include "pch.h"

#include "opts.h"
#include "Parser/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/Evaluator.h"

#include <chrono>


int main(int argc, const char* argv[])
{
	// parse command line options
	if (!parseopts(argc, argv))
		return 1;

	// do the parser stuff
	auto time_start = std::chrono::high_resolution_clock::now();
	std::chrono::microseconds lex_time, pars_time, eval_time;
	{
		// Lexer
		auto lex_start = std::chrono::high_resolution_clock::now();
		Parser::Lexer lex;
		if (!lex.GenerateTokens(config::infile))
			return 1;
		auto lex_end = std::chrono::high_resolution_clock::now();
		lex_time = std::chrono::duration_cast<std::chrono::microseconds>(lex_end - lex_start);

		// Parser
		auto pars_start = std::chrono::high_resolution_clock::now();
		Parser::Parser pars;
		if (!pars.ParseTokens())
			return 1;
		auto pars_end = std::chrono::high_resolution_clock::now();
		pars_time = std::chrono::duration_cast<std::chrono::microseconds>(pars_end - pars_start);

		// Evaluator
		auto eval_start = std::chrono::high_resolution_clock::now();
		Parser::Evaluator eval;
		if (!eval.Prepare())
			return 1;
		auto eval_end = std::chrono::high_resolution_clock::now();
		eval_time = std::chrono::duration_cast<std::chrono::microseconds>(eval_end - eval_start);
	}
	auto time_end = std::chrono::high_resolution_clock::now();


	auto time_sum = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
	std::cout << "Done (" << time_sum << "us)" << std::endl;
	std::cout << " - Lexer:     " << lex_time.count()<< "us" << std::endl;
	std::cout << " - Parser:    " << pars_time.count() << "us" << std::endl;
	std::cout << " - Evaluator: " << eval_time.count() << "us" << std::endl;
	return 0;
}

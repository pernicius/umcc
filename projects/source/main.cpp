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
	{
		// Lexer
		Parser::Lexer lex;
		if (!lex.GenerateTokens(config::infile))
			return 1;

		// Parser
		Parser::Parser pars;
		if (!pars.ParseTokens())
			return 1;

		// Evaluator
		Parser::Evaluator eval;
		if (!eval.Prepare())
			return 1;
		if (!eval.Generate())
			return 1;
	}
	auto time_end = std::chrono::high_resolution_clock::now();


	auto time_sum = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
	std::cout << "Done (" << (time_sum / 1000) << "." << (time_sum - (time_sum / 1000) * 1000) << "s)" << std::endl;
	return 0;
}

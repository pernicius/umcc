#include "pch.h"

#include "Parser/Lexer.h"


int main()
{
	bool result;

	std::string startfile = "..\\ucode_test\\main.ucode";

	Parser::Lexer lex;
	result = lex.GenerateTokens(startfile);

	return 0;
}

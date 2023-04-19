#include "pch.h"

#include <iostream>

#include "global_data.h"
std::vector<SrcFiles> g_vSrcFiles;
std::vector<SrcLines> g_vSrcLines;

#include "Parser/Lexer.h"


int main()
{
	std::cout << "main()\n";

	bool result;

	std::string startfile = "..\\ucode_test\\main.ucode";

	Parser::Lexer lex(startfile);
	result = lex.GenerateTokens();

	return 0;
}

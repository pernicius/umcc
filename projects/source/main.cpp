#include "pch.h"

#include "global_data.h"
std::vector<SrcFiles> g_vSrcFiles;
std::vector<SrcLines> g_vSrcLines;

#include "Parser/Lexer.h"


int main()
{
	bool result;

	std::string startfile = "..\\ucode_test\\main.ucode";

	Parser::Lexer lex;
	result = lex.GenerateTokens(startfile);

	return 0;
}

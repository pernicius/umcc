#include "pch.h"

#include <filesystem>
#include <iostream>

#include "global_data.h"
std::vector<SrcFiles> g_vSrcFiles;
std::vector<SrcLines> g_vSrcLines;

#include "Parser/CharReader.h"


int main()
{
	bool result;

	std::cout << "main()\n";
//	std::cout << std::filesystem::current_path().generic_string() << std::endl;

//	std::string startfile = std::filesystem::current_path().generic_string() + "/../test/test.ucode";
	std::string startfile = "..\\test\\test.ucode";

	Parser::CharReader cr(startfile);
	result = cr.Init();
	result = cr.Peek();

	return 0;
}

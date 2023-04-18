#include "pch.h"

#include "FileReader.h"

#include <fstream>
#include <filesystem>

namespace Parser {


	/*

	std::string corrfilename;

	corrfilename = std::filesystem::path(srcfile).remove_filename().generic_string()
				 + std::filesystem::path(srcfile).filename().generic_string();

	// open file
	std::ifstream file(corrfilename);
	if(!file.is_open())
	{
	}

	// load file
	while(file)
	{
		std::getline(file, ld.text);
	}

	*/


	FileReader::FileReader(const std::string& filename)
	{
		std::cout << "Parser::FileReader::FileReader()\n";

		m_filename = filename;

	}


	FileReader::~FileReader()
	{
		std::cout << "Parser::FileReader::~FileReader()\n";
	}


	bool FileReader::Init()
	{
		std::cout << "Parser::FileReader::Init()\n";

		std::string corrfilename = std::filesystem::path(m_filename).remove_filename().generic_string()
		                         + std::filesystem::path(m_filename).filename().generic_string();
		std::cout << "File (internal): \"" << corrfilename << "\"\n";
		std::cout << "File: \"" << m_filename << "\"\n";

		std::ifstream file(corrfilename);
		std::cout << "Exists: " << file.is_open() << std::endl;

		return true;
	}


	bool FileReader::Peek(char* c, short index)
	{
		std::cout << "Parser::FileReader::Peek()\n";
		return false;
	}


	bool FileReader::Consume(char* c, short index)
	{
		std::cout << "Parser::FileReader::Consume()\n";
		return false;
	}


} // namespace Parser

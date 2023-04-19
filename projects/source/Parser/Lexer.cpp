#include "pch.h"

#include "Lexer.h"

#include <filesystem>

namespace Parser {


	Lexer::Lexer()
		: m_reader(nullptr)
	{
//		std::cout << "Parser::Lexer::Lexer()" << std::endl;
	}


	Lexer::~Lexer()
	{
//		std::cout << "Parser::Lexer::~Lexer()" << std::endl;

		if (m_reader)
			delete m_reader;
	}


	bool Lexer::GenerateTokens(const std::string& filename)
    {
//		std::cout << "Parser::Lexer::GenerateTokens()" << std::endl;

		// initialize the file reader
		m_reader = new FileReader(filename);
		if (!m_reader->Init())
			return false;

		// loop until end-of-file
		char c, c2;
		do
		{
			// skip leading spaces
			SkipWhitespaces(true);

			// get the first char of a token
			if (!m_reader->Consume(&c))
				return false;
			// TODO...

			// single-line comments
			if (c == '/')
			{
				if (m_reader->Peek(&c2))
				{
					if (c2 == '/')
					{
						SkipRestofLine();

						// continue parsing
						continue;
					}
				}
			}

			// multi-line comments
			// TODO...

			// PREPROCESSOR COMMAND
			if (c == '#')
			{
				// get the command
				std::string cmd;
				if (!ScanSymbol(cmd))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

				// #include ...
				if (cmd.compare("include") == 0)
				{
//					std::cout << "Lexer -> found #include statement" << std::endl;

					// at least a single white char
					if (!SkipWhitespaces())
					{
						std::cout << "ERROR: delimeter character ' ' expected" << std::endl;
						return false;
					}

					// quotes
					if (!m_reader->Consume(&c))
						return false;
					// TODO...
					if (c != '"' and c != '\'')
					{
						std::cout << "ERROR: begin of string '\"' expected" << std::endl;
						return false;
					}

					// get the filename
					std::string includefile;
					if (!ScanString(c, includefile))
					{
						std::cout << "ERROR: ScanString() returns false" << std::endl;
						return false;
					}
					std::cout << "Lexer -> found #include command for file: \"" << includefile << "\"" << std::endl;

					// next line
					if (!SkipRestofLine())
						return false;
					// TODO...

					// include file
					includefile = std::filesystem::path(filename).remove_filename().generic_string() + includefile;
					std::cout << "Lexer -> including file: \"" << includefile << "\"" << std::endl;

					FileReader* pOldReader = m_reader;
					bool subresult = GenerateTokens(includefile);
					if (m_reader)
						delete m_reader;
					m_reader = pOldReader;
					if (!subresult)
						return false;

					// continue parsing
					continue;
				}

				// unsupported
				std::cout << "ERROR: unsupported preprocessor statement" << std::endl;
				return false;
			}

			// SECTION
			if (c == '.')
			{
				std::string section;
				if (!ScanSymbol(section))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

				std::cout << "Lexer -> found section: ." << section << std::endl;

				// continue parsing
				//continue;
std::cout << "DEBUG-STOP" << std::endl;
return false;
			}

			// unexpected char
			std::cout << "ERROR: unexpected character" << std::endl;
			return false;

		} while (m_reader->Peek());

		// cleanup
		if (m_reader)
			delete m_reader;
		m_reader = nullptr;

        return true;
    }


	bool Lexer::SkipWhitespaces(bool skip_newline)
	{
//		std::cout << "Parser::Lexer::SkipWhitespaces()" << std::endl;

		char c;
		while (m_reader->Peek(&c))
		{
			// skip spaces and tabs
			if (c == ' ' or c == '\t')
			{
				m_reader->Consume();
				continue;
			}

			// skip new line
			if (skip_newline and c == '\n')
			{
				m_reader->Consume();
				continue;
			}

			// no white space
			return true;
		}

		return false;
	}


	bool Lexer::SkipRestofLine()
	{
//		std::cout << "Parser::Lexer::SkipRestofLine()" << std::endl;

		char c;
		while (m_reader->Consume(&c))
		{
			if (c == '\n')
				return true;
		}

		return false;
	}


	bool Lexer::ScanSymbol(std::string& result)
	{
//		std::cout << "Parser::Lexer::ScanSymbol()" << std::endl;
		char c;

		// symbols must start with an alpha char or underscore
		if (!m_reader->Peek(&c))
			return false;
		if (!(c >= 'a' and c <= 'z') and !(c >= 'A' and c <= 'Z') and !(c == '_'))
			return false;
		if (!m_reader->Consume(&c))
			return false;
		result = c;

		// allowed chars are [a-z], [A-Z], [0-9] and [_]
		while (m_reader->Peek(&c))
		{
			if (!(c >= 'a' and c <= 'z') and !(c >= 'A' and c <= 'Z') and !(c >= '0' and c <= '9') and !(c == '_'))
				return true;
			if (!m_reader->Consume(&c))
				return false;
			result += c;
		}

		return false;
	}


	bool Lexer::ScanString(char delim, std::string& result)
	{
//		std::cout << "Parser::Lexer::ScanString()" << std::endl;
		char c;

		while (m_reader->Peek(&c))
		{
			// end-of-line
			if (c == '\n')
				return false;

			// ending delim
			if (c == '\"')
			{
				m_reader->Consume();
				return true;
			}

			// string
			m_reader->Consume();
			result += c;
		}

		return false;
	}


} // namespace Parser

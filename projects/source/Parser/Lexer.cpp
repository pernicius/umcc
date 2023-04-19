#include "pch.h"

#include "Lexer.h"

namespace Parser {


	Lexer::Lexer(const std::string& filename)
		: m_filename(filename)
	{
		std::cout << "Parser::Lexer::Lexer()" << std::endl;

		m_reader = nullptr;
	}


	Lexer::~Lexer()
	{
		std::cout << "Parser::Lexer::~Lexer()" << std::endl;

		if (m_reader)
			delete m_reader;
	}


	bool Lexer::GenerateTokens()
    {
		std::cout << "Parser::Lexer::GenerateTokens()" << std::endl;

		// initialize the file reader
		m_reader = new FileReader(m_filename);
		if (!m_reader->Init())
			return false;

		// loop until end-of-file
		do
		{
			// skip leading spaces
			SkipWhitespaces(true);

			// get the first char of a token
			char c;
			if (!m_reader->Consume(&c))
				return false;
			// TODO...

			// PREPROCESSOR COMMAND
			if (c == '#')
			{
				std::cout << "Lexer -> PreProcCmd begin" << std::endl;

				// get the command
				std::string cmd;
				if (!ScanSymbol(cmd))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

				std::cout << "Lexer -> PreProcCmd: #" << cmd << std::endl;

				// #include ...
				if (cmd.compare("include") == 0)
				{
					std::cout << "Lexer -> Preprocessor: #include statement" << std::endl;

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
					std::string filename;
					if (!ScanString(c, filename))
					{
						std::cout << "ERROR: ScanString() returns false" << std::endl;
						return false;
					}
					std::cout << "Lexer -> Preprocessor: #include file: \"" << filename << "\"" << std::endl;

					// next line
					if (!SkipRestofLine())
						return false;
					// TODO...

					// include file
					std::cout << "Lexer -> Preprocessor: including file: \"" << filename << "\"" << std::endl;
					// TODO...

					// continue parsing
					continue;
				}

				// unsupported
				std::cout << "ERROR: unsupported preprocessor statement" << std::endl;
				return false;
			}

			// unexpected char
			// TODO...
			return false;

			// SECTION
			if (c == '.')
			{
				std::cout << "SECTION: ." << std::endl;

				std::string buf;
				if (!ScanSymbol(buf))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

				continue;
			}

		} while (m_reader->Peek());

        return true;
    }


	bool Lexer::SkipWhitespaces(bool skip_newline)
	{
		std::cout << "Parser::Lexer::SkipWhitespaces()" << std::endl;

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
		std::cout << "Parser::Lexer::SkipRestofLine()" << std::endl;

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
		std::cout << "Parser::Lexer::ScanSymbol()" << std::endl;
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
		std::cout << "Parser::Lexer::ScanString()" << std::endl;
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

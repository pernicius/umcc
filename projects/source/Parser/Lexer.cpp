#include "pch.h"

#include "Lexer.h"
#include "data_tokens.h"

#include <filesystem>

namespace Parser {


	std::vector<token_data> v_data_tokens;


	Lexer::Lexer()
		: m_reader(nullptr)
	{
	}


	Lexer::~Lexer()
	{
		if (m_reader)
			delete m_reader;
	}


	bool Lexer::GenerateTokens(const std::string& filename)
    {
		// initialize the file reader
		m_reader = new FileReader(filename);
		if (!m_reader->Init())
			return false;

		// loop until end-of-file
		char c, c2;
		short p_coll, p_line;
		do
		{
			// skip leading spaces
			SkipWhitespaces(true);

			// get the first char of a token (if any)
			m_reader->GetPos(p_coll, p_line);
			if (!m_reader->Peek(&c))
				return true;

			// comments
			if (c == '/')
			{
				if (m_reader->Peek(&c2, 1))
				{
					// single-line comments
					if (c2 == '/')
					{
						SkipRestofLine();

						// continue parsing
						continue;
					}

					// multi-line comments
					// TODO... multi-line comments
				}
			}

			// PREPROCESSOR COMMAND
			if (c == '#')
			{
				m_reader->Consume();

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
					// TODO... error handling
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
//					std::cout << "Lexer -> found #include command for file: \"" << includefile << "\"" << std::endl;

					// include file
					includefile = std::filesystem::path(filename).remove_filename().generic_string() + includefile;
//					std::cout << "Lexer -> including file: \"" << includefile << "\"" << std::endl;

					FileReader* pOldReader = m_reader;
					bool subresult = GenerateTokens(includefile);
					if (m_reader)
						delete m_reader;
					m_reader = pOldReader;
					if (!subresult)
						return false;

					// next line
					SkipRestofLine();

					// continue parsing
					continue;
				}

				// unsupported
				std::cout << "ERROR: unsupported preprocessor statement" << std::endl;
				return false;
			}

			// TOKEN_SECTION
			if (c == '.')
			{
				m_reader->Consume();

				// get section id
				std::string section;
				if (!ScanSymbol(section))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

//				std::cout << "Lexer -> found TOKEN_SECTION(\"." << section << "\")" << std::endl;

				// store token
				token_data td;
				td.type = TOKEN_SECTION;
				td.value = section;
				td.line_idx = p_line;
				td.pos_start = p_coll;
				td.pos_len = (short)section.length() + 1;
				v_data_tokens.push_back(td);

				// continue parsing
				continue;
			}

			// TOKEN_NUMBER
			if ((c >= '0') and (c <= '9'))
			{
				std::string buf;
				ScanNumber(buf);

//				std::cout << "Lexer -> found TOKEN_NUMBER(\"" << buf << "\")" << std::endl;

				// store token
				token_data td;
				td.type = TOKEN_NUMBER;
				td.value = buf;
				td.line_idx = p_line;
				td.pos_start = p_coll;
				td.pos_len = (short)buf.length();
				v_data_tokens.push_back(td);

				// continue parsing
				continue;
			}

			// TOKEN_SYMBOL
			if ((c == '_') or ((c >= 'a') and (c <= 'z')) or ((c >= 'A') and (c <= 'Z')))
			{
				// get symbol
				std::string symbol;
				if (!ScanSymbol(symbol))
				{
					std::cout << "ERROR: ScanSymbol() returns false" << std::endl;
					return false;
				}

//				std::cout << "Lexer -> found TOKEN_SYMBOL(\"" << symbol << "\")" << std::endl;

				// store token
				token_data td;
				td.type = TOKEN_SYMBOL;
				td.value = symbol;
				td.line_idx = p_line;
				td.pos_start = p_coll;
				td.pos_len = (short)symbol.length();
				v_data_tokens.push_back(td);

				// continue parsing
				continue;
			}

			// TOKEN_STRING
			// TODO... TOKEN_STRING

			// single char tokens
			if ((c == '=') or (c == ':') or (c == ';') or (c == ',') or (c == '{') or (c == '}') or (c == '(') or (c == ')') or (c == '[') or (c == ']'))
			{
				token_type t;

				if (c == '=')
				{
//					std::cout << "Lexer -> found TOKEN_EQUAL(\"" << c << "\")" << std::endl;
					t = TOKEN_EQUAL;
				}
				if (c == ':')
				{
//					std::cout << "Lexer -> found TOKEN_COLON(\"" << c << "\")" << std::endl;
					t = TOKEN_COLON;
				}
				if (c == ';')
				{
//					std::cout << "Lexer -> found TOKEN_SEMI(\"" << c << "\")" << std::endl;
					t = TOKEN_SEMI;
				}
				if (c == ',')
				{
//					std::cout << "Lexer -> found TOKEN_COMMA(\"" << c << "\")" << std::endl;
					t = TOKEN_COMMA;
				}
				if ((c == '{') or (c == '}') or (c == '(') or (c == ')') or (c == '[') or (c == ']'))
				{
//					std::cout << "Lexer -> found TOKEN_PAREN(\"" << c << "\")" << std::endl;
					t = TOKEN_PAREN;
				}

				m_reader->Consume();

				// store token
				token_data td;
				td.type = t;
				td.value = c;
				td.line_idx = p_line;
				td.pos_start = p_coll;
				td.pos_len = 1;
				v_data_tokens.push_back(td);

				// continue parsing
				continue;
			}

			// unexpected char
			std::cout << "ERROR: unexpected character '" << c << "'" << std::endl;
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
		char c;
		result = "";

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


	void Lexer::ScanNumber(std::string& result)
	{
		bool is_bin = false;
		bool is_hex = false;
		char c;
		result = "";

		// number must start with an numeric char
		if (!m_reader->Peek(&c))
			return;
		if (!(c >= '0' and c <= '9'))
			return;
		if (!m_reader->Consume(&c))
			return;
		result = c;

		while (m_reader->Peek(&c))
		{
			// check type of number
			if ((result.length() == 1) and (result[0] == '0'))
			{
				// binary number
				if (c == 'b')
				{
					result += c;
					is_bin = true;
					m_reader->Consume();
					continue;
				}
				// haxadecimal number
				if (c == 'x')
				{
					result += c;
					is_hex = true;
					m_reader->Consume();
					continue;
				}
			}

			if ((c >= '0') and (c <= '1'))
			{
				result += c;
				m_reader->Consume();
				continue;
			}

			if (!is_bin and (c >= '2') and (c <= '9'))
			{
				result += c;
				m_reader->Consume();
				continue;
			}

			if (is_hex and (((c >= 'a') and (c <= 'f')) or ((c >= 'A') and (c <= 'F'))))
			{
				result += c;
				m_reader->Consume();
				continue;
			}

			return;
		}
	}


	bool Lexer::ScanString(char delim, std::string& result)
	{
		char c;
		result = "";

		while (m_reader->Peek(&c))
		{
			// end-of-line
			if (c == '\n')
				return false;

			// ending delim
			if (c == delim)
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

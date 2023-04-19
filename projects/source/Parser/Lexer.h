#pragma once

#include <vector>

#include "FileReader.h"

namespace Parser {


	enum token_type {
		TOKEN_SECTION = 1, // .XYZ
		TOKEN_SYMBOL,      // xyz
		TOKEN_STRING,      // "xyz" or 'xyz'
		TOKEN_NUMBER,      // 1234 or 0b0101 or 0x12ab or 0x12AB
		TOKEN_PAREN,       // (){}[]
		TOKEN_COLON,       // :
		TOKEN_COMMA,       // ,
		TOKEN_SEMI         // ;
	};


	struct token_data {
		token_type type;
		std::string value;

//TODO: pointer to line storage
		short pos_start;
		short pos_len;
	};


	class Lexer
	{
	public:

		Lexer();
		~Lexer();

	public:

		bool GenerateTokens(const std::string& filename);

//		bool Peek();
//		bool Consume();

	private:

		// [ ][\t] if skip_newline is true also [\r][\n]
		bool SkipWhitespaces(bool skip_newline = false);
		// read until '\n'
		bool SkipRestofLine();

		// [_a-zA-Z][_a-zA-Z0-9]...
		bool ScanSymbol(std::string& result);
		// [0-9]... or [0][b][0-9]... or [0][x][0-9a-fA-F]...
//		bool ScanNumber(short& result);
		// ["][...]["] or ['][...][']
		bool ScanString(char delim, std::string& result);

	private:

		FileReader* m_reader;

		std::vector<token_data> m_tokens;

	}; // class Lexer


} // namespace Parser

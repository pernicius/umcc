#pragma once

#include <vector>

#include "FileReader.h"

namespace Parser {


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
		void ScanNumber(std::string& result);
		// ["][...]["] or ['][...][']
		bool ScanString(char delim, std::string& result);

	private:

		FileReader* m_reader;

	}; // class Lexer


} // namespace Parser

#pragma once


namespace Parser {

	// ===== class FileReader =====

	// i/o errors
	constexpr char E_FILE_CANT_OPEN[]          = "can't open file";
	constexpr char E_FILE_READ[]               = "can't read file";
	// internals
	constexpr char E_FILE_INT[]                = "internal error (please report)";

	// ===== class Lexer

	// ===== class Parser =====

	// missing main tokens
	constexpr char E_SYNTAX_MISS_SECTION[]     = "section expected";
	constexpr char E_SYNTAX_MISS_SYMBOL[]      = "symbol expected";
	constexpr char E_SYNTAX_MISS_NUMBER[]      = "number expected";
	// missing bracket tokens
	constexpr char E_SYNTAX_MISS_LCBRAC[]      = "opening brackets '{' expected";
	constexpr char E_SYNTAX_MISS_RCBRAC[]      = "closing brackets '}' expected";
	constexpr char E_SYNTAX_MISS_LRBRAC[]      = "opening brackets '(' expected";
	constexpr char E_SYNTAX_MISS_RRBRAC[]      = "closing brackets ')' expected";
	constexpr char E_SYNTAX_MISS_LSBRAC[]      = "opening brackets '[' expected";
	constexpr char E_SYNTAX_MISS_RSBRAC[]      = "closing brackets ']' expected";
	// missing char tokens
	constexpr char E_SYNTAX_MISS_COLON[]       = "character ':' expected";
	constexpr char E_SYNTAX_MISS_SEMI[]        = "character ';' expected";
	constexpr char E_SYNTAX_MISS_COMMA[]       = "character ',' expected";
	constexpr char E_SYNTAX_MISS_EQUAL[]       = "character '=' expected";
	// unsupported
	constexpr char E_SYNTAX_SECTION_UNKNOWN[]  = "unsupported section type";
	// limits
	constexpr char E_SYNTAX_LIM_INPUT[]        = "only a single .INPUTS section is allowed";
	// multiple definitions
	constexpr char E_SYNTAX_DOUBLE_SYMBOL[]    = "symbol already defined";
	// undefined
	constexpr char E_SYNTAX_NO_INPUT[]         = "input not defined";
	constexpr char E_SYNTAX_NO_CONSTANT[]      = "constant not defined";
	constexpr char E_SYNTAX_NO_OPTPUT[]        = "output not defined";


} // namespace Parser




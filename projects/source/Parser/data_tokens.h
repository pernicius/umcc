#pragma once

#include <string>
#include <vector>

namespace Parser {


	typedef enum {
		TOKEN_SECTION = 1, // .XYZ
		TOKEN_SYMBOL,      // xyz
		TOKEN_STRING,      // "xyz" or 'xyz'
		TOKEN_NUMBER,      // 1234 or 0b0101 or 0x12ab or 0x12AB
		TOKEN_PAREN,       // (){}[]
		TOKEN_COLON,       // :
		TOKEN_COMMA,       // ,
		TOKEN_SEMI,        // ;
		TOKEN_EQUAL        // =
	} token_type;


	typedef struct {
		token_type type;
		std::string value;
		short line_idx;
		short pos_start;
		short pos_len;
	} token_data;
	extern std::vector<token_data> v_data_tokens;


} // namespace Parser

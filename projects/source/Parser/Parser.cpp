#include "pch.h"

#include "Parser.h"

#include "data_parsed.h"


namespace Parser {


	std::vector<input_data> data_inputs;
	std::vector<signal_data> data_signals;
	std::map<std::string, output_data> data_outputs;
	std::map<std::string, short> data_constants;
	std::vector<rule_data> data_rules;


	Parser::Parser()
		: m_token_idx(0)
	{
	}


	bool Parser::ParseTokens()
	{
		token_data* p_token;
		token_data* p_tmp;

		// each token...
		while (TokenConsume(&p_token))
		{
			if (p_token->type != TOKEN_SECTION)
			{
				std::cout << "ERROR: section expected" << std::endl;
				return false;
			}

			// section block start
			if (!CheckToken(&p_tmp, TOKEN_PAREN, "{", "ERROR: opening brackets '{' expected"))
				return false;

			// 
			if (p_token->value.compare("INPUTS") == 0)
			{
				while (1)
				{
					// end-of-section ?
					if (TestToken(TOKEN_PAREN, "}"))
					{
						TokenConsume();
						break;
					}

					input_data data;

					// format:  <number> <colon> <symbol> <semi>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
						return false;
					data.num_bits = to_number(p_tmp->value);
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					data.symbol = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;

					// store data
					data_inputs.push_back(data);
				}
				continue;

			}
			else if (p_token->value.compare("SIGNALS") == 0)
			{
				while (1)
				{
					// end-of-section ?
					if (TestToken(TOKEN_PAREN, "}"))
					{
						TokenConsume();
						break;
					}

					signal_data data;
					data.num_bits = 0;

					// format:  <symbol> <paren'('> ... <paren')'> <semi>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					data.symbol = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "ERROR: opening brackets '(' expected"))
						return false;

					// bit definition
					while (1)
					{
						// end-of-bits ?
						if (TestToken(TOKEN_PAREN, ")"))
						{
							TokenConsume();
							break;
						}

						signal_bit data_bit;
						data_bit.sel_bit = -1;
						data_bit.const_val = -1;

						// format(1):  <number> <colon> <symbol> <semi>
						// format(2):  <number> <colon> <symbol> <paren'['> <number> <paren']'> <semi>
						// format(3):  <number> <colon> <number> <semi>
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
							return false;
						data_bit.num_bits = to_number(p_tmp->value);
						data.num_bits += data_bit.num_bits;
						if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
							return false;

						// (1), (2)
						if (TestToken(TOKEN_SYMBOL, ""))
						{
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
								return false;
							data_bit.symbol = p_tmp->value;
							// (1)
							if (TestToken(TOKEN_SEMI, ""))
							{
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
									return false;
							}
							// (2)
							else
							{
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "[", "ERROR: opening brackets '[' expected"))
									return false;
								if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
									return false;
								data_bit.sel_bit = to_number(p_tmp->value);
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "]", "ERROR: closing brackets ']' expected"))
									return false;
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
									return false;
							}
						}
						// (3)
						else
						{
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
								return false;
							data_bit.const_val = to_number(p_tmp->value);
							if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
								return false;
						}

						// store bits
						data.v_bits.push_back(data_bit);
					}

					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;

					// store data
					data_signals.push_back(data);
				}
				continue;
			}
			else if (p_token->value.compare("OUTPUTS") == 0)
			{
				while (1)
				{
					// end-of-section ?
					if (TestToken(TOKEN_PAREN, "}"))
					{
						TokenConsume();
						break;
					}

					std::string key;
					output_data data;
					data.def_val = 0;

					// format(1):  <number> <colon> <symbol> <semi>
					// format(2):  <number> <colon> <symbol> <comma> <number> <semi>

					// (1), (2)
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
						return false;
					data.num_bits = to_number(p_tmp->value);
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					key = p_tmp->value;

					// (2)
					if (!TestToken(TOKEN_SEMI, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_COMMA, "", "ERROR: character ',' expected"))
							return false;
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
							return false;
						data.def_val = to_number(p_tmp->value);
					}

					// (1), (2)
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;

					// store data
					data_outputs[key] = data;
				}
				continue;
			}
			else if (p_token->value.compare("CONSTANTS") == 0)
			{
				while (1)
				{
					// end-of-section ?
					if (TestToken(TOKEN_PAREN, "}"))
					{
						TokenConsume();
						break;
					}

					std::string key;
					short value;

					// format:  <symbol> <colon> <number> <semi>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					key = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
						return false;
					value = to_number(p_tmp->value);
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;

					// store data
					data_constants[key] = value;
				}
				continue;
			}
			else if (p_token->value.compare("RULE") == 0)
			{
				// format:  <paren'('> ... <paren')> <comma> <paren'('> ... <paren')>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "ERROR: opening brackets '(' expected"))
					return false;

				rule_data data;

				// input filter
				while (1)
				{
					rule_filter filter;

					// format(1):  <symbol> <equal> <number>
					// format(2):  <symbol> <equal> <symbol>

					// (1), (2)
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					filter.symbol = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", "ERROR: character '=' expected"))
						return false;

					// (1)
					if (TestToken(TOKEN_NUMBER, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
							return false;
						filter.value = to_number(p_tmp->value);
					}
					// (2)
					else
					{
						if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
							return false;

						// lookup constant
						if (!data_constants.contains(p_tmp->value))
						{
							std::cout << "ERROR: constant not defined" << std::endl;
							return false;
						}
						filter.value = data_constants[p_tmp->value];
					}

					// store filter
					data.filters.push_back(filter);
					
					// end-of-filter ?
					if (!TestToken(TOKEN_COMMA, ""))
						break;
					TokenConsume();
				}

				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", "ERROR: closing brackets ')' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_COMMA, "", "ERROR: character ',' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "ERROR: opening brackets '(' expected"))
					return false;

				// output setting
				while (1)
				{
					rule_setting setting;
					setting.value = -1;

					// format(1):  <symbol> <equal> <number>
					// format(2):  <symbol> <equal> <symbol>
					// format(3):  <symbol>

					// (1), (2), (3)
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					setting.symbol = p_tmp->value;

					// (1), (2)
					if (TestToken(TOKEN_EQUAL, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", "ERROR: character '=' expected"))
							return false;

						// (1)
						if (TestToken(TOKEN_NUMBER, ""))
						{
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
								return false;
							setting.value = to_number(p_tmp->value);
						}
						// (2)
						else
						{
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
								return false;

							// lookup constant
							if (!data_constants.contains(p_tmp->value))
							{
								std::cout << "ERROR: constant not defined" << std::endl;
								return false;
							}
							setting.value = data_constants[p_tmp->value];
						}
					}

					// lookup default value of output
					if (setting.value == -1)
						setting.value = ~data_outputs[setting.symbol].def_val;

					// store setting
					data.settings.push_back(setting);

					// end-of-setting ?
					if (!TestToken(TOKEN_COMMA, ""))
						break;
					TokenConsume();
				}

				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", "ERROR: closing brackets ')' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "}", "ERROR: closing brackets '}' expected"))
					return false;

				// store data
				data_rules.push_back(data);
			}
			else
			{
				std::cout << "ERROR: unsupported section type \"." << p_token->value << "\"" << std::endl;
				return false;
			}
		}

		return true;
	}


	bool Parser::TokenPeek(token_data** p_result, short pos_idx)
	{
		// check size limits
		if (v_data_tokens.size() <= (m_token_idx + pos_idx))
			return false;

		// return requested token
		if (p_result != nullptr)
			*p_result = &v_data_tokens[m_token_idx + pos_idx];

		return true;
	}


	bool Parser::TokenConsume(token_data** p_result)
	{
		// check size limits
		if (v_data_tokens.size() <= (m_token_idx))
			return false;

		// return requested token
		if (p_result != nullptr)
			*p_result = &v_data_tokens[m_token_idx];

		// advance position
		m_token_idx++;

		return true;
	}


	bool Parser::CheckToken(token_data** p_result, token_type req_type, const std::string& req_value, const std::string& err_msg)
	{
		// try to get a token
		if (!TokenConsume(p_result))
		{
			std::cout << err_msg << std::endl;
			return false;
		}

		// check type
		if ((*p_result)->type != req_type)
		{
			std::cout << err_msg << std::endl;
			return false;
		}

		// check value
		if (req_value.length() > 0)
		{
			if ((*p_result)->value.compare(req_value) != 0)
			{
				std::cout << err_msg << std::endl;
				return false;
			}
		}

		return true;
	}


	bool Parser::TestToken(token_type req_type, const std::string& req_value)
	{
		token_data* p_tmp;

		if (!TokenPeek(&p_tmp))
			return false;

		if (p_tmp->type != req_type)
			return false;

		if (req_value.length() > 0)
		{
			if (p_tmp->value.compare(req_value) != 0)
				return false;
		}

		return true;
	}


	short Parser::to_number(std::string& value)
	{
		short result = 0;

		// decimal
		if ((value.length() < 3) or ((value.length() > 2) and ((value[1] != 'x') and (value[1] != 'b'))))
			return std::atoi(value.c_str());

		// binary
		if (value[1] == 'b')
		{
			for (auto x = 2; x < value.length(); x++)
			{
				result = result << 1;
				result += value[x] - '0';
			}
			return result;
		}

		// hexadecimal
		if (value[1] == 'x')
		{
			for (auto x = 2; x < value.length(); x++)
			{
				result = result << 4;
				if ((value[x] >= '0') and (value[x] <= '9'))
					result += value[x] - '0';
				else if ((value[x] >= 'a') and (value[x] <= 'z'))
					result += value[x] - 'a';
				else
					result += value[x] - 'A';
			}
			return result;
		}

		return 0;
	}


} // namespace Parser

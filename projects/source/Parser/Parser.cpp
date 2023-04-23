#include "pch.h"

#include "Parser.h"
#include "Util.h"

#include "data_parsed.h"


namespace Parser {


	// see "data_parsed.h"
	std::map<std::string, input_data> data_inputs;
	std::map<std::string, signal_data> data_signals;
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
				return PrintLineError(*p_token, "section expected");
			if (!CheckToken(&p_tmp, TOKEN_PAREN, "{", "opening brackets '{' expected"))
				return false;

			// ==================
			// ===== INPUTS =====
			// ==================
			if (p_token->value.compare("INPUTS") == 0)
			{
				// check if there was a previous .INPUTS section
				if (data_inputs.size() > 0)
					return PrintLineError(*p_token, "only a single .INPUTS section is allowed");

				short start_bit = 0;

				while (1)
				{
					// end-of-section ?
					if (TestToken(TOKEN_PAREN, "}"))
					{
						TokenConsume();
						break;
					}

					std::string key;
					input_data data;

					// format:  <number> <colon> <symbol> <semi>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
						return false;
					data.start_bit = start_bit;
					data.num_bits = to_number(p_tmp->value);
					start_bit += data.num_bits;
// TODO: number of bits exceeded test
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
					if (data_inputs.count(key))
						return PrintLineError(*p_tmp, "symbol already defined");
					key = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
						return false;

					// store data
					data_inputs[key] = data;
				}
				continue;

			}
			// ===================
			// ===== SIGNALS =====
			// ===================
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

					std::string key;
					signal_data data;
					data.num_bits = 0;

					// format:  <symbol> <paren'('> ... <paren')'> <semi>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
// TOTO: test: double def
					key = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "opening brackets '(' expected"))
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
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
							return false;
						data_bit.num_bits = to_number(p_tmp->value);
						data.num_bits += data_bit.num_bits;
						if (!CheckToken(&p_tmp, TOKEN_COLON, "", "character ':' expected"))
							return false;

						// (1), (2)
						if (TestToken(TOKEN_SYMBOL, ""))
						{
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
								return false;
// TODO: test: exists
							data_bit.symbol = p_tmp->value;
							// (1)
							if (TestToken(TOKEN_SEMI, ""))
							{
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
									return false;
							}
							// (2)
							else
							{
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "[", "opening brackets '[' expected"))
									return false;
								if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
									return false;
// TODO: test: value '1'
								data_bit.sel_bit = to_number(p_tmp->value);
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "]", "closing brackets ']' expected"))
									return false;
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
									return false;
							}
						}
						// (3)
						else
						{
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
								return false;
							data_bit.const_val = to_number(p_tmp->value);
							if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
								return false;
						}

						// store bits
						data.v_bits.push_back(data_bit);
					}

					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
						return false;

					// store data
					data_signals[key] = data;
				}
				continue;
			}
			// ===================
			// ===== OUTPUTS =====
			// ===================
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
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
						return false;
					data.num_bits = to_number(p_tmp->value);
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
// TODO: test: double def (except '_UNUSED')
					key = p_tmp->value;

					// (2)
					if (!TestToken(TOKEN_SEMI, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_COMMA, "", "character ',' expected"))
							return false;
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
							return false;
						data.def_val = to_number(p_tmp->value);
					}

					// (1), (2)
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
						return false;

					// store data
					data_outputs[key] = data;
				}
				continue;
			}
			// =====================
			// ===== CONSTANTS =====
			// =====================
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
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
// TODO: test: def in inputs
// TODO: test: def in signals
// TODO: test: def in outputs
// TODO: test: double def
					key = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
						return false;
					value = to_number(p_tmp->value);
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "character ';' expected"))
						return false;

					// store data
					data_constants[key] = value;
				}
				continue;
			}
			// ================
			// ===== RULE =====
			// ================
			else if (p_token->value.compare("RULE") == 0)
			{
				// format:  <paren'('> ... <paren')> <comma> <paren'('> ... <paren')>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "opening brackets '(' expected"))
					return false;

				rule_data data;

				// input filter
				while (1)
				{
					rule_filter filter;

					// format(1):  <symbol> <equal> <number>
					// format(2):  <symbol> <equal> <symbol>

					// (1), (2)
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
// TODO: test: exists
					filter.symbol = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", "character '=' expected"))
						return false;

					// (1)
					if (TestToken(TOKEN_NUMBER, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
							return false;
						filter.value = to_number(p_tmp->value);
					}
					// (2)
					else
					{
						if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
							return false;

						// lookup constant
						if (!data_constants.contains(p_tmp->value))
							return PrintLineError(*p_tmp, "constant not defined");
						filter.value = data_constants[p_tmp->value];
					}

					// store filter
					data.filters.push_back(filter);
					
					// end-of-filter ?
					if (!TestToken(TOKEN_COMMA, ""))
						break;
					TokenConsume();
				}

				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", "closing brackets ')' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_COMMA, "", "character ',' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", "opening brackets '(' expected"))
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
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
						return false;
// TODO: test: exists
					setting.symbol = p_tmp->value;

					// (1), (2)
					if (TestToken(TOKEN_EQUAL, ""))
					{
						if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", "character '=' expected"))
							return false;

						// (1)
						if (TestToken(TOKEN_NUMBER, ""))
						{
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "number expected"))
								return false;
							setting.value = to_number(p_tmp->value);
						}
						// (2)
						else
						{
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "symbol expected"))
								return false;

							// lookup constant
							if (!data_constants.contains(p_tmp->value))
								return PrintLineError(*p_tmp, "constant not defined");
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

				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", "closing brackets ')' expected"))
					return false;
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "}", "closing brackets '}' expected"))
					return false;

				// store data
				data_rules.push_back(data);
			}
			else
				return PrintLineError(*p_token, "unsupported section type");
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
		bool err = false;

		// try to get a token
		if (!TokenConsume(p_result))
		{
			// print error
			std::cout << err_msg << std::endl;
			return false;
		}

		// check type
		if ((*p_result)->type == req_type)
		{
			// check value
			if (req_value.length() > 0)
			{
				if ((*p_result)->value.compare(req_value) != 0)
					err = true;
			}
		}
		else
			err = true;

		// print error
		if (err)
			return PrintLineError(**p_result, err_msg);

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

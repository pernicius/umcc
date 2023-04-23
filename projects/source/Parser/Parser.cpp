#include "pch.h"

#include "Parser.h"
#include "Util.h"

#include "data_parsed.h"
#include "errors.h"


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
				return PrintLineError(*p_token, E_SYNTAX_MISS_SECTION);
			if (!CheckToken(&p_tmp, TOKEN_PAREN, "{", E_SYNTAX_MISS_LCBRAC))
				return false;

			// ==================
			// ===== INPUTS =====
			// ==================
			if (p_token->value.compare("INPUTS") == 0)
			{
				// check if there was a previous .INPUTS section
				if (data_inputs.size() > 0)
					return PrintLineError(*p_token, E_SYNTAX_LIM_INPUT);

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

					// <number>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
						return false;
					data.start_bit = start_bit;
					data.num_bits = to_number(p_tmp->value);
					start_bit += data.num_bits;
// TODO: number of bits exceeded test
					// <colon>
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", E_SYNTAX_MISS_COLON))
						return false;
					// <symbol>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: double-def
					if (data_inputs.count(key))
						return PrintLineError(*p_tmp, E_SYNTAX_DOUBLE_SYMBOL);
					// <semi>
					key = p_tmp->value;
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
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

					// <symbol>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: double-def
					if (data_signals.count(p_tmp->value))
						return PrintLineError(*p_tmp, E_SYNTAX_DOUBLE_SYMBOL);
					key = p_tmp->value;
					// <paren>
					if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", E_SYNTAX_MISS_LRBRAC))
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

						// (1), (2), (3)

						// <number>
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
							return false;
						data_bit.num_bits = to_number(p_tmp->value);
						data.num_bits += data_bit.num_bits;
						// <colon>
						if (!CheckToken(&p_tmp, TOKEN_COLON, "", E_SYNTAX_MISS_COLON))
							return false;

						// (1), (2)
						if (TestToken(TOKEN_SYMBOL, ""))
						{
							// <symbol>
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
								return false;
							// test: exists
							if (!data_inputs.count(p_tmp->value))
								return PrintLineError(*p_tmp, E_SYNTAX_NO_INPUT);
							data_bit.symbol = p_tmp->value;

							// (1)
							if (TestToken(TOKEN_SEMI, ""))
							{
								// <semi>
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
									return false;
							}
							// (2)
							else
							{
								// <paren>
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "[", E_SYNTAX_MISS_LSBRAC))
									return false;
								// <number>
								if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
									return false;
								data_bit.sel_bit = to_number(p_tmp->value);
								// <paren>
								if (!CheckToken(&p_tmp, TOKEN_PAREN, "]", E_SYNTAX_MISS_RSBRAC))
									return false;
								// <semi>
								if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
									return false;
							}
						}
						// (3)
						else
						{
							// <number>
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
								return false;
							data_bit.const_val = to_number(p_tmp->value);
							// <semi>
							if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
								return false;
						}

						// store bits
						data.v_bits.push_back(data_bit);
					}
					// <semi>
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
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

					// <number>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
						return false;
					data.num_bits = to_number(p_tmp->value);
					// <colon>
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", E_SYNTAX_MISS_COLON))
						return false;
					// <symbol>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: double-def (except '_UNUSED')
					if ((p_tmp->value.compare("_UNUSED") != 0) and data_outputs.count(p_tmp->value))
						return PrintLineError(*p_tmp, E_SYNTAX_DOUBLE_SYMBOL);
					key = p_tmp->value;

					// (2)
					if (!TestToken(TOKEN_SEMI, ""))
					{
						// <comma>
						if (!CheckToken(&p_tmp, TOKEN_COMMA, "", E_SYNTAX_MISS_COMMA))
							return false;
						// <number>
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
							return false;
						data.def_val = to_number(p_tmp->value);
					}

					// (1), (2)

					// <semi>
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
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
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: double-def
					if (data_inputs.count(p_tmp->value)
						or data_signals.count(p_tmp->value)
						or data_outputs.count(p_tmp->value))
						return PrintLineError(*p_tmp, E_SYNTAX_DOUBLE_SYMBOL);
					key = p_tmp->value;
					// <colon>
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", E_SYNTAX_MISS_COLON))
						return false;
					// <number>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
						return false;
					value = to_number(p_tmp->value);
					// <semi>
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", E_SYNTAX_MISS_SEMI))
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

				// <paren>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", E_SYNTAX_MISS_LRBRAC))
					return false;

				rule_data data;

				// input filter
				while (1)
				{
					rule_filter filter;

					// format(1):  <symbol> <equal> <number>
					// format(2):  <symbol> <equal> <symbol>

					// (1), (2)

					// <symbol>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: exists
					if (!data_inputs.count(p_tmp->value))
						return PrintLineError(*p_tmp, E_SYNTAX_NO_INPUT);
					filter.symbol = p_tmp->value;
					// <equal>
					if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", E_SYNTAX_MISS_EQUAL))
						return false;

					// (1)
					if (TestToken(TOKEN_NUMBER, ""))
					{
						// <number>
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
							return false;
						filter.value = to_number(p_tmp->value);
					}
					// (2)
					else
					{
						// <symbol>
						if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
							return false;
						// test: exists
						if (!data_constants.contains(p_tmp->value))
							return PrintLineError(*p_tmp, E_SYNTAX_NO_CONSTANT);
						filter.value = data_constants[p_tmp->value];
					}

					// store filter
					data.filters.push_back(filter);
					
					// end-of-filter ?
					if (!TestToken(TOKEN_COMMA, ""))
						break;
					TokenConsume();
				}

				// <paren>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", E_SYNTAX_MISS_RRBRAC))
					return false;
				// <comma>
				if (!CheckToken(&p_tmp, TOKEN_COMMA, "", E_SYNTAX_MISS_COMMA))
					return false;
				// <paren>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "(", E_SYNTAX_MISS_LRBRAC))
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

					// <symbol>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
						return false;
					// test: exists
					if (!data_outputs.count(p_tmp->value))
						return PrintLineError(*p_tmp, E_SYNTAX_NO_OPTPUT);
					setting.symbol = p_tmp->value;

					// (1), (2)
					if (TestToken(TOKEN_EQUAL, ""))
					{
						// <equal>
						if (!CheckToken(&p_tmp, TOKEN_EQUAL, "", E_SYNTAX_MISS_EQUAL))
							return false;

						// (1)
						if (TestToken(TOKEN_NUMBER, ""))
						{
							// <number>
							if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", E_SYNTAX_MISS_NUMBER))
								return false;
							setting.value = to_number(p_tmp->value);
						}
						// (2)
						else
						{
							// <symbol>
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", E_SYNTAX_MISS_SYMBOL))
								return false;
							// test: exists
							if (!data_constants.contains(p_tmp->value))
								return PrintLineError(*p_tmp, E_SYNTAX_NO_CONSTANT);
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

				// <paren>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, ")", E_SYNTAX_MISS_RRBRAC))
					return false;
				// <paren>
				if (!CheckToken(&p_tmp, TOKEN_PAREN, "}", E_SYNTAX_MISS_RCBRAC))
					return false;

				// store data
				data_rules.push_back(data);
			}
			else
				return PrintLineError(*p_token, E_SYNTAX_SECTION_UNKNOWN);
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

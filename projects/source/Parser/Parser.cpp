#include "pch.h"

#include "Parser.h"

namespace Parser {


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

			std::cout << "Parser -> found section \"." << p_token->value << "\"" << std::endl;

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

std::cout << "inputline" << std::endl;

					// format:  <number> <colon> <symbol> <semi>
					if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;
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

					std::cout << "signalline" << std::endl;

					// format:  <symbol> <paren'('> ... <paren')'> <semi>
					if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
						return false;
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

						std::cout << "signalbitline" << std::endl;

						// format(1):  <number> <colon> <symbol> <semi>
						// format(2):  <number> <colon> <symbol> <paren'['> <number> <paren']'> <semi>
						// format(3):  <number> <colon> <number> <semi>
						if (!CheckToken(&p_tmp, TOKEN_NUMBER, "", "ERROR: number expected"))
							return false;
						if (!CheckToken(&p_tmp, TOKEN_COLON, "", "ERROR: character ':' expected"))
							return false;

						// (1), (2)
						if (TestToken(TOKEN_SYMBOL, ""))
						{
							if (!CheckToken(&p_tmp, TOKEN_SYMBOL, "", "ERROR: symbol expected"))
								return false;
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
							if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
								return false;
						}
					}

					if (!CheckToken(&p_tmp, TOKEN_SEMI, "", "ERROR: character ';' expected"))
						return false;
				}
				continue;
			}
			else if (p_token->value.compare("OUTPUTS") == 0)
			{

// TODO: current working spot
std::cout << "==DEBUG-STOP==" << std::endl;
return false;
			}
//			else if (p_token->value.compare("CONSTANTS") == 0)
//			{
//			}
//			else if (p_token->value.compare("RULE") == 0)
//			{
//			}
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


} // namespace Parser

#pragma once

#include "data_tokens.h"

namespace Parser {


	class Parser
	{
	public:

		Parser();
//		~Parser();

	public:

		bool ParseTokens();

	private:

		bool TokenPeek(token_data** p_result = nullptr, short pos_idx = 0);
		bool TokenConsume(token_data** p_result = nullptr);
		short m_token_idx;

		bool CheckToken(token_data** p_result, token_type req_type, const std::string& req_value, const std::string& err_msg);
		bool TestToken(token_type req_type, const std::string& req_value);

		short to_number(std::string& value);
	};


} // namespace Parser

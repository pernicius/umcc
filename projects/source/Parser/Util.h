#pragma once

#include "data_tokens.h"

namespace Parser {

	bool PrintErrorMsg(const std::string& msg);

	void PrintLine(token_data& token, std::string msg = "");
	bool PrintLineError(token_data& token, const std::string& msg);
	bool PrintLineWarning(token_data& token, const std::string& msg);


} // namespace Parser

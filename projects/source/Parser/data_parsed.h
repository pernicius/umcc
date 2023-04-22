#pragma once

#include <string>
#include <vector>
#include <map>

namespace Parser {


	// ===== INPUTS

	typedef struct {
		short start_bit;  // position of the first bit in the input-word
		short num_bits;   // size of the input symbol in bits
	} input_data;
	extern std::map<std::string, input_data> data_inputs;


	// ===== SIGNALS

	typedef struct {
		short num_bits;
		std::string symbol;
		short sel_bit;
		short const_val;
	} signal_bit;

	typedef struct {
		short num_bits;
		std::vector<signal_bit> v_bits;
	} signal_data;
	extern std::map<std::string, signal_data> data_signals;


	// ===== OUTPUTS

	typedef struct {
		short num_bits;
		short def_val;
	} output_data;
	extern std::map<std::string, output_data> data_outputs;


	// ===== CONSTANTS

	extern std::map<std::string, short> data_constants;


	// ===== RULES

	typedef struct {
		std::string symbol;
		short value;
	} rule_filter;

	typedef struct {
		std::string symbol;
		short value;
	} rule_setting;

	typedef struct {
		std::vector<rule_filter> filters;
		std::vector<rule_setting> settings;
	} rule_data;
	extern std::vector<rule_data> data_rules;


} // namespace Parser

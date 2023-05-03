#pragma once

#include <string>
#include <vector>
#include <map>

namespace Parser {


	// ===== INPUTS

	typedef struct {
		short start_bit;  // position of the first bit in the input-word
		short num_bits;   // size of the input symbol in bits
		int mask;         // only the relevant bits are set
		short curr_value; // holds tempoary input value during generation
	} input_data;
	extern std::map<std::string, input_data> data_inputs;


	// ===== SIGNALS

	typedef struct {
		short start_bit;    // position of the first bit in the signal
		short num_bits;     // partial size in bits
		std::string symbol; // referencing symbol
		short sel_bit;      // used bit of ref. symbol
		short const_val;    // constant value
	} signal_bit;

	typedef struct {
		short num_bits;                 // size of the symbol in bits
		std::vector<signal_bit> v_bits; // list of bit definitions
		short curr_value;               // holds tempoary input value during generation
	} signal_data;
	extern std::map<std::string, signal_data> data_signals;


	// ===== OUTPUTS

	typedef struct {
		short start_bit;  // position of the first bit in the signal
		short num_bits;   // size of the output symbol in bits
		short def_val;    // default value
		int mask;         // only the relevant bits are set
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

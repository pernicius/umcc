#pragma once

#include <string>
#include <vector>

namespace Parser {


	// ===== INPUTS

	typedef struct {
		short num_bits;
		std::string symbol;
	} input_data;
	extern std::vector<input_data> v_data_inputs;


	// ===== SIGNALS

	typedef struct {
		short num_bits;
		std::string symbol;
		short sel_bit;
		short const_val;
	} signal_bit;

	typedef struct {
		short num_bits;
		std::string symbol;
		std::vector<signal_bit> v_bits;
	} signal_data;
	extern std::vector<signal_data> v_data_signals;


	// ===== OUTPUTS

	typedef struct {
		short num_bits;
		std::string symbol;
		short def_val;
	} output_data;
	extern std::vector<output_data> v_data_outputs;


	// ===== CONSTANTS

	typedef struct {
		std::string symbol;
		short value;
	} constant_data;
	extern std::vector<constant_data> v_data_constants;


} // namespace Parser

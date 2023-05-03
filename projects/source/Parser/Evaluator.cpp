#include "pch.h"

#include "Evaluator.h"

#include <cmath>
#include <fstream>
#include <filesystem>

#include "opts.h"
#include "data_parsed.h"
#include "Util.h"
#include "errors.h"

namespace Parser {


// DEBUG
//	int m_curr_input = 0b110100001;
//	inline short GetInputValue(const std::string& input_name)
//	{
//		return (m_curr_input & data_inputs[input_name].mask) >> data_inputs[input_name].start_bit;
//	}
//
//	auto test = GetInputValue("funct3");
// DEBUG


	bool Evaluator::Prepare()
	{
		// gen mask for inputs
		for (auto& input : data_inputs)
			input.second.mask = GenMask(input.second.start_bit, input.second.num_bits);

		// gen mask for outputs
		for (auto& output : data_outputs)
			output.second.mask = GenMask(output.second.start_bit, output.second.num_bits);

		return true;
	}

	bool Evaluator::Generate()
	{
		// calc max value
		short input_bits = 0;
		for (auto i : data_inputs)
			input_bits += i.second.num_bits;
		int max_input = 1 << input_bits;

		// allocate memory for the generator
		m_pgen = new int[max_input];

		// calculate default value
		int default_value = 0;
		for (auto o : data_outputs)
			default_value += o.second.def_val << o.second.start_bit;

		// go through all input values
		for (m_curr_input = 0; m_curr_input < max_input; m_curr_input++)
		{
			// update all values
			UpdateInputs();
			UpdateSignals();

			// set out_val to default
			m_pgen[m_curr_input] = default_value;

			// check rules
			for (auto r : data_rules)
			{
				// check rule filters
				short matches = 0;
				for (auto f : r.filters)
				{
					// check input
					if (data_inputs.count(f.symbol))
					{
						if (data_inputs[f.symbol].curr_value == f.value)
							matches++;
						else
							break;
					}
					// check signal
					else if (data_signals.count(f.symbol))
					{
						if (data_signals[f.symbol].curr_value == f.value)
							matches++;
						else
							break;
					}
				}
				// if all filters have matched -> setup out_val
				if (matches == r.filters.size())
				{
					for (auto s : r.settings)
					{
						m_pgen[m_curr_input] = m_pgen[m_curr_input] & ~data_outputs[s.symbol].mask;
						m_pgen[m_curr_input] |= (s.value << data_outputs[s.symbol].start_bit) & data_outputs[s.symbol].mask;
					}
				}
			}
		}

		// get output bits/bytes
		short num_bits = 0;
		for (auto o : data_outputs)
			num_bits += o.second.num_bits;
		short num_bytes = (short)std::ceil((float)num_bits / 8);

		// try to open/create output file(s)
		std::string corrfilename = std::filesystem::path(config::outfile).remove_filename().generic_string()
			+ std::filesystem::path(config::outfile).filename().replace_extension("").generic_string();
		std::ofstream files[4];
		bool err = false;
		for (short x = 0; x < num_bytes; x++)
		{
			std::string fname = corrfilename;
			if (num_bytes > 1)
				fname += std::to_string(x);
			fname += std::filesystem::path(config::outfile).filename().extension().generic_string();
			files[x].open(fname, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
			if (!files[x].is_open())
			{
				err = true;
				PrintFileError(fname, E_FILE_CANT_OPEN);
			}
		}

		// xyz
		if (!err)
		{
			for (m_curr_input = 0; m_curr_input < max_input; m_curr_input++)
			{
				// binary
				if (config::format.compare("bin") == 0)
				{
					for (short x = 0; x < num_bytes; x++)
					{
						char val = (m_pgen[m_curr_input] >> (x * 8)) & 0b11111111;
						files[x] << val;
					}
				}

				// intel hex
				// TODO...
				// TODO...
				// TODO...

				// logisim hex
				// TODO...
				// TODO...
				// TODO...
			}
		}

		// free used mem
		if (m_pgen)
			delete m_pgen;
		m_pgen = nullptr;

		return true;
	}


	int Evaluator::GenMask(short pos, short len)
	{
		int result = 0;

		// gen mask
		for (auto x = 0; x < len; x++)
		{
			result = result << 1;
			result += 1;
		}

		// move mask
		result = result << pos;

		return result;
	}


	void Evaluator::UpdateInputs()
	{
		// update data_inputs[].curr_value
		for (auto& i : data_inputs)
			i.second.curr_value = (m_curr_input & i.second.mask) >> i.second.start_bit;
	}


	void Evaluator::UpdateSignals()
	{
		// update data_signals[].curr_value
		for (auto& s : data_signals)
		{
			s.second.curr_value = 0;

			// each bit def...
			for (auto b : s.second.v_bits)
			{
				short src_size = 0;
				short src_val = 0;

				// constants
				if (b.const_val != -1)
				{
					src_size = b.num_bits;
					src_val = b.const_val;
				}
				// inputs
				else
				{
					// specific bit of input
					if (b.sel_bit != -1)
					{
						src_size = 1;
						src_val = (data_inputs[b.symbol].curr_value >> b.sel_bit) & 0b1;
					}
					// full input
					else
					{
						src_size = data_inputs[b.symbol].num_bits;
						src_val = data_inputs[b.symbol].curr_value;
					}
				}

				// compute value - single bit
				if (src_size == 1)
				{
					for (auto x = 0; x < b.num_bits; x++)
						s.second.curr_value += (src_val << x) << b.start_bit;
				}
				// compute value - multibit
				else
					s.second.curr_value += src_val << b.start_bit;
			}
		}
	}


} // namespace Parser

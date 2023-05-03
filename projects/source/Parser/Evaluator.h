#pragma once

namespace Parser {


	class Evaluator
	{

	public:

		bool Prepare();
		bool Generate();

	private:

		int GenMask(short pos, short len);

		// used for the rom generation
		void UpdateInputs();
		void UpdateSignals();

		int m_curr_input;
		int* m_pgen;
	};


} // namespace Parser


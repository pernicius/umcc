#pragma once

#include <string>
#include <fstream>

namespace Parser {


	class FileReader
	{
	public:

		FileReader() = delete;
		FileReader(const std::string& filename);
		~FileReader();

	public:

		bool Init();

		/**
		 * Get character at position (current + index).
		 * Position must be on the same line!
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \param index [in] Index of character to return.
		 * \return Returns true on success or false if no character can be returned.
		 */
		bool Peek(char* c = nullptr, short index = 0);

		/**
		 * Extract character at current position and update current position.
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \return Returns true on success or false if no character can be returned.
		 */
		bool Consume(char* c = nullptr);

		short GetPos();

	private:

		bool ReadLine();

		// internal file-name and file-stream
		const std::string& m_filename;
		std::ifstream m_file;
		short m_file_idx;

		// internal buffer and pointer
		std::string m_line;
		short m_linepos = 0;
		short m_linecount = 0;

	}; // class FileReader


} // namespace Parser

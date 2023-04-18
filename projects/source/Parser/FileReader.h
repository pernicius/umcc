#pragma once

#include "CharReader.h"

namespace Parser {


	class FileReader : public CharReaderInterface
	{
	public:

		/// No default constructor.
		FileReader() = delete;

		/// Init the file reader.
		FileReader(const std::string& filename);

		~FileReader();

	public:

		bool Init() override;

		/**
		 * Get character at position (current + index).
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \param index [in] Index of character to return.
		 * \return Returns true on success or false if no character can be returned.
		 */
		bool Peek(char* c = nullptr, short index = 0) override;

		/**
		 * Get character at position (current + index) and update current position.
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \param index [in] Index of character to return.
		 * \return Returns true on success or false if no character can be returned.
		 */
		bool Consume(char* c = nullptr, short index = 0) override;

	private:

		std::string m_filename;

	}; // class FileReader


} // namespace Parser

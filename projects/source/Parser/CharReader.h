
#pragma once

#include <string>

namespace Parser {


	/**
	 * Template for any implementation interface.
	 */
	class CharReaderInterface
	{
	public:

		/**
		 * Initialize the implementation class.
		 * 
		 * \return Returns true on success or false on fail.
		 */
		virtual bool Init() = 0;

		/**
		 * Get character at position (current + index).
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \param index [in] Index of character to return.
		 * \return Returns true on success or false if no character can be returned.
		 */
		virtual bool Peek(char* c = nullptr, short index = 0) = 0;

		/**
		 * Get character at position (current + index) and update current position.
		 *
		 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
		 * \param index [in] Index of character to return.
		 * \return Returns true on success or false if no character can be returned.
		 */
		virtual bool Consume(char* c = nullptr, short index = 0) = 0;

	}; // class CharReaderInterface


	// Dummy definition of implementation classes
	class FileReader;


	/**
	 * Wrapper class for multiple sources.
	 * 
	 * Use CharReader(std::string&) for for a file as a source.
	 */
	class CharReader : public CharReaderInterface
	{
	public:

		/// No default constructor.
		CharReader() = delete;

		/// Init as a file reader.
		CharReader(const std::string& filename);

		~CharReader();

	public:

		/**
		 * Initialize the wrapper and it's underlying implementation class.
		 *
		 * \return Returns true on success or false on fail.
		 */
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

		bool m_InitDone = false;
		FileReader* m_pFileReader;

	}; // class CharReader


} // namespace Parser

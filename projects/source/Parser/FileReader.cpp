#include "pch.h"

#include "FileReader.h"
#include "data_source.h"

#include <filesystem>

namespace Parser {


	std::vector<data_sourcefiles> data_srcfiles;
	std::vector<data_sourcelines> data_srclines;


	FileReader::FileReader(const std::string& filename)
		: m_filename(filename)
	{
//		std::cout << "Parser::FileReader::FileReader()" << std::endl;
	}


	FileReader::~FileReader()
	{
//		std::cout << "Parser::FileReader::~FileReader()" << std::endl;
	}


	bool FileReader::Init()
	{
//		std::cout << "Parser::FileReader::Init()" << std::endl;

		// reformat filename
		std::string corrfilename = std::filesystem::path(m_filename).remove_filename().generic_string()
		                         + std::filesystem::path(m_filename).filename().generic_string();
//		std::cout << "File (internal): \"" << corrfilename << "\"" << std::endl;

		// open requested file
		m_file.open(corrfilename, std::ifstream::in);
		if (!m_file.is_open())
		{
			std::cout << "ERROR: can't open file" << std::endl;
			return false;
		}

		// store file data
		data_sourcefiles sf;
		sf.name = corrfilename;
		data_srcfiles.push_back(sf);
		m_file_idx = (short)data_srcfiles.size() - 1;

		// init buffer and pointer
		if (!ReadLine())
			return false;

		return true;
	}


	/**
	 * Get character at position (current + index).
	 * Position must be on the same line!
	 *
	 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
	 * \param index [in] Index of character to return.
	 * \return Returns true on success or false if no character can be returned.
	 */
	bool FileReader::Peek(char* c, short index)
	{
//		std::cout << "Parser::FileReader::Peek()" << std::endl;

		if (!m_file)
		{
			// end-of-file reached
			if (m_file.eof())
				return false;
			// any other error
			std::cout << "ERROR: filestream error";
			return false;
		}

		// check position
		if (m_linepos + index > m_line.length())
			return false;

		// check end-of-line
		if (m_linepos + index == m_line.length())
		{
			if (c != nullptr)
				*c = '\n';
			return true;
		}

		// other chars
		if (c != nullptr)
			*c = m_line[m_linepos + index];
		return true;
	}


	/**
	 * Extract character at current position and update current position.
	 *
	 * \param c [out] Pointer to a character buffer (if set to nullptr no character will be returned).
	 * \return Returns true on success or false if no character can be returned.
	 */
	bool FileReader::Consume(char* c)
	{
//		std::cout << "Parser::FileReader::Consume()" << std::endl;

		if (!m_file)
		{
			// end-of-file reached
			if (m_file.eof())
				return false;
			// any other error
			std::cout << "ERROR: filestream error";
			return false;
		}

		if ((m_line.length() == 0) or (m_linepos >= m_line.length()))
		{
//			std::cout << "Parser::FileReader::Consume -> getline()" << std::endl;
			ReadLine();
			if (c != nullptr)
				*c = '\n';
			return true;
		}

		if (c != nullptr)
			*c = m_line[m_linepos];
		m_linepos++;

		return true;
	}


	void FileReader::GetPos(short& coll, short& line_idx)
	{
		coll = m_linepos;
		line_idx = (short)data_srclines.size() - 1;
	}


	bool FileReader::ReadLine()
	{
		std::getline(m_file, m_line);
		// TODO... error handling

		m_linepos = 0;
		m_linecount++;

		// store line
		data_sourcelines sl;
		sl.file_idx = m_file_idx;
		sl.line_number = m_linecount;
		sl.line = m_line;
		data_srclines.push_back(sl);

		return true;
	}


} // namespace Parser

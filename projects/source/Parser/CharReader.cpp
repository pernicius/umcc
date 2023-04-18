#include "pch.h"

#include "CharReader.h"
#include "FileReader.h"

namespace Parser {


	CharReader::CharReader(const std::string& filename)
	{
		std::cout << "Parser::CharReader::CharReader()\n";

		// Create the implementation-class
		m_pFileReader = new FileReader(filename);
	}


	CharReader::~CharReader()
	{
		std::cout << "Parser::CharReader::~CharReader()\n";

		// Delete the used implementation-class
		if (m_pFileReader)
			delete(m_pFileReader);
	}


	bool CharReader::Init()
	{
		std::cout << "Parser::CharReader::Init()\n";

		// Abord if already initialized
		if (m_InitDone)
			return true;

		if (m_pFileReader)
			m_InitDone = m_pFileReader->Init();

		return m_InitDone;
	}


	bool CharReader::Peek(char* c, short index)
	{
		std::cout << "Parser::CharReader::Peek()\n";

		// Abord if not initialized
		if (!m_InitDone)
			return false;

		// Call the appropriate method of the implementation-class
		if (m_pFileReader)
			return m_pFileReader->Peek(c, index);

		return false;
	}


	bool CharReader::Consume(char* c, short index)
	{
		std::cout << "Parser::CharReader::Consume()\n";

		// Abord if not initialized
		if (!m_InitDone)
			return false;

		// Call the appropriate method of the implementation-class
		if (m_pFileReader)
			return m_pFileReader->Consume(c, index);

		return false;
	}


} // namespace Parser

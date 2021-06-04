#include "MiniginPCH.h"
#include "BinaryReader.h"

dae::BinaryReader::BinaryReader(const std::string& filePath)
{
	m_pStream = new std::ifstream();
	m_pStream->open(filePath, std::ios::in | std::ios::binary);

	if(!m_pStream->is_open())
	{
		std::cout << "BinaryReader::BinaryReader: Could not open file: " << filePath << '\n';
		delete m_pStream;
		m_pStream = nullptr;
		return;
	}
}

dae::BinaryReader::~BinaryReader()
{
	if(m_pStream)
	{
		m_pStream->close();
	}
	delete m_pStream;
}

bool dae::BinaryReader::IsActive() const
{
	return m_pStream != nullptr;
}

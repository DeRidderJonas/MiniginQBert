#pragma once
#include <fstream>
#include <iostream>
#include <string>

namespace dae
{

	class BinaryReader
	{
	public:
		BinaryReader(const std::string& filePath);
		~BinaryReader();

		BinaryReader(const BinaryReader&) = delete;
		BinaryReader& operator=(const BinaryReader&) = delete;
		BinaryReader(BinaryReader&&) = delete;
		BinaryReader& operator=(BinaryReader&&) = delete;

		template<typename T>
		T Read()
		{
			if (m_pStream == nullptr)
			{
				std::cout << "BinaryReader::Read: No stream\n";
				return T{};
			}

			T data{};
			m_pStream->read((char*)&data, sizeof(T));
			return data;
		}

		bool IsActive() const;
	private:
		std::ifstream* m_pStream;
	};


}
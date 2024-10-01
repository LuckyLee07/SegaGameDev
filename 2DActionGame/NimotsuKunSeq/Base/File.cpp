#include "File.h"
#include <iostream>
#include <fstream>
#include <windows.h>

File::File(const char* fileName) : m_size(0), m_data(NULL)
{
	char cwd[MAX_PATH]; //获取当前工作目录
	if (GetCurrentDirectoryA(MAX_PATH, cwd)) {
		std::cout << "FilePath: " << cwd << "\\" << fileName << std::endl;
	}

	std::ifstream inputFile(fileName, std::ifstream::binary);
	inputFile.seekg(0, std::ifstream::end);
	m_size = static_cast<int>(inputFile.tellg());

	if (m_size > 0) // 有数据的情况下
	{
		inputFile.seekg(0, std::ifstream::beg);
		m_data = new char[m_size];
		if (!inputFile.read(m_data, m_size))
		{
			m_size = 0;
			delete[] m_data;
			m_data = NULL;
		}
	}
}

File::~File()
{
	delete[] m_data;
	m_data = NULL;
}

int File::size() const
{
	return m_size;
}

const char* File::data() const
{
	return m_data;
}

unsigned File::getUnsigned(int pos) const
{
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(m_data);

	unsigned ret = up[pos];
	ret |= (up[pos + 1] << 8);
	ret |= (up[pos + 2] << 16);
	ret |= (up[pos + 3] << 24);

	return ret;
}
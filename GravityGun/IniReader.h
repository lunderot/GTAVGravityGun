#pragma once
#include <string>
#include <sstream>
#include <Windows.h>

class IniReader
{
private:
	std::string filename;
public:
	IniReader(std::string filename);
	~IniReader();

	float ReadFloat(std::string section, std::string name, float default);
	std::string ReadString(std::string section, std::string name, std::string default);
	int ReadInt(std::string section, std::string name, int default);
	unsigned int ReadHexInt(std::string section, std::string name, int default);
};


#include "IniReader.h"


IniReader::IniReader(std::string filename)
{
	this->filename = filename;
}


IniReader::~IniReader()
{
}

float IniReader::ReadFloat(std::string section, std::string name, float default)
{
	std::stringstream sstream;
	float value;

	sstream << default;
	char returnString[64];
	GetPrivateProfileString(section.c_str(), name.c_str(), sstream.str().c_str(), returnString, 64, filename.c_str());
	sstream.str("");
	sstream << returnString;
	sstream >> value;
	return value;
}

std::string IniReader::ReadString(std::string section, std::string name, std::string default)
{
	char returnString[64];
	GetPrivateProfileString(section.c_str(), name.c_str(), default.c_str(), returnString, 64, filename.c_str());
	return std::string(returnString);
}

int IniReader::ReadInt(std::string section, std::string name, int default)
{
	return GetPrivateProfileInt(section.c_str(), name.c_str(), default, filename.c_str());
}

unsigned int IniReader::ReadHexInt(std::string section, std::string name, int default)
{
	std::stringstream sstream;
	unsigned int value;

	sstream << std::hex << default;
	char returnString[64];
	GetPrivateProfileString(section.c_str(), name.c_str(), sstream.str().c_str(), returnString, 64, filename.c_str());
	sstream.str("");
	sstream << std::hex << returnString;
	sstream >> value;
	return value;
}


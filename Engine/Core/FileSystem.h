#pragma once
#include <string>

namespace pbls
{
	void SetFilePath(const std::string& pathname);
	bool ReadFileToString(const std::string& filename, std::string& filestring);
	std::string GetFilePath();
}
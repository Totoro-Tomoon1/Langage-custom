#pragma once
#include "pch.h"

struct LoadText
{
	std::vector<std::string> text;
	std::vector<std::string> LoadTxt(const char* _path);
	void Print();
};


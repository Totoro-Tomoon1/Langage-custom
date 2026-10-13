#pragma once
#include "pch.h"

class MiniFb
{
	uint32_t* buffer;
	int height;
	int width;

public:
	void Init(int _height, int _width);
	void SetBufferColor(int r, int g, int b);
	void Run();
	uint32_t* GetBuffer() { return buffer; }
};


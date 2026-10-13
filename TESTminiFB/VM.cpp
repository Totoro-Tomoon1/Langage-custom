#include "VM.h"
#include "Compilateur.h"

void VM::Load(uint8_t r, uint32_t v)
{
	tab[r] = _mm_set1_epi32(static_cast<int>(v));
}

void VM::Add(uint8_t r, uint8_t a, uint8_t b)
{
	tab[r] = simd.add_vec4_I(tab[a], tab[b]);
}

void VM::Move(uint8_t r1, uint8_t r2)
{
	tab[r1] = tab[r2];
}

void VM::InfSup(uint8_t r, uint8_t a, uint8_t b)
{
	tab[r] = simd.comp_lt(tab[a], tab[b]);
}

void VM::SupInf(uint8_t r, uint8_t a, uint8_t b)
{
	tab[r] = simd.comp_gt(tab[a], tab[b]);
}

void VM::JumBoo(uint8_t id, uint8_t r, int idFunction )
{
	bool isTrue = CheckBool(r);

	if (isTrue)
	{
		//store l'index du jb
		ExecuteFunction(idFunction, id + 1);
	}
}

void VM::Jump(uint8_t id)
{
	ExecuteFunction(id, 0);
}

void VM::DrawColor(uint8_t r, uint8_t g, uint8_t b)
{
	currentColor = MFB_RGB(r, g, b);
}

void VM::DrawPos(uint8_t x, uint8_t y)
{
	int intX = _mm_cvtsi128_si32(tab[x]);
	int intY = _mm_cvtsi128_si32(tab[y]);

	int posInBuffer = width * intY + intX;

	buffer[posInBuffer] = currentColor;
}

void VM::ExecuteFunction(int id, int offset = 0)
{
	for (int i = offset; i < result[id].size(); i++)
	{
		uint32_t test = result[id][i];
		uint8_t op = test & 0xFF;
		uint8_t a = (test >> 8) & 0xFF;
		uint8_t b = (test >> 16) & 0xFF;
		uint8_t c = (test >> 24) & 0xFF;

		if (op == Instruction::LOAD)
		{
			Load(a, b);
		}
		else if (op == Instruction::ADD)
		{
			Add(a, b, c);
		}
		else if (op == Instruction::IS)
		{
			InfSup(a, b, c);
		}
		else if (op == Instruction::JB)
		{
			JumBoo(a, b, id);
		}
		else if (op == Instruction::JMP)
		{
			Jump(a);
		}
		else if (op == Instruction::MOVE)
		{
			Move(b, c);
		}
		else if (op == Instruction::SI)
		{
			SupInf(a, b, c);
		}
		else if (op == Instruction::PRINT)
		{
			Print(a);
		}
		else if (op == Instruction::RET)
		{
			return;
		}
		else if (op == Instruction::STOP)
		{
			isRunning = false;
			return;
		}
		else if (op == Instruction::DRAWCOLOR)
		{
			DrawColor(a, b, c);
		}
		else if (op == Instruction::DRAWPOS)
		{
			DrawPos(a, b);
		}
	}
}

void VM::Print(uint8_t r)
{
	int var[4];
	_mm_storeu_si128((__m128i*)var, tab[r]);
	cout << "Print " << var[0] << endl;
}

bool VM::CheckBool(uint8_t r)
{
	if (simd.check_int(tab[r]) != 0)
	{
		return true;
	}

	return false;
}

void VM::CheckVM(std::unordered_map<int, std::vector<uint32_t>> _result)
{
	currentColor = MFB_RGB(0, 0, 0);

	result = _result;
	
	ExecuteFunction(0);
	
	while (isRunning)
	{
		ExecuteFunction(1);
	}
}

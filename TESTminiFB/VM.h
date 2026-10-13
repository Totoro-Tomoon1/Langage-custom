#pragma once
#include "pch.h"

struct SIMD
{
    // Conversions
    __m128 convert_I_F(__m128i v) // int to float
    {
        return _mm_cvtepi32_ps(v);
    }

    __m128i convert_F_I(__m128 v) // float to int
    {
        return _mm_cvtps_epi32(v);
    }

    __m128i convert_F_I_arrondi(__m128 v) // float to int arrondi
    {
        return _mm_cvttps_epi32(v);
    }

    //---------------------------
    // Charger un vecteur
    __m128i load_vec4_I(const int* data)
    {
        return _mm_loadu_si128((const __m128i*)data);
    }

    // Addition
    __m128i add_vec4_I(__m128i a, __m128i b)
    {
        return _mm_add_epi32(a, b);
    }

    // Soustraction
    __m128i sub_vec4_I(__m128i a, __m128i b)
    {
        return _mm_sub_epi32(a, b);
    }

    // Multiplication
    __m128i mul_vec4_I(__m128i a, __m128i b)
    {
        return _mm_mullo_epi32(a, b);
    }

    // Division
    __m128i div_vec4_I(__m128i a, __m128i b)
    {
        __m128 fa = _mm_cvtepi32_ps(a);
        __m128 fb = _mm_cvtepi32_ps(b);
        __m128 q = _mm_div_ps(fa, fb);
        return _mm_cvttps_epi32(q);
    }

    // Maximum
    __m128i max_vec4_I(__m128i a, __m128i b)
    {
        return _mm_max_epi32(a, b);
    }

    // Minimum
    __m128i min_vec4_I(__m128i a, __m128i b)
    {
        return _mm_min_epi32(a, b);
    }

    // Reduction horizontale
    int horizontal_sum_I(__m128i v)
    {
        __m128i high = _mm_unpackhi_epi64(v, v);
        __m128i s = _mm_add_epi32(v, high);
        s = _mm_add_epi32(s, _mm_shuffle_epi32(s, _MM_SHUFFLE(1, 1, 1, 1)));
        return _mm_cvtsi128_si32(s);
    }

    // Produit scalaire
    int dot_vec4_I(__m128i a, __m128i b)
    {
        return horizontal_sum_I(mul_vec4_I(a, b));
    }

    // Selection branchless
    __m128i clamp_vec4_I(__m128i v, __m128i lo, __m128i hi)
    {
        return min_vec4_I(max_vec4_I(v, lo), hi);
    }

    // Valeur absolue
    __m128i abs_vec4_I(__m128i a)
    {
        return _mm_abs_epi32(a);
    }

    // bit a bit
    __m128i and_vec4_I(__m128i a, __m128i b)
    {
        return _mm_and_si128(a, b);
    }

    __m128i xor_vec4_I(__m128i a, __m128i b)
    {
        return _mm_xor_si128(a, b);
    }

    __m128i or_vec4_I(__m128i a, __m128i b)
    {
        return _mm_or_si128(a, b);
    }

    // Comparaisons
    __m128i comp_eq(__m128i a, __m128i b)
    {
        return _mm_cmpeq_epi32(a, b);
    }

    __m128i comp_gt(__m128i a, __m128i b)
    {
        return _mm_cmpgt_epi32(a, b);
    }

    __m128i comp_lt(__m128i a, __m128i b)
    {
        return _mm_cmplt_epi32(a, b);
    }

    __m128i comp_ge(__m128i a, __m128i b)
    {
        return or_vec4_I(comp_gt(a, b), comp_eq(a, b));
    }

    __m128i comp_le(__m128i a, __m128i b)
    {
        return or_vec4_I(comp_lt(a, b), comp_eq(a, b));
    }

    int check_int(__m128i m)
    {
        return _mm_cvtsi128_si32(m);
    }
};

class VM
{
	int variable;
	SIMD simd;

	std::unordered_map<uint8_t, __m128i> tab;
	std::unordered_map<int, std::vector<uint32_t>> result;

    bool isRunning = true;

    uint32_t currentColor;

    uint32_t* buffer;
    int height;
    int width;

	void Load(uint8_t r, uint32_t v);
	void Add(uint8_t r, uint8_t a, uint8_t b);
	void Move(uint8_t r1, uint8_t r2);
	void InfSup(uint8_t r, uint8_t a, uint8_t b);
	void SupInf(uint8_t r, uint8_t a, uint8_t b);
	void JumBoo(uint8_t id, uint8_t r, int idFunction);
	void Jump(uint8_t id);
    void DrawColor(uint8_t r, uint8_t g, uint8_t b);
    void DrawPos(uint8_t x, uint8_t y);

    void ExecuteFunction(int id,int offset);
    bool CheckBool(uint8_t r);
    void Print(uint8_t r);

public:
	void CheckVM(std::unordered_map<int, std::vector<uint32_t>> _result);
    void SetBuffer(uint32_t* buf) { buffer = buf; }
    void SetSize(int h, int w) { height = h; width = w; }
};


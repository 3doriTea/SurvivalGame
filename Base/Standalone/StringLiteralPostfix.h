#pragma once
#include <Windows.h>
#include <string>

// UTF8文字列である！
#define U8(string)\
reinterpret_cast<const char*>(u8##string)

inline std::wstring operator""_w(const char* _str, std::size_t _size)
{
	if (_size == 0)
	{
		return L"";
	}

	int requestSize{ MultiByteToWideChar(CP_UTF8, 0, _str, static_cast<int>(_size), nullptr, 0)};
	std::wstring wstrBuff(requestSize, '\0');
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(_str), static_cast<int>(_size), wstrBuff.data(), requestSize);
	return wstrBuff;
}

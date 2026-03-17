#pragma once
#include <string>
#include <format>
#include <Windows.h	>


// アサーション 評価式のみ
#define GB_ASSERT_1(expression)\
if (!(expression))\
{\
	MessageBox(\
		NULL,\
		std::format("Expr:{}\r\nLastErrorCode:{}", (#expression), GetLastError()).c_str(),\
		"assertion failed!!!\r\n",\
		MB_OK);\
	while (true) { __debugbreak(); }\
}

// アサーション 評価式 + ランタイムメッセージ
#define GB_ASSERT_2(expression, message)\
if (!(expression))\
{\
	MessageBox(\
		NULL,\
		std::format("Expr:{}\r\nLastErrorCode:{}\r\nMessage:{}", #expression , GetLastError(), std::string{ message }).c_str(),\
		"assertion failed!!!\r\n",\
		MB_OK);\
	while (true) { __debugbreak(); }\
}

//#define GB_ASSERT_SELECT(_FIRST, _SECOND, NAME, ...) NAME
//#define GB_ASSERT(...) GB_ASSERT_SELECT(__VA_ARGS__, GB_ASSERT_2, GB_ASSERT_1)(__VA_ARGS__)

//#pragma warning(push)
#pragma warning(disable: 4003) // 引数不足の警告を無効化

#define GB_ASSERT GB_ASSERT_2

//#pragma warning(pop)


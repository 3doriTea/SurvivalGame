#pragma once

// アサーション
#define GB_ASSERT(expression)\
if (!(expression))\
{\
	MessageBox(\
		NULL,\
		std::format("LastErrorCode:{}", GetLastError()).c_str(),\
		"assertion failed!!!",\
		MB_OK);\
	while (true) { __debugbreak(); }\
}

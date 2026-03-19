#pragma once

// 文字列
#include <string>
#include <string_view>
#include <sstream>
#include <regex>  // 正規表現

// 整数値
#include <cstdint>

// 型情報
#include <typeindex>
#include <typeinfo>

// コレクション
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <bitset>
#include <unordered_set>

// 関数型
#include <functional>

// ファイルシステム
#include <filesystem>

// ファイル
#include <fstream>

// フォーマット
#include <format>

// 時間系
#include <chrono>

// アサーション
//#include <cassert>

// デバイス入力系
#include <dinput.h>
#include <Xinput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "xinput.lib")

// Windowsアプリに必要
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WinMM.lib")

// JSON解析
#include "Library/nlohmann-json/json_fwd.hpp"
using json = nlohmann::json;

// YAML解析
#include <yaml-cpp/yaml.h>
//#ifdef _DEBUG
//#pragma comment(lib, "yaml-cppd.lib")
//#else
//#pragma comment(lib, "yaml-cpp.lib")
//#endif
// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Library/imgui/imgui.h"
#include "Library/imgui/imgui_impl_win32.h"
#include "Library/imgui/imgui_impl_dx11.h"

// DirectX11
#include <d3d11.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")

// シェーダコンパイラ
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

// テクスチャ
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

// ComPtr
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
namespace fs = std::filesystem;

#include <CoreType/Color.h>
#include <CoreType/Quaternion.h>
#include <CoreType/Vec2.h>
#include <CoreType/Vec2Int.h>
#include <CoreType/Vec3.h>
#include <CoreType/Vec3Int.h>
#include <CoreType/Float4.h>
#include <CoreType/Mat4x4.h>
#include <CoreType/Handle.h>
#include <CoreType/XMOperator.h>

#include <Utility/TimeUtil.h>
#include <Utility/StringUtil.h>
#include <Utility/FlyweightFactory.h>

#include <Debugger.h>

// 独自アサーション
#include "Standalone/Utility/Assert.h"

#include <Event.h>
#include <ResponderEvent.h>
#include <FluentVector.h>

#include "Constant/KeyCode.h"

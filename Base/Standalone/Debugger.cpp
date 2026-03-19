#include "Debugger.h"
#include <cassert>
#include <fstream>
#include <Windows.h>

namespace
{
	std::vector<std::string> scopeNameStack_{};
	std::stringstream scopeName_{};
	std::vector<std::string> logHistory_{};

	/// <summary>
	/// スコープ名を新しく結合する
	/// </summary>
	/// <param name="_name">結合するスコープ名</param>
	inline void ConcatScope(const std::string_view _name)
	{
		if (scopeName_.str().empty() == false)
		{
			scopeName_ << ".";
		}
		scopeName_ << _name;
	}
}

void GameBase::Debugger::LogBegin(const std::string_view _name)
{
	scopeNameStack_.push_back(_name.data());

	// 名前の区切りを入れる
	ConcatScope(_name);
}

void GameBase::Debugger::Log(const std::string_view _content)
{
	if (LoggingEnabled() == false)
	{
		/*
		* Debugger::LogBegin("空間名");
		* ↓
		* ==ログ出力はこの範囲内のみ有効です。==
		* ↑
		* Debugger::LogEnd();
		*/
		__debugbreak();
	}

	auto now{ std::chrono::system_clock::now() };
	logHistory_.emplace_back(std::format("@{}[{:%H:%M:%S}] {}", scopeName_.str(), now, _content));
	OutputDebugString(std::format("{}\r\n", logHistory_.back()).c_str());
}

void GameBase::Debugger::LogEnd()
{
	scopeNameStack_.pop_back();

	scopeName_.str("");
	for (const std::string_view name : scopeNameStack_)
	{
		ConcatScope(name);
	}
}

bool GameBase::Debugger::LoggingEnabled()
{
	// スコープ名スタックがあればログ有効
	return scopeNameStack_.size() > 0;
}

void GameBase::Debugger::LogWriteOutFile(const fs::path& _dir, const std::string_view _name)
{
	auto now{ std::chrono::system_clock::now() };
	std::ofstream ofs{ _dir / std::format("{}{:%H-%M-%S}.log", _name, now) };

	assert(ofs.is_open() && "ファイル作成に失敗");

	for (auto& log : logHistory_)
	{
		ofs << log << std::endl;
	}

	ofs.close();
}

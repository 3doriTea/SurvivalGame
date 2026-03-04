#pragma once

namespace GameBase
{
	namespace Debugger
	{
		/// <summary>
		/// ログ出力範囲を開始する
		/// </summary>
		/// <param name="_name">ログの範囲に名前をつける</param>
		void LogBegin(const std::string_view _name);

		/// <summary>
		/// ログをフォーマット付きで出力する
		/// </summary>
		/// <typeparam name="...Args">フォーマット引数</typeparam>
		/// <param name="_format">フォーマット文字列</param>
		/// <param name="..._args">フォーマットに指定した引数</param>
		template<typename ... Args>
		void LogF(std::format_string<Args...> _format, Args&& ... _args);

		/// <summary>
		/// ログを出力する
		/// </summary>
		/// <param name="_content">文字列</param>
		void Log(const std::string_view _content);

		/// <summary>
		/// ログ出力範囲を終了する
		/// </summary>
		void LogEnd();

		/// <summary>
		/// ログ出力範囲内であるか
		/// </summary>
		/// <returns>範囲内である true / false</returns>
		bool LoggingEnabled();
	}
}

template<typename ...Args>
inline void GameBase::Debugger::LogF(std::format_string<Args...> _format, Args && ..._args)
{
	if (LoggingEnabled())
	{
		Log(
			std::format(
				_format, std::forward<Args>(_args)
				)
			);
	}
}

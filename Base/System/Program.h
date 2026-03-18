#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: プログラム管理
	/// </summary>
	class IProgram : public ISystemInterfaceBase
	{
	public:
		IProgram() = default;
		virtual ~IProgram() = default;
	};

	/// <summary>
	/// システム: プログラム管理
	/// </summary>
	class Program : public SystemBase<Program, IProgram>
	{
	public:
		Program();
		~Program();

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		/// <summary>
		/// プログラムのハンドルを取得する
		/// </summary>
		/// <returns>プログラムのハンドル</returns>
		inline HINSTANCE GetInstance() const { return hInstance_; }

	private:
		HINSTANCE hInstance_;  // このプログラムのハンドル
	};
}

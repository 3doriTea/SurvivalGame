#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	struct IGameTime : ISystemInterfaceBase
	{
		IGameTime() = default;
		virtual ~IGameTime() = default;

		/// <summary>
		/// フレーム間時間(秒)を取得
		/// </summary>
		/// <returns>フレーム間時間(秒)</returns>
		virtual float DeltaTime() const = 0;

		/// <summary>
		/// フレーム更新のタイミングであるか
		/// </summary>
		/// <returns>フレーム更新タイミング true / false</returns>
		virtual bool IsFrameDue() const = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class GameTime : public SystemBase<GameTime, IGameTime>
	{
	public:
		GameTime();
		~GameTime();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		inline float DeltaTime() const override { return deltaTimeSec_; }
		inline bool IsFrameDue() const override { return isFrameDue_; }

	private:
		bool isFrameDue_;              // フレーム更新のタイミングであるか
		float deltaTimeSec_;           // フレーム間時間
		LARGE_INTEGER currentMicro_;   // 現在のCPU時間 (マイクロ秒)
		LARGE_INTEGER previousMicro_;  // 前回のCPU時間 (マイクロ秒)
		int fps_;                      // fps指定
	};
}

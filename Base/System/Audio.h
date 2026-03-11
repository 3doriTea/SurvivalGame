#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IAudio : public ISystemInterfaceBase
	{
	public:
		IAudio() = default;
		virtual ~IAudio() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class Audio : public SystemBase<Audio, IAudio>
	{
	public:
		Audio();
		~Audio();

		// 利用する参照があるときに使用
		//void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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

	private:
		ComPtr<IXAudio2>
	};
}

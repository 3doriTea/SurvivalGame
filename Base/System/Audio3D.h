#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 3D音響
	/// </summary>
	class IAudio3D : public ISystemInterfaceBase
	{
	public:
		IAudio3D() = default;
		virtual ~IAudio3D() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: 3D音響
	/// </summary>
	class Audio3D : public SystemBase<Audio3D, IAudio3D>
	{
	public:
		Audio3D();
		~Audio3D();

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
	};
}

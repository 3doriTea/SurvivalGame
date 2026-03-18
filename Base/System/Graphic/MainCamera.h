#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: メインカメラ
	/// </summary>
	class IMainCamera : public ISystemInterfaceBase
	{
	public:
		IMainCamera() = default;
		virtual ~IMainCamera() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: メインカメラ
	/// </summary>
	class MainCamera : public SystemBase<MainCamera, IMainCamera>
	{
	public:
		MainCamera();
		~MainCamera();

		// 利用する参照があるときに使用
		//void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

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

	private:
	};
}

#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: シェーダコンパイラ
	/// </summary>
	class IShaderCompiler : public ISystemInterfaceBase
	{
	public:
		IShaderCompiler() = default;
		virtual ~IShaderCompiler() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: シェーダコンパイラ
	/// </summary>
	class ShaderCompiler : public SystemBase<ShaderCompiler, IShaderCompiler>
	{
	public:
		ShaderCompiler();
		~ShaderCompiler();

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

	private:
	};
}

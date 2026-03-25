#pragma once
#include "../SystemBase.h"
#include <Structure/Shader.h>
#include <Structure/ShaderCompileConfig.h>


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

		/// <summary>
		/// シェーダを取得する
		/// </summary>
		/// <param name="_hShader">シェーダハンドル</param>
		/// <returns>シェーダの参照</returns>
		virtual Shader& At(const ShaderHandle _hShader) = 0;

		/// <summary>
		/// シェーダを読み込む
		/// </summary>
		/// <param name="_file">シェーダファイルのパス</param>
		/// <param name="_rasterizerConfig">ラスタライザせて血</param>
		/// <param name="_config">シェーダコンパイル設定</param>
		/// <returns></returns>
		virtual ShaderHandle Load(
			const fs::path& _file,
			const RasterizerConfig& _rasterizerConfig,
			const ShaderCompileConfig& _config) = 0;
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
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		Shader Compile(
			const fs::path& _file,
			const RasterizerConfig& _rasterizerConfig,
			const ShaderCompileConfig& _config);

		Shader& At(const ShaderHandle _hShader) override;

		ShaderHandle Load(
			const fs::path& _file,
			const RasterizerConfig& _rasterizerConfig,
			const ShaderCompileConfig& _config) override;

	private:
		FlyweightFactory<Shader, ShaderHandle> shaders_;
	};
}

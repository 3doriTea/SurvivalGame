#pragma once
#include "../SystemBase.h"
#include "../MaterialBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 環境情報のコンスタントバッファを管理
	/// </summary>
	class IEnvConstant : public ISystemInterfaceBase
	{
	public:
		IEnvConstant() = default;
		virtual ~IEnvConstant() = default;

		/// <summary>
		/// 環境情報のマテリアルのハンドルを取得
		/// </summary>
		/// <returns>マテリアルのハンドル</returns>
		virtual MaterialHandle GetEnvMaterialHandle() = 0;

		/// <summary>
		/// 環境定数バッファを更新、送信する
		/// </summary>
		/// <param name="_slotNumber">環境コンスタントバッファのスロット番号</param>
		/// <param name="_transformMatrix">座標変換行列</param>
		virtual void UpdateSendTo(const UINT _slotNumber, const Mat4x4& _transformMatrix, EntityRegistry& _registry) = 0;
	};

	/// <summary>
	/// システム: 環境情報のコンスタントバッファを管理
	/// </summary>
	class EnvConstant : public SystemBase<EnvConstant, IEnvConstant>, public MaterialBase
	{
		/// <summary>
		/// 環境定数
		/// </summary>
		struct Constant
		{
			Constant() :
				transformMatrix{ Mat4x4::Identity() },
				viewProjectionMatrix{ Mat4x4::Identity() },
				lightDirection{}
			{}

			Mat4x4 transformMatrix;  // 座標変換行列
			Mat4x4 viewProjectionMatrix;  // VP
			Vec3 lightDirection;  // 平行(光線/光源)の方向
		};

	public:
		EnvConstant();
		~EnvConstant();

		// ワールド実行情報を参照、このシステムを登録除外するかチェックする true を返すことで除外される
		//bool OnVerifyShouldSkip(const RunInfo& _info) override;

		// 他システムへの依存関係を登録する cppでincludeした他システムはここで登録
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		void UpdateSendTo(const UINT _slotNumber, const Mat4x4& _transformMatrix, EntityRegistry& _registry) override;

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

		MaterialHandle GetEnvMaterialHandle() override { return hEnvMaterial_; }

		inline size_t Size() const override { return sizeof(Constant); }
		inline ShaderHandle Shader() const override { return INVALID_HANDLE; }
		inline const void* GetPtr() const override { return &constant_; }

	private:
		Constant constant_;  // 環境定数
		MaterialHandle hEnvMaterial_;  // 環境マテリアル
	};
}

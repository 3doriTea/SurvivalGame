#pragma once
#include "../SystemBase.h"
#include "../Structure/Texture.h"
#include "../Structure/TextureLoadConfig.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class ITextureRegistry : public ISystemInterfaceBase
	{
	public:
		ITextureRegistry() = default;
		virtual ~ITextureRegistry() = default;

		/// <summary>
		/// テクスチャを読み込む
		/// </summary>
		/// <param name="_file">ファイルパス</param>
		/// <param name="_loadConfig">読み込み設定</param>
		/// <returns>テクスチャハンドル</returns>
		virtual TextureHandle Load(const fs::path& _file, const TextureLoadConfig& _loadConfig = {}) = 0;

		/// <summary>
		/// 読み込んだテクスチャの画像サイズを取得する
		/// </summary>
		/// <param name="_hTexture">テクスチャハンドル</param>
		/// <returns>画像サイズ(width, height)</returns>
		virtual Vec2Int GetImageSize(const TextureHandle _hTexture) const = 0;

		/// <summary>
		/// 読み込んだテクスチャを参照する
		/// </summary>
		/// <param name="_hTexture">テクスチャハンドル</param>
		/// <param name="_callback">参照するコールバックvoid(const Texture&)</param>
		virtual void RefAt(const TextureHandle _hTexture, std::function<void(const Texture&)> _callback) const = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class TextureRegistry : public SystemBase<TextureRegistry, ITextureRegistry>
	{
	public:
		TextureRegistry();
		~TextureRegistry();

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

		TextureHandle Load(const fs::path& _file, const TextureLoadConfig& _loadConfig = {}) override;
		Vec2Int GetImageSize(const TextureHandle _hTexture) const override;
		void RefAt(const TextureHandle _hTexture, std::function<void(const Texture&)> _callback) const override;

	private:
		/// <summary>
		/// テクスチャを読み込む 本体
		/// </summary>
		/// <param name="_file">テクスチャに使用するファイルのパス</param>
		/// <param name="_loadConfig">読み込み設定</param>
		/// <param name="_pOut">読み込んだテクスチャの出力先</param>
		/// <returns>読み込みに成功 true / false</returns>
		bool TryLoadTexture(const fs::path& _file, const TextureLoadConfig& _loadConfig, Texture* _pOut);

	private:
		FlyweightFactory<Texture, TextureHandle> registry_;

	};
}

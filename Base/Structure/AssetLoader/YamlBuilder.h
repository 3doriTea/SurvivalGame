#pragma once
#include "../SchemaLoadBundle.h"
#include "../Schema/GameComponentSchema.h"


namespace GameBase
{
	/// <summary>
	/// YAML生成するやつ
	/// </summary>
	class YamlBuilder
	{
	public:
		YamlBuilder(const SchemaLoadBundle& _loadBundle);
		~YamlBuilder();

		/// <summary>
		/// コンポーネントを書き込む
		/// </summary>
		/// <param name="component">コンポーネントスキーマ</param>
		YamlBuilder& Write(const Schema::GameComponent& _component);

		/// <summary>
		/// 文字列に書き出す
		/// </summary>
		/// <returns>文字列</returns>
		std::string Build();

	private:
		/// <summary>
		/// ヘッド情報
		/// </summary>
		void Header();

	private:
		const SchemaLoadBundle& LOAD_BUNDLE_;  // 外部,動的情報を参照
		std::stringstream out_;         // 書き込むss
	};
}

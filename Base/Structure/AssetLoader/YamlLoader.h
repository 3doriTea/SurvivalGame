#pragma once
#include "../IAssetLoader.h"

namespace GameBase
{
	/// <summary>
	/// YAMLファイルを読み込む
	/// </summary>
	struct YamlLoader : IAssetLoader
	{
		YamlLoader() = default;
		~YamlLoader() = default;

		/// <summary>
		/// ノードが読まれたときのコールバック処理
		/// </summary>
		/// <param name="_callback"> コールバック関数(const std::string& _tag, const std::string& _fileName, const YAML::Node& _node)</param>
		inline void OnNode(
			const std::function<void(
				const std::string&,
				const std::string&,
				const YAML::Node&)>& _callback) { callback = _callback; }

		/// <summary>
		/// 試しにアセットを読み込む
		/// </summary>
		/// <param name="_file">ファイルのパス</param>
		/// <returns>読み込めた true / false</returns>
		bool TryLoad(const fs::path& _file);

	private:
		std::function<void(const std::string&, const std::string&, const YAML::Node&)> callback;  // コールバック
	};
}

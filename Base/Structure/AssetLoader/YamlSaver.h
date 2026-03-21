#pragma once

namespace GameBase
{
	class YamlSaver
	{
	public:
		inline YamlSaver() = default;
		inline ~YamlSaver() = default;

		/// <summary>
		/// ノードを書き込む
		/// </summary>
		/// <param name="_callback">void(YAML::Emitter&)</param>
		/// <param name="_tag">--- ここのタグ名</param>
		void WriteNode(
			const std::function<void(YAML::Emitter&)>& _callback,
			const std::string_view _tag);

		/// <summary>
		/// 試しにYAMLを書き込む
		/// </summary>
		/// <param name="_file">ファイルのパス</param>
		/// <returns>書き込めた true / false</returns>
		bool TrySave(const fs::path& _file);

	private:
		std::stringstream stream_;
	};
}

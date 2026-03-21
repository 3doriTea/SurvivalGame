#pragma once

namespace GameBase
{
	using AssetType = int;

	/// <summary>
	/// アセットファイルの種類
	/// </summary>
	enum AssetType_ : AssetType
	{
		AssetType_UnknownFile,
		AssetType_UnknownFolder,
		AssetType_YamlScene,
		AssetType_Shader,
		AssetType_ShaderInclude,
		AssetType_CppHeader,
		AssetType_CppSource,
		AssetType_ModelFbx,
		AssetType_UpDirectory,
		AssetType_MAX,
	};

	static const std::map<GameBase::AssetType, std::string_view> FILE_NAMES
	{
		{ AssetType_UnknownFile, "UnknownFile.png" },
		{ AssetType_UnknownFolder, "Folder.png" },
		{ AssetType_CppHeader, "CppHeader.png" },
		{ AssetType_CppSource, "CppSource.png" },
		{ AssetType_YamlScene, "YamlScene.png" },
		{ AssetType_ModelFbx, "ModelFbx.png" },
		{ AssetType_UpDirectory, "Updir.png" },
	};

	static const std::map<std::string_view, GameBase::AssetType> EXT_TO_TYPE
	{
		{ ".cpp",        AssetType_CppSource },
		{ ".h",          AssetType_CppHeader },
		{ ".hpp",        AssetType_CppHeader },
		{ ".scene.yaml", AssetType_YamlScene },
		{ ".fbx",        AssetType_ModelFbx },
	};
}

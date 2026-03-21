#include "AssetGenerator.h"

GameBase::System::AssetGenerator::AssetGenerator()
{}

GameBase::System::AssetGenerator::~AssetGenerator()
{}

void GameBase::System::AssetGenerator::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>*_registry)
{}

void GameBase::System::AssetGenerator::Initialize()
{}

void GameBase::System::AssetGenerator::Update(EntityRegistry&)
{}

void GameBase::System::AssetGenerator::Release()
{}

bool GameBase::System::AssetGenerator::TryGenerate(const fs::path& _file, const AssetType _type)
{
	switch (_type)
	{
	case AssetType_YamlScene:
	{
		// テンプレート読み込み
		std::ifstream ifs{ "./Base/_Template/SceneTemplate.scene.yaml.txt" };
		if (!ifs)
		{
			GB_ASSERT(false && "ファイルの読み取りに失敗");
			return false;  // 失敗
		}

		std::stringstream ss{};
		ss << ifs.rdbuf();
		ifs.close();

		std::ofstream ofs{ _file };
		if (!ofs)
		{
			GB_ASSERT(false && "ファイルの作成に失敗");
			return false;  // 失敗
		}

		ofs << ss.rdbuf();

		ofs.close();

		return true;  // 成功
	}
	case AssetType_Shader:
	case AssetType_ShaderInclude:
	case AssetType_ModelFbx:
	default:
		break;
	}


	return false;
}

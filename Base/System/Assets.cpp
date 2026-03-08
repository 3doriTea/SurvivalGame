#include "Assets.h"


GameBase::System::Assets::Assets()
{
}

GameBase::System::Assets::~Assets()
{
}

void GameBase::System::Assets::Initialize()
{
	directory_ = fs::current_path() / "Assets";
	Load();
}

void GameBase::System::Assets::Update()
{
}

void GameBase::System::Assets::Release()
{
}

void GameBase::System::Assets::SetDirectoryAndLoad(const fs::path& _dir)
{
	directory_ = _dir;
}

void GameBase::System::Assets::Load()
{
#pragma region GameConfig.json 読み込み
	json gameConfigJson{ FetchJson("Settings/GameConfig.json") };
	gameConfig_ = GameConfig
	{
		.windowSize =
		{{
			.x = gameConfigJson.value("/Window/Width", 1600),
			.y = gameConfigJson.value("/Window/Height", 900),
		}},
		.fps = gameConfigJson.value("/Window/FPS", 60U),
		.vSync = gameConfigJson.value("/Window/VSync", false),
	};
#pragma endregion

#pragma region GameManifest.json 読み込み
	json gameManifestJson{ FetchJson("Settings/GameConfig.json") };
	gameManifest_ = GameManifest
	{
		.title = gameManifestJson.value("Title", "Sample Game"),
		.version =
		{{
			.x = gameManifestJson.value("/Version/Major", 1),
			.y = gameManifestJson.value("/Version/Minor", 0),
			.z = gameManifestJson.value("/Version/Patch", 0),
		}},
		.developer = gameManifestJson.value("Developer", ""),
		.copyright = gameManifestJson.value("Copyright", ""),
	};
#pragma endregion
}

json GameBase::System::Assets::FetchJson(const fs::path& _file)
{
	fs::path jsonFile{ directory_ / _file };
	if (fs::is_regular_file(jsonFile) == false)
	{
		assert(false && "JSONファイルが見つからなかった");
		return {};
	}

	std::ifstream ifs{ jsonFile };
	if (!ifs)
	{
		assert(false && "JSONファイルの読み込みに失敗");
		return {};
	}

	json j{};
	ifs >> j;

	return std::move(j);
}

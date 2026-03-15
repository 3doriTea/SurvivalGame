#include "pch.h"
#include "YamlLoader.h"

bool GameBase::YamlLoader::TryLoad(const fs::path& _file)
{
	Debugger::LogBegin("YAML-Loader");
	try
	{
		YAML::Node test{ YAML::LoadFile(_file.string()) };

		std::vector<YAML::Node> docs{ YAML::LoadAllFromFile(_file.string()) };

		for (const YAML::Node& node : docs)
		{
			const std::string_view TAG{ node.Tag() };
			std::stringstream name{};

			name << TAG;

			/*if (const std::string_view TAG_REP{ "!gb!" };
				StringUtil::StartWith(TAG, TAG_REP))
			{
				name << "class GameBase::";
				name << TAG.substr(TAG_REP.size());
			}
			else if (const std::string_view TAG_REP{ "!gbc!" };
				StringUtil::StartWith(TAG, TAG_REP))
			{
				name << "struct GameBase::Component::";
				name << TAG.substr(TAG_REP.size());
			}
			else
			{
				assert(false && "Unknown TAG");
				return false;
			}*/
			callback(name.str(), _file.stem().string(), node);
		}
	}
	catch (const YAML::Exception& ex)
	{
		Debugger::LogF("YAML Load Error:{}", ex.what());
		Debugger::LogEnd();
		return false;
	}

	Debugger::LogEnd();
	return true;
}

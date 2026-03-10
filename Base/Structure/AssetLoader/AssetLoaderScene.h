#pragma once
#include "../IAssetLoader.h"


namespace GameBase
{
	struct AssetLoaderScene : IAssetLoader
	{
		inline bool TryLoad(const fs::path& _file) override
		{
			Debugger::LogBegin("YAML-LoaderScene");
			try
			{
				std::vector<YAML::Node> docs{ YAML::LoadAllFromFile(_file.string()) };

				for (const YAML::Node node : docs)
				{
					const std::string_view TAG{ node.Tag() };

					if (const std::string_view TAG_REP{ "!gb!" };
						StringUtil::StartWith(TAG, TAG_REP))
					{
						std::stringstream name{ "GameBase::" };
						name << TAG.substr(TAG_REP.size());
					}
					else if (const std::string_view TAG_REP{ "!gbc!" };
						StringUtil::StartWith(TAG, TAG_REP))
					{
						std::stringstream name{ "GameBase::Component::" };
						name << TAG.substr(TAG_REP.size());
					}
					else
					{
						assert(false && "Unknown TAG");
						return false;
					}
				}
				//YAML::Load("[1, 2, 3]");
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
	};
}

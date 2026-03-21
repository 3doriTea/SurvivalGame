#include "YamlSaver.h"

void GameBase::YamlSaver::WriteNode(
	const std::function<void(YAML::Emitter&)>& _callback,
	const std::string_view _tag)
{
	YAML::Emitter emitter{};
	_callback(emitter);
	stream_ << "--- " << _tag << std::endl
	<< emitter.c_str() << std::endl << std::endl;
}

bool GameBase::YamlSaver::TrySave(const fs::path& _file)
{
	std::ofstream ofs{ _file };
	if (!ofs)
	{
		return false;  // ファイル開けなかった
	}

	ofs << "%YAML 1.1" << std::endl;
	ofs << "%TAG !gbc! GameBase::Component::" << std::endl;
	ofs << "%TAG !gb! GameBase::" << std::endl << std::endl;

	ofs << stream_.rdbuf();

	ofs.close();

	return true;
}

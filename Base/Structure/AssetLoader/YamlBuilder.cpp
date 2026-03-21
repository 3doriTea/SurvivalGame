#include "YamlBuilder.h"
#include <yaml-cpp/emitter.h>
#include "../../ECS/IComponentBase.h"


GameBase::YamlBuilder::YamlBuilder(const SchemaLoadBundle& _loadBundle) :
	LOAD_BUNDLE_{ _loadBundle }
{
	//out_ << YAML::BeginDoc;
	Header();
}

GameBase::YamlBuilder::~YamlBuilder()
{}

GameBase::YamlBuilder& GameBase::YamlBuilder::Write(const Schema::GameComponent& _component)
{
	std::string yamlTag{ std::regex_replace(_component.tag, std::regex{ "GameBase::Component::" }, "!gbc!") };
	std::string componentName{ yamlTag.substr(5) };  // !gbc!を飛ばす

	std::string node{ YAML::Dump(_component.node) };

	out_ << "---" << " " << yamlTag << std::endl;
	YAML::Emitter outY{};
	outY << YAML::BeginMap;
	outY << YAML::Key << "Id" << YAML::Value << _component.self;
	outY << YAML::Key << componentName << YAML::Value << _component.node;
	outY << YAML::EndMap;

	out_ << outY.c_str();

	return *this;
}

std::string GameBase::YamlBuilder::Build()
{
	//out_ << YAML::EndDoc;

	std::stringstream ss{};
	ss << "%YAML 1.1" << std::endl;
	ss << "%TAG !gbc! GameBase::Component::" << std::endl;
	ss << "%TAG !gb! GameBase::" << std::endl;
	ss << out_.str();

	return ss.str();
}

void GameBase::YamlBuilder::Header()
{
	//out_ << YAML::BeginDoc;
}

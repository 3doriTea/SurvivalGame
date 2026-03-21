#include "Component.h"
#include <Component/GameObject.h>
#include <Component/Transform.h>


GameBase::Editor::Inspector::Component::Component(
	const Entity _entity,
	const ComponentIndex _componentIndex) :
	ENTITY{ _entity },
	COMPONENT_INDEX{ _componentIndex }
{
}

bool GameBase::Editor::Inspector::Component::IsUpdatedShow(
	EntityRegistry& _registry,
	std::string& _yamlStr,
	const SchemaLoadBundle& _loadBundle)
{
	//schema.node
	//ImGui::Selectable(build.c_str());
	static char text[4096]{};
	memset(text, 0, sizeof(text));
	memcpy(text, _yamlStr.c_str(), _yamlStr.size());

	std::vector<YAML::Node> docs{ YAML::LoadAll(_yamlStr.c_str()) };

	static const std::string_view COMPONENT_HEAD{ "GameBase::Component::" };

	bool isUpdated{ false };

	std::string componentTag{};
	for (const YAML::Node& node : docs)
	{
		componentTag = node.Tag();
		if (StringUtil::StartWith(componentTag, COMPONENT_HEAD))
		{
			std::string componentName{ componentTag.substr(COMPONENT_HEAD.size()) };

			try
			{
				const YAML::Node& componentNode{ node[componentName] };
				bool isChanged{ false };
				YAML::Emitter updated{};
				updated << YAML::BeginMap;

				for (auto itr = node[componentName].begin(); itr != node[componentName].end(); itr++)
				{
					std::string paramKey{ itr->first.as<std::string>() };
					updated << YAML::Key << paramKey;

					const YAML::Node params{ itr->second };


					if (params.IsScalar())
					{
						try
						{
							float v{ params.as<float>() };
							if (ImGui::DragFloat(paramKey.c_str(), &v, 0.1f))
							{
								isChanged = true;  // 変更あった
							}
							updated << YAML::Value << v;
						}
						catch (const YAML::BadConversion&)
						{
							try
							{
								std::string str{ params.as<std::string>() };
								static std::array<char, 1024> buffer{};

								buffer.fill('\0');
								std::copy(str.begin(), str.end(), buffer.begin());

								if (ImGui::InputText(paramKey.c_str(), buffer.data(), buffer.size()))
								{
									str = buffer.data();
									isChanged = true; // 変更があった
								}
								updated << YAML::Value << str;
							}
							catch (const YAML::Exception& ex)
							{
								GB_ASSERT(false && "変換失敗",
									std::format("reason:{}", ex.what()));
							}
						}
					}
					else if (params.IsMap())
					{
						if (HasKeys(params, { "x", "y", "z" }))
						{
							Vec3 v3
							{
								params["x"].as<float>(),
								params["y"].as<float>(),
								params["z"].as<float>(),
							};
							if (ImGui::DragFloat3(paramKey.c_str(), v3.v, 0.1f))
							{
								isChanged = true;  // 変更がアタ
							}
							updated << YAML::Value << YAML::BeginMap;
							updated << YAML::Key << "x" << YAML::Value << v3.x;
							updated << YAML::Key << "y" << YAML::Value << v3.y;
							updated << YAML::Key << "z" << YAML::Value << v3.z;
							updated << YAML::EndMap;
						}
						else if (HasKeys(params, { "fileId" }))
						{
							int selectedIndex{};
							Entity selectedEntity{ params["fileId"].as<Schema::FileId>() };
							auto view{ _registry.GetView<GameBase::Component::GameObject>() };
							auto [ selectedGameObject ]{ view.At(selectedEntity) };
							std::string selectedName{};
							if (selectedEntity == INVALID_ENTITY)
							{
								selectedName = "[>未選択<]";
							}
							else
							{
								selectedName = selectedGameObject.name.data();
							}
							if (ImGui::BeginCombo(paramKey.c_str(), selectedName.c_str()))
							{
								view.ForEach([&selectedIndex, &selectedEntity, &isChanged, &_registry, this](const Entity _entity, GameBase::Component::GameObject& _gameObject)
								{
									if (_entity == ENTITY)
									{
										return;  // 自分自身は選択させない
									}

									auto view{ _registry.GetView<GameBase::Component::Transform>() };
									Entity at{ _entity };
									while (at != INVALID_ENTITY &&
										_registry.HasComponent(
											at,
											ComponentRegistry::GetComponentIndex<GameBase::Component::Transform>()))
									{
										if (at == ENTITY)
										{
											return;  // 自分自身が親にいるなら、選択できない
										}

										auto [t]{ view.At(at)};
										at = t.parent;
									}

									bool isSelected{ selectedEntity == _entity };
									if (ImGui::Selectable(_gameObject.name.data(), isSelected))
									{
										selectedEntity = _entity;
										isChanged = true;
									}

									if (isSelected)  // 選択項目としてフォーカス
									{
										ImGui::SetItemDefaultFocus();
									}
								});

								if (selectedEntity == INVALID_ENTITY)
								{
									if (ImGui::Selectable(selectedName.c_str(), true))
									{
									}
								}

								ImGui::EndCombo();
							}
							updated << YAML::Value << YAML::BeginMap;
							updated << YAML::Key << "fileId" << YAML::Value << selectedEntity;
							updated << YAML::EndMap;
						}
					}
				}

				if (isChanged)  // 変更があった
				{
					YAML::Node undatedNode{ YAML::Load(updated.c_str()) };
					std::string t{ YAML::Dump(undatedNode) };

					_registry.GetComponent(ENTITY, COMPONENT_INDEX).OnLoad(undatedNode, _loadBundle);
					isUpdated = true;
				}
			}
			catch (const YAML::Exception& ex)
			{
				GB_ASSERT(false && "コンポーネントパースに失敗",
					std::format("reason={}", ex.what()));
			}
		}
	}

	ImGui::PushStyleColor(ImGuiCol_FrameBg, {});
	ImGui::PushStyleColor(ImGuiCol_Border, {});
	ImGui::InputTextMultiline(std::format("##{}_{}", ENTITY, COMPONENT_INDEX).c_str(), text, sizeof(text),
		ImVec2{ -FLT_MIN, ImGui::GetTextLineHeight() * 10 }, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleColor(2);

	return isUpdated;
}

bool GameBase::Editor::Inspector::Component::HasKeys(
	const YAML::Node& _node,
	const std::vector<std::string>& _keys) const
{
	if (!_node)
	{
		return false;  // そもそもないなら違う
	}
	if (!_node.IsMap())
	{
		return false;  // そもそもmapじゃないなら違う
	}
	for (const std::string& key : _keys)
	{
		if (!_node[key])
		{
			return false;  // 含まれていないキーがあるなら違う
		}
	}
	return true;
}

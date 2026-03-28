#pragma once
#include "../../System/Renderer.h"

namespace GameBase
{
	/// <summary>
	/// 実行モード: ゲーム
	/// </summary>
	struct GamePipeLine : System::Renderer::IPipeLine
	{
		// IPipeLine を介して継承されました
		void Render(System::Renderer& _self, EntityRegistry& _registry) override;
	};
}

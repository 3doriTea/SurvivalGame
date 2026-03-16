#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: 
	/// </summary>
	class IEditorBase : public ISystemInterfaceBase
	{
	public:
		IEditorBase() = default;
		virtual ~IEditorBase() = default;

		//virtual void Hoge() = 0;
	};

	/// <summary>
	/// システム: 
	/// </summary>
	class EditorBase : public SystemBase<EditorBase, IEditorBase>
	{
	public:
		EditorBase();
		~EditorBase();

		// 利用する参照があるときに使用
		void OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry) override;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

	private:
		std::shared_ptr<void> onGUIEvent_;
	};
}

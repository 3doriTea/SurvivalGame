#pragma once


namespace GameBase::Editor
{
	class IEditorView;

	/// <summary>
	/// エディタ上で選択中のイベントクラス
	/// </summary>
	class SelectedEvent
	{
	public:
		inline SelectedEvent() :
			pEditorView_{},
			typeIndex_{ typeid(void) }
		{}
		inline ~SelectedEvent() = default;

		/// <summary>
		/// 試しにイベントを取得する
		/// </summary>
		/// <typeparam name="T">エディタビュア型</typeparam>
		/// <param name="_callback">取得時のコールバック関数</param>
		/// <returns>取得できた true / false</returns>
		template<typename T>
		inline bool TryGetEvent(const std::function<void(T&)>& _callback = [](T&){})
		{
			if (std::type_index{ typeid(T) } != typeIndex_)
			{
				return false;
			}

			auto spIntafece{ pEditorView_.lock() };
			auto sp{ std::static_pointer_cast<T>(spIntafece) };
			_callback(*sp.get());
			/*pEditorView_.reset();
			typeIndex_ = typeid(void);*/

			return true;
		}

		inline void Update(std::weak_ptr<IEditorView>&& _pEditorView, const std::type_index _typeIndex)
		{
			pEditorView_ = std::move(_pEditorView);
			typeIndex_ = _typeIndex;//std::type_index{ typeid(T) };
		}

	private:
		std::type_index typeIndex_;               // ビューの型情報
		std::weak_ptr<IEditorView> pEditorView_;  // イベント (nullable)
	};
}

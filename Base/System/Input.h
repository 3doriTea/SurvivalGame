#pragma once
#include "../SystemBase.h"


namespace GameBase::System
{
	/// <summary>
	/// システムインタフェース: デバイス入力
	/// </summary>
	class IInput : public ISystemInterfaceBase
	{
	public:
		IInput() = default;
		virtual ~IInput() = default;

		/// <summary>
		/// 押されている間か
		/// </summary>
		/// <param name="_keyCode">キーボードのキー</param>
		/// <returns>押されている true / false</returns>
		virtual bool IsPress(const KeyCode _keyCode) = 0;
		/// <summary>
		/// 押された瞬間のフレームか
		/// </summary>
		/// <param name="_keyCode">キーボードのキー</param>
		/// <returns>押された瞬間 true / false</returns>
		virtual bool IsPressDown(const KeyCode _keyCode) = 0;
		/// <summary>
		/// 離された瞬間のフレームか true / false
		/// </summary>
		/// <param name="_keyCode">キーボードのキー</param>
		/// <returns>離された瞬間 true / false</returns>
		virtual bool IsPressUp(const KeyCode _keyCode) = 0;
	};

	/// <summary>
	/// システム: デバイス入力
	/// </summary>
	class Input : public SystemBase<Input, IInput>
	{
		using KeyBoardStateArray = std::array<BYTE, KEY_BOARD_STATE_MAX>;
	public:
		Input();
		~Input();

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

		bool IsPress(const KeyCode _keyCode) override;
		bool IsPressDown(const KeyCode _keyCode) override;
		bool IsPressUp(const KeyCode _keyCode) override;
	private:
		struct
		{
			// 現フレームでのキーボード状態
			KeyBoardStateArray stateCurr_;
			// 前フレームでのキーボード状態
			KeyBoardStateArray statePrev_;
		} keyBoard;  // キーボードデバイスの情報

		struct
		{
			// 現フレームでのマウス座標
			Vec2Int positionCurr_;
			// 前フレームでのマウス座標
			Vec2Int positionPrev_;
			//DIMOUSESTATE
		} mouse;  // マウスデバイスの情報
	};
}

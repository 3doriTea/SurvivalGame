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
		void Update(EntityRegistry&) override;

		/// <summary>
		/// 終了処理
		/// </summary>
		void Release() override;

		bool IsPress(const KeyCode _keyCode) override;
		bool IsPressDown(const KeyCode _keyCode) override;
		bool IsPressUp(const KeyCode _keyCode) override;
	private:
		// Direct Input 使ういんたフェース
		ComPtr<IDirectInput8> pDirectInput_;

		struct
		{
			// デバイスインタフェースインタフェース
			ComPtr<IDirectInputDevice8> pDevice;

			// 現フレームでのキーボード状態
			KeyBoardStateArray stateCurr;
			// 前フレームでのキーボード状態
			KeyBoardStateArray statePrev;
		} keyBoard_;  // キーボードデバイスの情報

		struct
		{
			// デバイスインタフェースインタフェース
			ComPtr<IDirectInputDevice8> pDevice;

			// 現フレームでのマウス座標
			Vec2Int positionCurr;
			// 前フレームでのマウス座標
			Vec2Int positionPrev;
			// 現フレームでのマウス入力
			DIMOUSESTATE stateCurr;
			// 前フレームでのマウス入力
			DIMOUSESTATE statePrev;
			// 現フレームでのマウス座標
			Vec2Int mousePositionCurr;
			// 前フレームでのマウス座標
			Vec2Int mousePositionPrev;
		} mouse_;  // マウスデバイスの情報

		struct
		{
			// 現フレームでのゲームパッド状態
			XINPUT_STATE padStateCurr;
			// 前フレームでのゲームパッド状態
			XINPUT_STATE padStatePrev;
		} controller_;  // ゲームコントローラ ゲームパッド
	};
}

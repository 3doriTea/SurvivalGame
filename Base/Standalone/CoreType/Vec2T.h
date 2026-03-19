#pragma once
#include <cmath>

namespace GameBase
{
	template<typename T, typename DirectXMathT, typename BIGT>
	struct Vec2T
	{
		struct Vec2TData { T x; T y; };

		enum
		{
			AT_X,
			AT_Y,
			AT_MAX,
		};

		inline ~Vec2T() = default;

		inline Vec2T(Vec2TData _in) :
			Vec2T{ _in.x, _in.y }
		{
		}

		inline Vec2T() :
			Vec2T{ 0, 0 }
		{}

		inline Vec2T(const T* _pV) :
			Vec2T{ _pV[AT_X], _pV[AT_Y] }
		{}

		inline Vec2T(BIGT* _pV) :
			Vec2T
		{
			static_cast<T>(_pV[AT_X]),
			static_cast<T>(_pV[AT_Y])
		}
		{}

		template<typename U>
		inline Vec2T(U _x, U _y ) :
			x{ static_cast<T>(_x) },
			y{ static_cast<T>(_y) }
		{
		}

		template<typename U, typename DirectXMathU, typename BIGT>
		inline Vec2T(const Vec2T<U, DirectXMathU, BIGT>& _other) :
			Vec2T{ _other.x, _other.y }
		{
		}

		template<typename U, typename DirectXMathU, typename BIGT>
		inline Vec2T& operator=(const Vec2T<U, DirectXMathU, BIGT>& _other)
		{
			if (this != &_other)
			{
				this->x = _other.x;
				this->y = _other.y;
			}
			return *this;
		}

		template<typename U, typename DirectXMathU>
		inline Vec2T(Vec2T<U, DirectXMathU, BIGT>&& _other) noexcept :
			Vec2T{ _other.x, _other.y }
		{
			_other.x = 0;
			_other.y = 0;
		}

		template<typename U, typename DirectXMathU, typename BIGT>
		inline Vec2T& operator=(Vec2T<U, DirectXMathU, BIGT>&& _other) noexcept
		{
			if (this != &_other)
			{
				this->x = _other.x;
				this->y = _other.y;

				_other.x = 0;
				_other.y = 0;
			}
			return *this;
		}

		union
		{
			struct
			{
				T x;
				T y;
			};
			T v[AT_MAX];
			DirectXMathT xm;
		};

#pragma region 便利系
		/// <summary>
		/// ベクトルの長さを求める
		/// </summary>
		/// <returns>長さ float</returns>
		float Length() const
		{
			return std::sqrtf(
				static_cast<float>(x) * static_cast<float>(x)
				+ static_cast<float>(y) * static_cast<float>(y)
			);
		};

		/// <summary>
		/// 正規化したベクトルを求める
		/// </summary>
		/// <returns>長さが1のベクトル</returns>
		Vec2T Normalize() const
		{
			float length{ Length() };
			if (length > 0.0f)
			{
				return *this / length;
			}
			else
			{
				return Vec2T{};
			}
		}
#pragma endregion

#pragma region 四則演算
		template<typename U, typename DirectXMathU, typename BIGT>
		inline Vec2T& operator+=(const Vec2T<U, DirectXMathU, BIGT> _other)
		{
			this->x += static_cast<T>(_other.x);
			this->y += static_cast<T>(_other.y);
			return *this;
		}
		template<typename U, typename DirectXMathU, typename BIGT>
		inline Vec2T& operator-=(const Vec2T<U, DirectXMathU, BIGT> _other)
		{
			this->x -= static_cast<T>(_other.x);
			this->y -= static_cast<T>(_other.y);
			return *this;
		}
		template<typename U>
		inline Vec2T& operator*=(const U _scalar)
		{
			this->x *= static_cast<T>(_scalar);
			this->y *= static_cast<T>(_scalar);
			return *this;
		}
		template<typename U>
		inline Vec2T& operator/=(const U _scalar)
		{
			this->x /= static_cast<T>(_scalar);
			this->y /= static_cast<T>(_scalar);
			return *this;
		}
		Vec2T operator-() const { return Vec2T{ -x, -y }; }
#pragma endregion

#pragma region 定数
		inline static Vec2T Zero()  { return Vec2T{ T{  0 }, T{  0 } }; }
		inline static Vec2T One()   { return Vec2T{ T{  1 }, T{  1 } }; }
		inline static Vec2T Right() { return Vec2T{ T{  1 }, T{  0 } }; }
		inline static Vec2T Left()  { return Vec2T{ T{ -1 }, T{  0 } }; }
		inline static Vec2T Up()    { return Vec2T{ T{  0 }, T{  1 } }; }
		inline static Vec2T Down()  { return Vec2T{ T{  0 }, T{ -1 } }; }
#pragma endregion
	};

#pragma region フレンド四則演算関数
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator+(Vec2T<T, DirectXMathT, BIGT> _left, const Vec2T<T, DirectXMathT, BIGT> _right)
	{
		return _left += _right;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator-(Vec2T<T, DirectXMathT, BIGT> _left, const Vec2T<T, DirectXMathT, BIGT> _right)
	{
		return _left -= _right;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator*(Vec2T<T, DirectXMathT, BIGT> _left, const T _right)
	{
		return _left *= _right;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator*(const T _left, const Vec2T<T, DirectXMathT, BIGT> _right)
	{
		return _right *= _left;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator/(Vec2T<T, DirectXMathT, BIGT> _left, const T _right)
	{
		return _left /= _right;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline Vec2T<T, DirectXMathT, BIGT> operator/(const T _left, const Vec2T<T, DirectXMathT, BIGT> _right)
	{
		return _right /= _left;
	}
#pragma endregion

#pragma region フレンド比較演算子
	template<typename T, typename DirectXMathT, typename BIGT>
	inline bool operator==(const Vec2T<T, DirectXMathT, BIGT> _a, const Vec2T<T, DirectXMathT, BIGT> _b)
	{
		return _a.x == _b.x && _a.y == _b.y;
	}
	template<typename T, typename DirectXMathT, typename BIGT>
	inline bool operator!=(const Vec2T<T, DirectXMathT, BIGT> _a, const Vec2T<T, DirectXMathT, BIGT> _b)
	{
		return !(_a == _b);
	}
#pragma endregion
}

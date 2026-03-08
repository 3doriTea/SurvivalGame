#pragma once
#include <cmath>

namespace GameBase
{
	template<typename T, typename DirectXMathT>
	struct Vec3T
	{
		struct Vec3TData { T x; T y; T z; };

		enum
		{
			AT_X,
			AT_Y,
			AT_Z,
			AT_MAX,
		};

		inline ~Vec3T() = default;

		inline Vec3T(Vec3TData _in) :
			Vec3T{ _in.x, _in.y, _in.z }
		{
		}

		inline Vec3T() :
			Vec3T{ 0, 0, 0 }
		{
		}

		template<typename U>
		inline Vec3T(U _x, U _y, U _z) :
			x{ static_cast<T>(_x) },
			y{ static_cast<T>(_y) },
			z{ static_cast<T>(_z) }
		{
		}

		template<typename U, typename DirectXMathU>
		inline Vec3T(const Vec3T<U, DirectXMathU>& _other) :
			Vec3T{ _other.x, _other.y, _other.z }
		{
		}

		template<typename U, typename DirectXMathU>
		inline Vec3T& operator=(const Vec3T<U, DirectXMathU>& _other)
		{
			if (this != &_other)
			{
				this->x = _other.x;
				this->y = _other.y;
				this->z = _other.z;
			}
			return *this;
		}

		template<typename U, typename DirectXMathU>
		inline Vec3T(const Vec3T<U, DirectXMathU>&& _other) noexcept :
			Vec3T{ _other.x, _other.y, _other.z }
		{
			_other.x = 0;
			_other.y = 0;
			_other.z = 0;
		}

		template<typename U, typename DirectXMathU>
		inline Vec3T& operator=(Vec3T<U, DirectXMathU>&& _other) noexcept
		{
			if (this != &_other)
			{
				this->x = _other.x;
				this->y = _other.y;
				this->z = _other.z;

				_other.x = 0;
				_other.y = 0;
				_other.z = 0;
			}
			return *this;
		}

		union
		{
			struct
			{
				T x;
				T y;
				T z;
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
				+ static_cast<float>(z) * static_cast<float>(z)
			);
		};

		/// <summary>
		/// 正規化したベクトルを求める
		/// </summary>
		/// <returns>長さが1のベクトル</returns>
		Vec3T Normalize() const
		{
			float length{ Length() };
			if (length > 0.0f)
			{
				return *this / length;
			}
			else
			{
				return Vec3T{};
			}
		}
#pragma endregion

#pragma region 四則演算
		template<typename U, typename DirectXMathU>
		inline Vec3T& operator+=(const Vec3T<U, DirectXMathU> _other)
		{
			this->x += static_cast<T>(_other.x);
			this->y += static_cast<T>(_other.y);
			this->z += static_cast<T>(_other.z);
			return *this;
		}
		template<typename U, typename DirectXMathU>
		inline Vec3T& operator-=(const Vec3T<U, DirectXMathU> _other)
		{
			this->x -= static_cast<T>(_other.x);
			this->y -= static_cast<T>(_other.y);
			this->z -= static_cast<T>(_other.z);
			return *this;
		}
		template<typename U>
		inline Vec3T& operator*=(const U _scalar)
		{
			this->x *= static_cast<T>(_scalar);
			this->y *= static_cast<T>(_scalar);
			this->z *= static_cast<T>(_scalar);
			return *this;
		}
		template<typename U>
		inline Vec3T& operator/=(const U _scalar)
		{
			this->x /= static_cast<T>(_scalar);
			this->y /= static_cast<T>(_scalar);
			this->z /= static_cast<T>(_scalar);
			return *this;
		}
		Vec3T operator-() const { return Vec3T{ -x, -y, -z }; }
#pragma endregion

#pragma region 定数
		inline static Vec3T Zero()     { return Vec3T{ T{  0 }, T{  0 }, T{  0 } }; }
		inline static Vec3T One()      { return Vec3T{ T{  1 }, T{  1 }, T{  0 } }; }
		inline static Vec3T Right()    { return Vec3T{ T{  1 }, T{  0 }, T{  0 } }; }
		inline static Vec3T Left()     { return Vec3T{ T{ -1 }, T{  0 }, T{  0 } }; }
		inline static Vec3T Up()       { return Vec3T{ T{  0 }, T{  1 }, T{  0 } }; }
		inline static Vec3T Down()     { return Vec3T{ T{  0 }, T{ -1 }, T{  0 } }; }
		inline static Vec3T Forward()  { return Vec3T{ T{  0 }, T{  0 }, T{  1 } }; }
		inline static Vec3T Back()     { return Vec3T{ T{  0 }, T{  0 }, T{ -1 } }; }
#pragma endregion
	};

#pragma region フレンド四則演算関数
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator+(Vec3T<T, DirectXMathT> _left, const Vec3T<T, DirectXMathT> _right)
	{
		return _left += _right;
	}
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator-(Vec3T<T, DirectXMathT> _left, const Vec3T<T, DirectXMathT> _right)
	{
		return _left -= _right;
	}
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator*(Vec3T<T, DirectXMathT> _left, const T _right)
	{
		return _left *= _right;
	}
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator*(const T _left, const Vec3T<T, DirectXMathT> _right)
	{
		return _right *= _left;
	}
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator/(Vec3T<T, DirectXMathT> _left, const T _right)
	{
		return _left /= _right;
	}
	template<typename T, typename DirectXMathT>
	inline Vec3T<T, DirectXMathT> operator/(const T _left, const Vec3T<T, DirectXMathT> _right)
	{
		return _right /= _left;
	}
#pragma endregion

#pragma region フレンド比較演算子
	template<typename T, typename DirectXMathT>
	inline bool operator==(const Vec3T<T, DirectXMathT> _a, const Vec3T<T, DirectXMathT> _b)
	{
		return _a.x == _b.x && _a.y == _b.y && _a.z == _b.z;
	}
	template<typename T, typename DirectXMathT>
	inline bool operator!=(const Vec3T<T, DirectXMathT> _a, const Vec3T<T, DirectXMathT> _b)
	{
		return !(_a == _b);
	}
#pragma endregion
}

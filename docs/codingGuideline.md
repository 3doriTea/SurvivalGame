# コーディング規約

## 命名規則

|種類|スタイル|スタイル見本|
|-|-|-|
|マクロ|スクリーミング|FOO_BAR|
|ローカル定数|スクリーミング|FOO_BAR|
|グローバル定数|スクリーミング|FOO_BAR|
|静的メンバ定数|スクリーミング|FOO_BAR|
|構造体メンバ定数|スクリーミング|FOO_BAR|
|クラスメンバ定数|スクリーミング 末尾にアンダースコア|FOO_BAR_|
|ローカル変数|ロワーキャメル|fooBar|
|構造体メンバ変数|ロワーキャメル|fooBar|
|仮引数|ロワーキャメル 先頭にアンダースコア|_fooBar|
|クラスメンバ変数|ロワーキャメル 末尾にアンダースコア|fooBar_|
|静的メンバ変数|ロワーキャメル 末尾にアンダースコア|fooBar_|
|名前空間|アッパーキャメル|FooBar|
|メンバ関数|アッパーキャメル|FooBar|
|静的メンバ関数|アッパーキャメル|FooBar|
|クラス|アッパーキャメル|FooBar|
|構造体|アッパーキャメル|FooBar|
|型エイリアス|アッパーキャメル|FooBar|
|型パラメータ|アッパーキャメル|FooBar|
|ファイル名|アッパーキャメル|FooBar|
|フォルダ名|アッパーキャメル|FooBar|

Example.h
```cpp
#define DEBUG_MODE 1

static const PLAYER_HP{ 100 };

struct Status
{
    Status(const int _hitPoint, const int _hitPointMax);
    ~Status() = default;

    int hitPoint;
    const int HIT_POINT_MAX;
};

class Player
{
public:
    Player(const int _myId);
    ~Player() = default;

private:
    Status myStatus_;
    const int MY_ID_;
};

```
Example.cpp
```cpp

```

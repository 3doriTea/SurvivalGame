#include "GameTime.h"
#include "Assets.h"


GameBase::System::GameTime::GameTime() :
	isFrameDue_{},
	deltaTimeSec_{},
	currentMicro_{},
	previousMicro_{},
	fps_{}
{
}

GameBase::System::GameTime::~GameTime()
{
}

void GameBase::System::GameTime::OnRegisterDependencies(FluentVectorAddOnly<SystemIndex>* _registry)
{
	_registry->Add(SystemRegistry::GetSystemIndex<Assets>());
}

void GameBase::System::GameTime::Initialize()
{
	MMRESULT result{};
	result = timeBeginPeriod(Util::PERIOD_MILLI);
	GB_ASSERT(result == TIMERR_NOERROR
		&& "時間解像度の開始に失敗");

	BOOL succeed{};
	succeed = QueryPerformanceCounter(&previousMicro_);
	GB_ASSERT(succeed && "CPU時間の取得に失敗");

	fps_ = Get<Assets>().GetGameConfig().fps;
}

void GameBase::System::GameTime::Update(EntityRegistry&)
{
	BOOL succeed{};
	succeed = QueryPerformanceCounter(&currentMicro_);
	GB_ASSERT(succeed && "CPU時間の取得に失敗");

	if (succeed == FALSE)
	{
		return;
	}

	// 前のフレームでのCPU時間との秒差
	const LONGLONG DIFF{ currentMicro_.QuadPart - previousMicro_.QuadPart };

	// 差をFPS倍して1秒を超える == 差が 1 / FPS ならフレーム更新タイミング
	isFrameDue_ = (DIFF * fps_ >= Util::ONE_SEC_TO_MICRO);

	if (isFrameDue_)
	{
		// DeltaTime更新
		deltaTimeSec_ = static_cast<float>(DIFF) * Util::MICRO_TO_SEC;
		previousMicro_ = currentMicro_;
	}
}

void GameBase::System::GameTime::Release()
{
	timeEndPeriod(Util::PERIOD_MILLI);
}

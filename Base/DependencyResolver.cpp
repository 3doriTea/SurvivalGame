#include "DependencyResolver.h"

GameBase::DependencyResolver::DependencyResolver()
{
	Reset();
}

GameBase::DependencyResolver::~DependencyResolver()
{
}

GameBase::DependencyResolver& GameBase::DependencyResolver::AddDependency(
	const std::vector<int>& _dependencies)
{
	graph_.push_back(_dependencies);

	return *this;
}

void GameBase::DependencyResolver::Resolve()
{
	// 入次数を管理する配列
	indegrees_ = std::vector<int>(graph_.size(), 0);

	for (const std::vector<int>& v : graph_)
	{
		for (const int& to : v)
		{
			indegrees_[to]++;  // 入次数加算
		}
	}

	// 入次数 0 の要素を管理する
	queue_ = {};

	// 現時点での入次数 0 の要素をEnqueueしていく
	for (int i = 0; i < graph_.size(); i++)
	{
		if (indegrees_[i] == 0)
		{
			queue_.push(i);
		}
	}

	// 結果
	result_ = {};

	// キューが空っぽになるまで処理
	while (queue_.empty() == false)
	{
		// 入次数 0 の要素を1つ取り出す
		int from{ queue_.front() };
		queue_.pop();

		result_.push_back(from);

		// 取り出した要素から参照される要素の入次数を減らしていく
		for (const int to : graph_[from])
		{
			indegrees_[to]--;
			// 0 になったらキューに追加
			if (indegrees_[to] <= 0)
			{
				queue_.push(to);
			}
		}
	}

	// すべての要素を処理できていない == 閉路が存在する
	if (result_.size() < graph_.size())
	{
		// 閉路 / 閉路に依存している要素を列挙
		for (int i = 0; i < indegrees_.size(); i++)
		{
			if (indegrees_[i] > 0)
			{
				deadlocked_.push_back(i);
			}
		}
	}
}

void GameBase::DependencyResolver::Reset()
{
	graph_ = {};
	indegrees_ = {};
	queue_ = {};
	result_ = {};
	deadlocked_ = {};
}

void GameBase::DependencyResolver::ForEachResult(
	const std::function<void(int)>& _callback)
{
	for (const int v : result_)
	{
		_callback(v);
	}
}

void GameBase::DependencyResolver::ForEachDeadlocked(const std::function<void(int)>& _callback)
{
	for (const int v : deadlocked_)
	{
		_callback(v);
	}
}

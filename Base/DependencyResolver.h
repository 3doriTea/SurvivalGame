#pragma once
// REF: https://zenn.dev/reputeless/books/standard-cpp-for-competitive-programming/viewer/topological-sort


namespace GameBase
{
	/// <summary>
	/// 依存関係を解決するクラス
	/// </summary>
	class DependencyResolver
	{
	public:
		DependencyResolver();
		~DependencyResolver();

		DependencyResolver& AddDependency(const std::vector<int>& _dependencies);
		void Resolve();
		void Reset();

		void ForEachResult(const std::function<void(int)>& _callback);
		void ForEachDeadlocked(const std::function<void(int)>& _callback);

		bool Deadlocked() { return deadlocked_.size() > 0; }

	private:
		std::vector<std::vector<int>> graph_;
		std::vector<int> indegrees_;
		std::queue<int> queue_;
		std::vector<int> result_;
		std::vector<int> deadlocked_;
	};
}

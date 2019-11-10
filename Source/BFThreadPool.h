#pragma once
#ifndef BFTHREADPOOL
#define BFTHREADPOOL

#include<functional>
#include<thread>
#include<mutex>
#include<iostream>
#include<vector>
#include<algorithm>

typedef std::function<void()> Task;

// Each BFThread contains a part of tasks in BFThreadPool.
class BFThread
{
public:
	BFThread(int i) : mID(i), mBeginTaskId(0), mEndTaskId(0) {};
	BFThread(int i, int b_id, int e_id) :mID(i), mBeginTaskId(b_id), mEndTaskId(e_id) {};

	void Run();
	void AssignTask(int begin, int end) { mBeginTaskId = begin; mEndTaskId = end; }

private:

	int mID;
	int mBeginTaskId;
	int mEndTaskId;
};

// A BFThreadPool contains a set of tasks and sub threads.
class BFThreadPool
{
	friend class IBFRenderPipeline;

public:

	BFThreadPool(int n = 10) : mThreadNum(n) { mThreads.clear(); };

	// Start all BFThread in std::threasds.
	void StartUp()
	{
		//mThreads.resize(mThreadNum);
		int task_length = (tasks.size() / mThreadNum);
		std::vector<BFThread> bfThreads;
		for (int i = 0; i < mThreadNum; i++)
		{
			// Split tasks for each thread.
			int start = i * task_length;
			int end = start + task_length;
			bfThreads.emplace_back(i, start, end);
			mThreads.emplace_back(std::thread(&BFThread::Run, *(bfThreads.end()-1)));
		}
		// Wait.
		std::for_each(mThreads.begin(), mThreads.end(), std::mem_fn(&std::thread::join));
	}

	void Clear()
	{
		tasks.clear();
		mThreads.clear();
	}

	static std::vector<Task> tasks;
	static std::mutex mutex;

private:

	int mThreadNum;
	std::vector<std::thread> mThreads;

};

#endif // !BFTHREADPOOL


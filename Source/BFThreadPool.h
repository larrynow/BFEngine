#pragma once
#ifndef BFTHREADPOOL
#define BFTHREADPOOL

#include<functional>
#include<thread>
#include<iostream>
#include<vector>

typedef std::function<void()> Task;

// Each BFThread contains a part of tasks in BFThreadPool.
class BFThread
{
public:
	BFThread(int i) : mID(i), mBeginTaskId(0), mEndTaskId(0) {};

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
public:

	BFThreadPool(int n = 10) : mThreadNum(n) {};

	// Start all BFThread in std::threasds.
	void StartUp()
	{
		mThreads.resize(mThreadNum);
		for (int i = 0; i < mThreadNum; ++i)
		{
			// Split tasks for each thread.
			BFThread t(i);
			int start = i * (tasks.size() / mThreadNum);
			int end = (i + 1) * (tasks.size() / mThreadNum);
			t.AssignTask(start, end > tasks.size() ? tasks.size() : end);
			mThreads[i] = std::thread(&BFThread::Run, t);// Start each thread.
		}

		for (int i = 0; i < mThreads.size(); ++i)
		{
			mThreads[i].join();
		}

		return;
	}

	static std::vector<Task> tasks;

private:

	int mThreadNum;
	std::vector<std::thread> mThreads;

};


#endif // !BFTHREADPOOL


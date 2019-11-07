#pragma once
#ifndef BFTHREADPOOL
#define BFTHREADPOOL

#include<functional>
#include<thread>
#include<mutex>
#include<iostream>
#include<vector>

//template<class... Args>
class BFTask
{
public:

	BFTask(std::function<void()> func) : operation(func) {};

	void Run()
	{
		operation();
	};

private:

	std::function<void()> operation;
};


class BFThread
{
public:
	BFThread(int i) : ID(i) {};
	template<class Task>
	void Run(std::vector<Task>& tasks)
	{
		while (true)
		{
			BFThreadPool::mutex.lock();
			if (BFThreadPool::TaskId < tasks.size())
			{
				auto id = BFThreadPool::TaskId++;
				BFThreadPool::mutex.unlock();
				tasks[id].Run();
			}
			else
			{
				BFThreadPool::mutex.unlock();
				break;
			}
		}
	}

private:
	int ID;
};

class BFThreadPool
{
public:

	static int TaskId;
	static std::mutex mutex;

	BFThreadPool(int n = 10) : mThreadNum(n) {};

	template<class Task>
	void StartUp(std::vector<Task>& tasks)
	{
		for (int i = 0; i < mThreadNum; ++i)
		{
			BFThread thread(i);
			//mThreads.emplace_back(&BFThread::Run<Task>, &thread, std::ref(tasks), i);
			mThreads.emplace_back(&BFThread::Run<Task>, &thread, std::ref(tasks));
		}

		for (int i = 0; i < mThreads.size(); ++i)
		{
			mThreads[i].join();
		}

		return;
	}

private:

	int mThreadNum;
	std::vector<std::thread> mThreads;

};


#endif // !BFTHREADPOOL


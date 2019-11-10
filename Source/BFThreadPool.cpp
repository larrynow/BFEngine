#include"BFThreadPool.h"
#include<string>

std::vector<Task> BFThreadPool::tasks;
std::mutex BFThreadPool::mutex;

void BFThread::Run()
{
	// Only excute tasks that assigned to this thread.
	for (size_t i = mBeginTaskId; i < mEndTaskId; ++i)
	{
		//BFThreadPool::mutex.lock();
		//std::cout << std::to_string(mID) + " : " + std::to_string(i) <<std::endl;
		BFThreadPool::tasks[i]();
		//BFThreadPool::mutex.unlock();
	}
}
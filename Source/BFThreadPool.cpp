#include"BFThreadPool.h"
#include<string>

std::vector<Task> BFThreadPool::tasks;

void BFThread::Run()
{
	// Only excute tasks that assigned to this thread.
	for (size_t i = mBeginTaskId; i < mEndTaskId; ++i)
	{
		//std::cout << mID << std::endl;
		BFThreadPool::tasks[i]();
	}
}
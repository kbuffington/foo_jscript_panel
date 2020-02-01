#include "stdafx.h"
#include "thread_pool.h"

simple_thread_worker::simple_thread_worker() {}

simple_thread_worker::~simple_thread_worker()
{
	waitTillDone();
}

void simple_thread_worker::threadProc()
{
	pfc::tickcount_t last_tick = pfc::getTickCount();

	while (WaitForSingleObject(simple_thread_pool::instance().exiting, 0) == WAIT_TIMEOUT)
	{
		if (WaitForSingleObject(simple_thread_pool::instance().have_task, 1000) == WAIT_OBJECT_0)
		{
			auto task = simple_thread_pool::instance().acquire_task();

			if (task)
			{
				task->run();
				simple_thread_pool::instance().untrack(task);
				last_tick = pfc::getTickCount();
				continue;
			}
		}

		if (pfc::getTickCount() - last_tick >= 10000)
		{
			insync(simple_thread_pool::instance().cs);
			if (simple_thread_pool::instance().is_queue_empty()) break;
		}
	}

	simple_thread_pool::instance().remove_worker(this);
}

simple_thread_pool simple_thread_pool::instance_;

simple_thread_pool::simple_thread_pool()
{
	empty_worker = CreateEvent(nullptr, TRUE, TRUE, nullptr);
	exiting = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	have_task = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	pfc::dynamic_assert(empty_worker != INVALID_HANDLE_VALUE);
	pfc::dynamic_assert(exiting != INVALID_HANDLE_VALUE);
	pfc::dynamic_assert(have_task != INVALID_HANDLE_VALUE);
}

simple_thread_pool::~simple_thread_pool()
{
	CloseHandle(empty_worker);
	CloseHandle(exiting);
	CloseHandle(have_task);
}

simple_thread_pool& simple_thread_pool::instance()
{
	return instance_;
}

bool simple_thread_pool::enqueue(simple_thread_task* task)
{
	if (WaitForSingleObject(exiting, 0) == WAIT_OBJECT_0)
		return false;

	insync(cs);
	const int max_count = pfc::getOptimalWorkerThreadCount();
	track(task);

	if (num_workers < max_count)
	{
		auto worker = new simple_thread_worker;
		worker->start();
		++num_workers;
		ResetEvent(empty_worker);
	}

	return true;
}

bool simple_thread_pool::is_queue_empty()
{
	insync(cs);
	return task_list.get_count() == 0;
}

simple_thread_task* simple_thread_pool::acquire_task()
{
	insync(cs);

	t_task_list::iterator iter = task_list.first();

	if (iter.is_valid())
	{
		task_list.remove(iter);
	}

	if (is_queue_empty())
		ResetEvent(have_task);

	return iter.is_valid() ? *iter : nullptr;
}

void simple_thread_pool::exit()
{
	core_api::ensure_main_thread();

	SetEvent(exiting);

	// Because the tasks may use blocking SendMessage() function, it should be avoid using
	// infinite wait here, or both main thread and worker thread will block, and it's also
	// important to dispatch windows messages here.
	while (WaitForSingleObject(empty_worker, 0) == WAIT_TIMEOUT)
	{
		MSG msg;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	untrack_all();
}

void simple_thread_pool::remove_worker(simple_thread_worker* worker)
{
	insync(cs);
	--num_workers;

	if (num_workers == 0)
		SetEvent(empty_worker);

	fb2k::inMainThread([=]()
		{
			delete worker;
		});
}

void simple_thread_pool::track(simple_thread_task* task)
{
	insync(cs);
	const bool empty = is_queue_empty();
	task_list.add_item(task);

	if (empty)
		SetEvent(have_task);
}

void simple_thread_pool::untrack(simple_thread_task* task)
{
	insync(cs);
	task_list.remove_item(task);
	delete task;

	if (is_queue_empty())
		ResetEvent(have_task);
}

void simple_thread_pool::untrack_all()
{
	insync(cs);
	for (t_task_list::iterator iter = task_list.first(); iter.is_valid(); ++iter)
	{
		task_list.remove(iter);
		delete *iter;
	}

	ResetEvent(have_task);
}

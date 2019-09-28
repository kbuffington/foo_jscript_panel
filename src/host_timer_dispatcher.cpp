#include "stdafx.h"
#include "host_timer_dispatcher.h"

host_timer::host_timer(HWND hwnd, t_size id, t_size delay, bool isRepeated)
{
	m_hTimer = 0;

	m_hwnd = hwnd;
	m_delay = delay;
	m_is_repeated = isRepeated;
	m_id = id;

	m_is_stop_requested = false;
	m_is_stopped = false;
}

host_timer::~host_timer() {}

HWND host_timer::get_hwnd() const
{
	return m_hwnd;
}

VOID CALLBACK host_timer::timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	host_timer* timer = (host_timer*)lpParameter;

	if (timer->m_is_stopped)
	{
		return;
	}

	if (timer->m_is_stop_requested)
	{
		timer->m_is_stopped = true;
		host_timer_dispatcher::instance().on_timer_stop_request(timer->m_hwnd, timer->m_hTimer, timer->m_id);

		return;
	}

	if (!timer->m_is_repeated)
	{
		timer->m_is_stopped = true;
		SendMessage(timer->m_hwnd, UWM_TIMER, timer->m_id, 0);
		host_timer_dispatcher::instance().on_timer_stop_request(timer->m_hwnd, timer->m_hTimer, timer->m_id);

		return;
	}

	SendMessage(timer->m_hwnd, UWM_TIMER, timer->m_id, 0);
}

bool host_timer::start(HANDLE hTimerQueue)
{
	return !!CreateTimerQueueTimer(
		&m_hTimer,
		hTimerQueue,
		host_timer::timerProc,
		this,
		m_delay,
		m_is_repeated ? m_delay : 0,
		WT_EXECUTEINTIMERTHREAD | (m_is_repeated ? 0 : WT_EXECUTEONLYONCE));
}

void host_timer::stop()
{
	m_is_stop_requested = true;
}

host_timer_task::host_timer_task(IDispatch* pDisp, t_size timerId)
{
	m_pDisp = pDisp;
	m_timerId = timerId;

	m_refCount = 0;
	m_pDisp->AddRef();
}

host_timer_task::~host_timer_task()
{
	m_pDisp->Release();
}

void host_timer_task::acquire()
{
	++m_refCount;
}

void host_timer_task::invoke()
{
	acquire();

	VARIANTARG args[1];
	args[0].vt = VT_I4;
	args[0].lVal = m_timerId;
	DISPPARAMS dispParams = { args, nullptr, _countof(args), 0 };
	m_pDisp->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, nullptr, nullptr, nullptr);

	release();
}

void host_timer_task::release()
{
	if (!m_refCount)
	{
		return;
	}

	--m_refCount;
	if (!m_refCount)
	{
		host_timer_dispatcher::instance().on_task_complete(m_timerId);
	}
}

host_timer_dispatcher::host_timer_dispatcher()
{
	m_cur_timer_id = 1;
	m_timer_queue = CreateTimerQueue();
}

host_timer_dispatcher::~host_timer_dispatcher()
{
	stop_thread();
	// Clear all references
	m_task_map.clear();
}

host_timer_dispatcher& host_timer_dispatcher::instance()
{
	static host_timer_dispatcher timerDispatcher;
	return timerDispatcher;
}

t_size host_timer_dispatcher::create_timer(HWND hwnd, t_size delay, bool isRepeated, IDispatch* pDisp)
{
	if (!pDisp)
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_timer_mutex);

	t_size id = m_cur_timer_id++;
	while (m_task_map.end() != m_task_map.find(id) && m_timer_map.end() != m_timer_map.find(id))
	{
		id = m_cur_timer_id++;
	}

	m_timer_map.emplace(id, new host_timer(hwnd, id, delay, isRepeated));

	const auto curTask = m_task_map.emplace(id, new host_timer_task(pDisp, id));
	curTask.first->second->acquire();

	if (!m_timer_map[id]->start(m_timer_queue))
	{
		m_timer_map.erase(id);
		m_task_map.erase(id);
		return 0;
	}

	return id;
}

t_size host_timer_dispatcher::set_interval(HWND hwnd, t_size delay, IDispatch* pDisp)
{
	return create_timer(hwnd, delay, true, pDisp);
}

t_size host_timer_dispatcher::set_timeout(HWND hwnd, t_size delay, IDispatch* pDisp)
{
	return create_timer(hwnd, delay, false, pDisp);
}

void host_timer_dispatcher::create_thread()
{
	m_thread = new std::thread(&host_timer_dispatcher::thread_main, this);
}

void host_timer_dispatcher::invoke_message(t_size timerId)
{
	if (m_task_map.end() != m_task_map.find(timerId))
	{
		m_task_map[timerId]->invoke();
	}
}

void host_timer_dispatcher::kill_timer(t_size timerId)
{
	{
		std::lock_guard<std::mutex> lock(m_timer_mutex);

		const auto timerIter = m_timer_map.find(timerId);
		if (m_timer_map.end() != timerIter)
		{
			timerIter->second->stop();
		}
	}

	const auto taskIter = m_task_map.find(timerId);
	if (m_task_map.end() != taskIter)
	{
		taskIter->second->release();
	}
}

void host_timer_dispatcher::kill_timers(HWND hwnd)
{
	std::list<t_size> timersToDelete;

	{
		std::lock_guard<std::mutex> lock(m_timer_mutex);
		for (const auto& elem : m_timer_map)
		{
			if (elem.second->get_hwnd() == hwnd)
			{
				timersToDelete.push_back(elem.first);
			}
		}
	}

	for (auto timerId : timersToDelete)
	{
		kill_timer(timerId);
	}
}

void host_timer_dispatcher::on_task_complete(t_size timerId)
{
	if (m_task_map.end() != m_task_map.find(timerId))
	{
		m_task_map.erase(timerId);
	}
}

void host_timer_dispatcher::on_timer_expire(t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_timer_mutex);

	m_timer_map.erase(timerId);
}

void host_timer_dispatcher::on_timer_stop_request(HWND hwnd, HANDLE hTimer, t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_thread_task_mutex);

	thread_task threadTask = {};
	threadTask.taskId = killTimerTask;
	threadTask.hwnd = hwnd;
	threadTask.hTimer = hTimer;
	threadTask.timerId = timerId;

	m_thread_task_list.push_front(threadTask);
	m_cv.notify_one();
}

void host_timer_dispatcher::stop_thread()
{
	if (!m_thread)
	{
		return;
	}

	{
		std::lock_guard<std::mutex> lock(m_thread_task_mutex);
		thread_task threadTask = {};
		threadTask.taskId = shutdownTask;

		m_thread_task_list.push_front(threadTask);
		m_cv.notify_one();
	}

	if (m_thread->joinable())
	{
		m_thread->join();
	}

	delete m_thread;
	m_thread = nullptr;
}

void host_timer_dispatcher::thread_main()
{
	while (true)
	{
		thread_task threadTask;
		{
			std::unique_lock<std::mutex> lock(m_thread_task_mutex);

			while (m_thread_task_list.empty())
			{
				m_cv.wait(lock);
			}

			if (m_thread_task_list.empty())
			{
				continue;
			}

			threadTask = m_thread_task_list.front();
			m_thread_task_list.pop_front();
		}

		switch (threadTask.taskId)
		{
		case killTimerTask:
			DeleteTimerQueueTimer(m_timer_queue, threadTask.hTimer, INVALID_HANDLE_VALUE);
			on_timer_expire(threadTask.timerId);
			break;
		case shutdownTask:
			DeleteTimerQueueEx(m_timer_queue, INVALID_HANDLE_VALUE);
			m_timer_queue = nullptr;
			return;
		default:
			assert(0);
			break;
		}
	}
}

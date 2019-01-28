#include "stdafx.h"
#include "host_timer_dispatcher.h"

host_timer::host_timer(HWND hWnd, t_size id, t_size delay, bool isRepeated)
{
	m_hTimer = 0;

	m_hWnd = hWnd;
	m_delay = delay;
	m_isRepeated = isRepeated;
	m_id = id;

	m_isStopRequested = false;
	m_isStopped = false;
}

host_timer::~host_timer() {}

HANDLE host_timer::GetHandle() const
{
	return m_hTimer;
}

HWND host_timer::GetHwnd() const
{
	return m_hWnd;
}

VOID CALLBACK host_timer::timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	host_timer* timer = (host_timer*)lpParameter;

	if (timer->m_isStopped)
	{
		return;
	}

	if (timer->m_isStopRequested)
	{
		timer->m_isStopped = true;
		host_timer_dispatcher::instance().onTimerStopRequest(timer->m_hWnd, timer->m_hTimer, timer->m_id);

		return;
	}

	if (!timer->m_isRepeated)
	{
		timer->m_isStopped = true;
		SendMessage(timer->m_hWnd, UWM_TIMER, timer->m_id, 0);
		host_timer_dispatcher::instance().onTimerStopRequest(timer->m_hWnd, timer->m_hTimer, timer->m_id);

		return;
	}

	SendMessage(timer->m_hWnd, UWM_TIMER, timer->m_id, 0);
}

bool host_timer::start(HANDLE hTimerQueue)
{
	return !!CreateTimerQueueTimer(
		&m_hTimer,
		hTimerQueue,
		host_timer::timerProc,
		this,
		m_delay,
		m_isRepeated ? m_delay : 0,
		WT_EXECUTEINTIMERTHREAD | (m_isRepeated ? 0 : WT_EXECUTEONLYONCE));
}

void host_timer::stop()
{
	m_isStopRequested = true;
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
	DISPPARAMS dispParams = { args, NULL, _countof(args), 0 };
	m_pDisp->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);

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
		host_timer_dispatcher::instance().onTaskComplete(m_timerId);
	}
}

host_timer_dispatcher::host_timer_dispatcher()
{
	m_curTimerId = 1;
	m_hTimerQueue = CreateTimerQueue();
}

host_timer_dispatcher::~host_timer_dispatcher()
{
	stopThread();
	// Clear all references
	m_taskMap.clear();
}

host_timer_dispatcher& host_timer_dispatcher::instance()
{
	static host_timer_dispatcher timerDispatcher;
	return timerDispatcher;
}

t_size host_timer_dispatcher::createTimer(HWND hWnd, t_size delay, bool isRepeated, IDispatch* pDisp)
{
	if (!pDisp)
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_timerMutex);

	t_size id = m_curTimerId++;
	while (m_taskMap.end() != m_taskMap.find(id) && m_timerMap.end() != m_timerMap.find(id))
	{
		id = m_curTimerId++;
	}

	m_timerMap.emplace(id, new host_timer(hWnd, id, delay, isRepeated));

	auto curTask = m_taskMap.emplace(id, new host_timer_task(pDisp, id));
	curTask.first->second->acquire();

	if (!m_timerMap[id]->start(m_hTimerQueue))
	{
		m_timerMap.erase(id);
		m_taskMap.erase(id);
		return 0;
	}

	return id;
}

t_size host_timer_dispatcher::setInterval(HWND hWnd, t_size delay, IDispatch* pDisp)
{
	return createTimer(hWnd, delay, true, pDisp);
}

t_size host_timer_dispatcher::setTimeout(HWND hWnd, t_size delay, IDispatch* pDisp)
{
	return createTimer(hWnd, delay, false, pDisp);
}

void host_timer_dispatcher::createThread()
{
	m_thread = new std::thread(&host_timer_dispatcher::threadMain, this);
}

void host_timer_dispatcher::killTimer(t_size timerId)
{
	{
		std::lock_guard<std::mutex> lock(m_timerMutex);

		auto timerIter = m_timerMap.find(timerId);
		if (m_timerMap.end() != timerIter)
		{
			timerIter->second->stop();
		}
	}

	auto taskIter = m_taskMap.find(timerId);
	if (m_taskMap.end() != taskIter)
	{
		taskIter->second->release();
	}
}

void host_timer_dispatcher::onInvokeMessage(t_size timerId)
{
	if (m_taskMap.end() != m_taskMap.find(timerId))
	{
		m_taskMap[timerId]->invoke();
	}
}

void host_timer_dispatcher::onPanelUnload(HWND hWnd)
{
	std::list<t_size> timersToDelete;

	{
		std::lock_guard<std::mutex> lock(m_timerMutex);
		for (const auto& elem : m_timerMap)
		{
			if (elem.second->GetHwnd() == hWnd)
			{
				timersToDelete.push_back(elem.first);
			}
		}
	}

	for (auto timerId : timersToDelete)
	{
		killTimer(timerId);
	}
}

void host_timer_dispatcher::onTaskComplete(t_size timerId)
{
	if (m_taskMap.end() != m_taskMap.find(timerId))
	{
		m_taskMap.erase(timerId);
	}
}

void host_timer_dispatcher::onTimerExpire(t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_timerMutex);

	m_timerMap.erase(timerId);
}

void host_timer_dispatcher::onTimerStopRequest(HWND hWnd, HANDLE hTimer, t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_threadTaskMutex);

	ThreadTask threadTask = {};
	threadTask.taskId = killTimerTask;
	threadTask.hWnd = hWnd;
	threadTask.hTimer = hTimer;
	threadTask.timerId = timerId;

	m_threadTaskList.push_front(threadTask);
	m_cv.notify_one();
}

void host_timer_dispatcher::stopThread()
{
	if (!m_thread)
	{
		return;
	}

	{
		std::lock_guard<std::mutex> lock(m_threadTaskMutex);
		ThreadTask threadTask = {};
		threadTask.taskId = shutdownTask;

		m_threadTaskList.push_front(threadTask);
		m_cv.notify_one();
	}

	if (m_thread->joinable())
	{
		m_thread->join();
	}

	delete m_thread;
	m_thread = NULL;
}

void host_timer_dispatcher::threadMain()
{
	while (true)
	{
		ThreadTask threadTask;
		{
			std::unique_lock<std::mutex> lock(m_threadTaskMutex);

			while (m_threadTaskList.empty())
			{
				m_cv.wait(lock);
			}

			if (m_threadTaskList.empty())
			{
				continue;
			}

			threadTask = m_threadTaskList.front();
			m_threadTaskList.pop_front();
		}

		switch (threadTask.taskId)
		{
		case killTimerTask:
			DeleteTimerQueueTimer(m_hTimerQueue, threadTask.hTimer, INVALID_HANDLE_VALUE);
			onTimerExpire(threadTask.timerId);
			break;
		case shutdownTask:
			DeleteTimerQueueEx(m_hTimerQueue, INVALID_HANDLE_VALUE);
			m_hTimerQueue = NULL;
			return;
		default:
			assert(0);
			break;
		}
	}
}

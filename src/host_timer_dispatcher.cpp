#include "stdafx.h"
#include "host_timer_dispatcher.h"

#include <vector>

host_timer::host_timer(HWND hWnd, t_size id, t_size delay, bool isRepeated) : m_hWnd(hWnd), m_delay(delay), m_isRepeated(isRepeated), m_id(id) {}

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

host_timer_task::host_timer_task(IDispatch* pDisp, t_size timerId) : m_pDisp(pDisp, true), m_timerId(timerId) {}

void host_timer_task::invoke()
{
	VARIANTARG args[1];
	args[0].vt = VT_I4;
	args[0].lVal = m_timerId;
	DISPPARAMS dispParams = { args, NULL, _countof(args), 0 };
	m_pDisp->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);
}

host_timer_dispatcher::host_timer_dispatcher()
{
	m_curTimerId = 1;
	m_hTimerQueue = CreateTimerQueue();
	createThread();
}

host_timer_dispatcher::~host_timer_dispatcher()
{
	stopThread();
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
	while (m_timerMap.count(id))
	{
		id = m_curTimerId++;
	}

	m_timerMap.emplace(id, std::make_unique<TimerObject>(std::make_unique<host_timer>(hWnd, id, delay, isRepeated), std::make_unique<host_timer_task>(pDisp, id)));

	if (!m_timerMap[id]->timer->start(m_hTimerQueue))
	{
		m_timerMap.erase(id);
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
	m_thread = std::make_unique<std::thread>(&host_timer_dispatcher::threadMain, this);
}

void host_timer_dispatcher::killTimer(t_size timerId)
{
	std::lock_guard<std::mutex> lock(m_timerMutex);

	auto it = m_timerMap.find(timerId);
	if (m_timerMap.end() != it)
	{
		it->second->timer->stop();
	}
}

void host_timer_dispatcher::onInvokeMessage(t_size timerId)
{
	std::shared_ptr<host_timer_task> task;
	{
		std::lock_guard<std::mutex> lock(m_timerMutex);

		if (m_timerMap.count(timerId))
		{
			task = m_timerMap[timerId]->task;
		}
	}

	if (task)
	{
		task->invoke();
	}
}

void host_timer_dispatcher::onPanelUnload(HWND hWnd)
{
	std::vector<t_size> timersToDelete;

	{
		std::lock_guard<std::mutex> lock(m_timerMutex);

		for (const auto& elem : m_timerMap)
		{
			if (elem.second->timer->GetHwnd() == hWnd)
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

void host_timer_dispatcher::onTimerExpire(t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_timerMutex);

	m_timerMap.erase(timerId);
}

void host_timer_dispatcher::onTimerStopRequest(HWND hWnd, HANDLE hTimer, t_size timerId)
{
	std::unique_lock<std::mutex> lock(m_threadTaskMutex);

	ThreadTask threadTask = {};
	threadTask.taskId = ThreadTaskId::killTimerTask;
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
		threadTask.taskId = ThreadTaskId::shutdownTask;

		m_threadTaskList.push_front(threadTask);
		m_cv.notify_one();
	}

	if (m_thread->joinable())
	{
		m_thread->join();
	}

	m_thread.reset();
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
		case ThreadTaskId::killTimerTask:
			DeleteTimerQueueTimer(m_hTimerQueue, threadTask.hTimer, INVALID_HANDLE_VALUE);
			onTimerExpire(threadTask.timerId);
			break;
		case ThreadTaskId::shutdownTask:
			DeleteTimerQueueEx(m_hTimerQueue, INVALID_HANDLE_VALUE);
			m_hTimerQueue = NULL;
			return;
		default:
			assert(0);
			break;
		}
	}
}

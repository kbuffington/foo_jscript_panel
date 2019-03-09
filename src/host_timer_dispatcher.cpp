#include "stdafx.h"
#include "host_timer_dispatcher.h"

#include <vector>

host_timer::host_timer(HWND hwnd, t_size id, t_size delay, bool is_repeated) : m_hwnd(hwnd), m_delay(delay), m_is_repeated(is_repeated), m_id(id) {}

HWND host_timer::GetHwnd() const
{
	return m_hwnd;
}

VOID CALLBACK host_timer::timer_proc(PVOID lp_param, BOOLEAN timer_or_wait_fired)
{
	host_timer* timer = (host_timer*)lp_param;

	if (timer->m_is_stopped)
	{
		return;
	}

	if (timer->m_is_stop_requested)
	{
		timer->m_is_stopped = true;
		host_timer_dispatcher::instance().stop_request(timer->m_hwnd, timer->m_timer_handle, timer->m_id);
		return;
	}

	if (!timer->m_is_repeated)
	{
		timer->m_is_stopped = true;
		SendMessage(timer->m_hwnd, UWM_TIMER, timer->m_id, 0);
		host_timer_dispatcher::instance().stop_request(timer->m_hwnd, timer->m_timer_handle, timer->m_id);
		return;
	}

	SendMessage(timer->m_hwnd, UWM_TIMER, timer->m_id, 0);
}

bool host_timer::start(HANDLE timer_queue)
{
	return !!CreateTimerQueueTimer(
		&m_timer_handle,
		timer_queue,
		host_timer::timer_proc,
		this,
		m_delay,
		m_is_repeated ? m_delay : 0,
		WT_EXECUTEINTIMERTHREAD | (m_is_repeated ? 0 : WT_EXECUTEONLYONCE));
}

void host_timer::stop()
{
	m_is_stop_requested = true;
}

host_timer_task::host_timer_task(IDispatch* p_disp, t_size timer_id) : m_disp(p_disp, true), m_timer_id(timer_id) {}

void host_timer_task::invoke()
{
	VARIANTARG args[1];
	args[0].vt = VT_UI4;
	args[0].ulVal = m_timer_id;
	DISPPARAMS dispParams = { args, nullptr, _countof(args), 0 };
	m_disp->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispParams, nullptr, nullptr, nullptr);
}

host_timer_dispatcher::host_timer_dispatcher()
{
	m_cur_timer_id = 1;
	m_timer_queue = CreateTimerQueue();
	create_thread();
}

host_timer_dispatcher::~host_timer_dispatcher()
{
	stop_thread();
}

host_timer_dispatcher& host_timer_dispatcher::instance()
{
	static host_timer_dispatcher dispatcher;
	return dispatcher;
}

t_size host_timer_dispatcher::create_timer(HWND hwnd, t_size delay, bool is_repeated, IDispatch* p_disp)
{
	if (!p_disp)
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_timer_mutex);

	t_size id = m_cur_timer_id++;
	while (m_timer_map.count(id))
	{
		id = m_cur_timer_id++;
	}

	m_timer_map.emplace(id, std::make_unique<timer_object>(std::make_unique<host_timer>(hwnd, id, delay, is_repeated), std::make_unique<host_timer_task>(p_disp, id)));

	if (!m_timer_map[id]->timer->start(m_timer_queue))
	{
		m_timer_map.erase(id);
		return 0;
	}

	return id;
}

t_size host_timer_dispatcher::set_interval(HWND hwnd, t_size delay, IDispatch* p_disp)
{
	return create_timer(hwnd, delay, true, p_disp);
}

t_size host_timer_dispatcher::set_timeout(HWND hwnd, t_size delay, IDispatch* p_disp)
{
	return create_timer(hwnd, delay, false, p_disp);
}

void host_timer_dispatcher::create_thread()
{
	m_thread = std::make_unique<std::thread>(&host_timer_dispatcher::thread_main, this);
}

void host_timer_dispatcher::kill_timer(t_size timer_id)
{
	std::lock_guard<std::mutex> lock(m_timer_mutex);

	auto it = m_timer_map.find(timer_id);
	if (m_timer_map.end() != it)
	{
		it->second->timer->stop();
	}
}

void host_timer_dispatcher::invoke_message(t_size timer_id)
{
	std::shared_ptr<host_timer_task> task;

	{
		std::lock_guard<std::mutex> lock(m_timer_mutex);

		if (m_timer_map.count(timer_id))
		{
			task = m_timer_map[timer_id]->task;
		}
	}

	if (task)
	{
		task->invoke();
	}
}

void host_timer_dispatcher::script_unload(HWND hwnd)
{
	std::vector<t_size> timers_to_delete;

	{
		std::lock_guard<std::mutex> lock(m_timer_mutex);

		for (const auto& elem : m_timer_map)
		{
			if (elem.second->timer->GetHwnd() == hwnd)
			{
				timers_to_delete.push_back(elem.first);
			}
		}
	}

	for (auto timer_id : timers_to_delete)
	{
		kill_timer(timer_id);
	}
}

void host_timer_dispatcher::stop_request(HWND hwnd, HANDLE timer_handle, t_size timer_id)
{
	std::unique_lock<std::mutex> lock(m_thread_task_mutex);

	thread_task task = {};
	task.task_id = thread_task_id::kill_timer_task;
	task.hwnd = hwnd;
	task.timer_handle = timer_handle;
	task.timer_id = timer_id;

	m_thread_task_list.push_front(task);
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
		thread_task task = {};
		task.task_id = thread_task_id::shutdown_task;

		m_thread_task_list.push_front(task);
		m_cv.notify_one();
	}

	if (m_thread->joinable())
	{
		m_thread->join();
	}

	m_thread.reset();
}

void host_timer_dispatcher::thread_main()
{
	while (true)
	{
		thread_task task;

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

			task = m_thread_task_list.front();
			m_thread_task_list.pop_front();
		}

		switch (task.task_id)
		{
		case thread_task_id::kill_timer_task:
			DeleteTimerQueueTimer(m_timer_queue, task.timer_handle, INVALID_HANDLE_VALUE);
			{
				std::unique_lock<std::mutex> lock(m_timer_mutex);
				m_timer_map.erase(task.timer_id);
			}
			break;
		case thread_task_id::shutdown_task:
			DeleteTimerQueueEx(m_timer_queue, INVALID_HANDLE_VALUE);
			m_timer_queue = nullptr;
			return;
		default:
			assert(0);
			break;
		}
	}
}

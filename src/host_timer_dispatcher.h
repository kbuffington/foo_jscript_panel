#pragma once

class host_timer
{
public:
	host_timer(HWND hwnd, size_t id, size_t delay, bool isRepeated);
	~host_timer();

	static VOID CALLBACK timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	HWND get_hwnd() const;
	bool start(HANDLE hTimerQueue);
	void stop();

private:
	HWND m_hwnd = nullptr;
	HANDLE m_hTimer = nullptr;
	IDispatch* m_pDisp = nullptr;
	bool m_is_repeated = false;
	bool m_is_stopped = false;
	size_t m_delay = 0;
	size_t m_id = 0;
	std::atomic_bool m_is_stop_requested = false;
};

class host_timer_task
{
public:
	host_timer_task(IDispatch* pDisp, size_t timerId);
	~host_timer_task();

	void acquire();
	void invoke();
	void release();

private:
	IDispatch* m_pDisp = nullptr;
	size_t m_refCount = 0;
	size_t m_timerId = 0;
};

class host_timer_dispatcher
{
public:
	host_timer_dispatcher();
	~host_timer_dispatcher();

	static host_timer_dispatcher& instance();

	size_t set_interval(HWND hwnd, size_t delay, IDispatch* pDisp);
	size_t set_timeout(HWND hwnd, size_t delay, IDispatch* pDisp);
	void invoke_message(size_t timerId);
	void kill_timer(size_t timerId);
	void kill_timers(HWND hwnd);
	void on_task_complete(size_t timerId);
	void on_timer_stop_request(HWND hwnd, HANDLE hTimer, size_t timerId);

private:
	size_t create_timer(HWND hwnd, size_t delay, bool is_repeated, IDispatch* pDisp);
	void create_thread();
	void stop_thread();
	void thread_main();

	enum thread_task_id
	{
		killTimerTask,
		shutdownTask
	};

	struct thread_task
	{
		HANDLE hTimer;
		HWND hwnd;
		thread_task_id taskId;
		size_t timerId;
	};

	using task_map = std::map<size_t, std::unique_ptr<host_timer_task>>;
	using timer_map = std::map<size_t, std::unique_ptr<host_timer>>;

	HANDLE m_timer_queue;
	task_map m_task_map;
	timer_map m_timer_map;
	size_t m_cur_timer_id = 1;
	std::condition_variable m_cv;
	std::list<thread_task> m_thread_task_list;
	std::mutex m_timer_mutex;
	std::mutex m_thread_task_mutex;
	std::thread* m_thread{};
};

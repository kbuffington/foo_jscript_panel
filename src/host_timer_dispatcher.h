#pragma once
#include <mutex>
#include <map>
#include <atomic>
#include <list>

class host_timer
{
public:
	host_timer(HWND hwnd, t_size id, t_size delay, bool isRepeated);
	~host_timer();

	static VOID CALLBACK timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	HWND get_hwnd() const;
	bool start(HANDLE hTimerQueue);
	void stop();

private:
	HWND m_hwnd;
	HANDLE m_hTimer;
	IDispatch* m_pDisp;
	bool m_is_repeated;
	bool m_is_stopped;
	std::atomic<bool> m_is_stop_requested;
	t_size m_delay;
	t_size m_id;
};

class host_timer_task
{
public:
	host_timer_task(IDispatch* pDisp, t_size timerId);
	~host_timer_task();

	void acquire();
	void invoke();
	void release();

private:
	IDispatch* m_pDisp;
	t_size m_refCount;
	t_size m_timerId;
};

class host_timer_dispatcher
{
public:
	~host_timer_dispatcher();

	static host_timer_dispatcher& instance();

	t_size set_interval(HWND hwnd, t_size delay, IDispatch* pDisp);
	t_size set_timeout(HWND hwnd, t_size delay, IDispatch* pDisp);
	void invoke_message(t_size timerId);
	void kill_timer(t_size timerId);
	void kill_timers(HWND hwnd);
	void on_task_complete(t_size timerId);
	void on_timer_expire(t_size timerId);
	void on_timer_stop_request(HWND hwnd, HANDLE hTimer, t_size timerId);

private:
	host_timer_dispatcher();

	t_size create_timer(HWND hwnd, t_size delay, bool isRepeated, IDispatch* pDisp);
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
		t_size timerId;
	};

	using task_map = std::map<t_size, std::unique_ptr<host_timer_task>>;
	using timer_map = std::map<t_size, std::unique_ptr<host_timer>>;

	HANDLE m_timer_queue;
	task_map m_task_map;
	timer_map m_timer_map;
	std::condition_variable m_cv;
	std::list<thread_task> m_thread_task_list;
	std::mutex m_timer_mutex;
	std::mutex m_thread_task_mutex;
	std::thread* m_thread;
	t_size m_cur_timer_id;
};

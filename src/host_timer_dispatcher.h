#pragma once
#include <mutex>
#include <map>
#include <atomic>
#include <list>

class host_timer
{
public:
	host_timer(HWND hwnd, t_size id, t_size delay, bool is_repeated);
	~host_timer() = default;

	HWND get_hwnd() const;
	bool start(HANDLE timer_queue);
	static VOID CALLBACK timer_proc(PVOID lp_param, BOOLEAN timer_or_wait_fired);
	void stop();

private:
	HWND m_hwnd = nullptr;
	HANDLE m_timer_handle = nullptr;
	bool m_is_repeated;
	bool m_is_stopped = false;
	std::atomic<bool> m_is_stop_requested = false;
	t_size m_delay;
	t_size m_id;
};

class host_timer_task
{
public:
	host_timer_task(IDispatch* p_disp, t_size timer_id);
	~host_timer_task() = default;

	void invoke();

private:
	IDispatchPtr m_disp;
	t_size m_timer_id;
};

class host_timer_dispatcher
{
public:
	host_timer_dispatcher();
	~host_timer_dispatcher();

	static host_timer_dispatcher& instance();
	void finalise();

	t_size set_interval(HWND hwnd, t_size delay, IDispatch* p_disp);
	t_size set_timeout(HWND hwnd, t_size delay, IDispatch* p_disp);
	void kill_timer(t_size timer_id);
	void invoke_message(t_size timer_id);
	void script_unload(HWND hwnd);
	void stop_request(HWND hwnd, HANDLE timer_handle, t_size timerId);

private:
	t_size create_timer(HWND hwnd, t_size delay, bool is_repeated, IDispatch* p_disp);
	void create_thread();
	void stop_thread();
	void thread_main();

	enum class thread_task_id
	{
		kill_timer_task,
		shutdown_task
	};

	struct thread_task
	{
		HANDLE timer_handle;
		HWND hwnd;
		thread_task_id task_id;
		t_size timer_id;
	};

	struct timer_object
	{
		timer_object(std::unique_ptr<host_timer> timerArg, std::unique_ptr<host_timer_task> taskArg) : timer(std::move(timerArg)), task(std::move(taskArg)) {}
		std::unique_ptr<host_timer> timer;
		std::shared_ptr<host_timer_task> task;
	};

	using timer_map = std::map<t_size, std::unique_ptr<timer_object>>;

	HANDLE m_timer_queue = nullptr;
	std::condition_variable m_cv;
	std::list<thread_task> m_thread_task_list;
	std::mutex m_timer_mutex;
	std::mutex m_thread_task_mutex;
	std::unique_ptr<std::thread> m_thread;
	timer_map m_timer_map;
	t_size m_cur_timer_id;
};

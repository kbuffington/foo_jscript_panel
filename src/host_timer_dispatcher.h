#pragma once
#include <mutex>
#include <map>
#include <atomic>
#include <list>

class host_timer
{
public:
	host_timer(HWND hWnd, t_size id, t_size delay, bool isRepeated);
	~host_timer() = default;

	HANDLE GetHandle() const;
	HWND GetHwnd() const;
	bool start(HANDLE hTimerQueue);
	static VOID CALLBACK timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	void stop();

private:
	HWND m_hWnd = nullptr;
	HANDLE m_hTimer = nullptr;
	bool m_isRepeated;
	bool m_isStopped = false;
	std::atomic<bool> m_isStopRequested = false;
	t_size m_delay;
	t_size m_id;
};

class host_timer_task
{
public:
	host_timer_task(IDispatch* pDisp, t_size timerId);
	~host_timer_task() = default;

	void invoke();

private:
	IDispatchPtr m_pDisp;
	t_size m_timerId;
};

class host_timer_dispatcher
{
public:
	~host_timer_dispatcher();

	static host_timer_dispatcher& instance();
	t_size setInterval(HWND hWnd, t_size delay, IDispatch* pDisp);
	t_size setTimeout(HWND hWnd, t_size delay, IDispatch* pDisp);
	void killTimer(t_size timerId);
	void onInvokeMessage(t_size timerId);
	void onPanelUnload(HWND hWnd);
	void onTimerExpire(t_size timerId);
	void onTimerStopRequest(HWND hWnd, HANDLE hTimer, t_size timerId);

private:
	host_timer_dispatcher();

	t_size createTimer(HWND hWnd, t_size delay, bool isRepeated, IDispatch* pDisp);
	void createThread();
	void stopThread();
	void threadMain();

	enum class ThreadTaskId
	{
		killTimerTask,
		shutdownTask
	};

	struct ThreadTask
	{
		HANDLE hTimer;
		HWND hWnd;
		ThreadTaskId taskId;
		t_size timerId;
	};

	struct TimerObject
	{
		TimerObject(std::unique_ptr<host_timer> timerArg, std::unique_ptr<host_timer_task> taskArg)
			: timer(std::move(timerArg))
			, task(std::move(taskArg))
		{
		}
		std::unique_ptr<host_timer> timer;
		std::shared_ptr<host_timer_task> task;
	};

	using TimerMap = std::map<t_size, std::unique_ptr<TimerObject>>;

	HANDLE m_hTimerQueue = nullptr;
	TimerMap m_timerMap;
	std::condition_variable m_cv;
	std::list<ThreadTask> m_threadTaskList;
	std::mutex m_timerMutex;
	std::mutex m_threadTaskMutex;
	std::unique_ptr<std::thread> m_thread;
	t_size m_curTimerId;
};

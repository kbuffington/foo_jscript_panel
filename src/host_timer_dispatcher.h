#pragma once
#include <mutex>
#include <map>
#include <atomic>
#include <list>

class host_timer
{
public:
	host_timer(HWND hWnd, t_size id, t_size delay, bool isRepeated);
	~host_timer();

	HANDLE GetHandle() const;
	HWND GetHwnd() const;
	bool start(HANDLE hTimerQueue);
	static VOID CALLBACK timerProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	void stop();

private:
	HWND m_hWnd;
	HANDLE m_hTimer;
	IDispatch* m_pDisp;
	bool m_isRepeated;
	bool m_isStopped;
	std::atomic<bool> m_isStopRequested;
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
	t_size setInterval(HWND hWnd, t_size delay, IDispatch* pDisp);
	t_size setTimeout(HWND hWnd, t_size delay, IDispatch* pDisp);
	void killTimer(t_size timerId);
	void onInvokeMessage(t_size timerId);
	void onPanelUnload(HWND hWnd);
	void onTaskComplete(t_size timerId);
	void onTimerExpire(t_size timerId);
	void onTimerStopRequest(HWND hWnd, HANDLE hTimer, t_size timerId);

private:
	host_timer_dispatcher();

	t_size createTimer(HWND hWnd, t_size delay, bool isRepeated, IDispatch* pDisp);
	void createThread();
	void stopThread();
	void threadMain();

	enum ThreadTaskId
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

	using TaskMap = std::map<t_size, std::unique_ptr<host_timer_task>>;
	using TimerMap = std::map<t_size, std::unique_ptr<host_timer>>;

	HANDLE m_hTimerQueue;
	TaskMap m_taskMap;
	TimerMap m_timerMap;
	std::condition_variable m_cv;
	std::list<ThreadTask> m_threadTaskList;
	std::mutex m_timerMutex;
	std::mutex m_threadTaskMutex;
	std::thread* m_thread;
	t_size m_curTimerId;
};

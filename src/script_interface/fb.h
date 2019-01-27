#pragma once
#include "script_interface.h"

class Fb : public IDispatchImpl3<IFb>
{
protected:
	Fb();
	virtual ~Fb();

public:
	STDMETHODIMP AcquireUiSelectionHolder(IUiSelectionHolder** pp);
	STDMETHODIMP AddDirectory();
	STDMETHODIMP AddFiles();
	STDMETHODIMP CheckClipboardContents(UINT window_id, VARIANT_BOOL* p);
	STDMETHODIMP ClearPlaylist();
	STDMETHODIMP CopyHandleListToClipboard(IMetadbHandleList* handles, VARIANT_BOOL* p);
	STDMETHODIMP CreateContextMenuManager(IContextMenuManager** pp);
	STDMETHODIMP CreateHandleList(VARIANT handle, IMetadbHandleList** pp);
	STDMETHODIMP CreateMainMenuManager(IMainMenuManager** pp);
	STDMETHODIMP CreateProfiler(BSTR name, IProfiler** pp);
	STDMETHODIMP DoDragDrop(IMetadbHandleList* handles, UINT okEffects, UINT* p);
	STDMETHODIMP Exit();
	STDMETHODIMP GetClipboardContents(UINT window_id, IMetadbHandleList** pp);
	STDMETHODIMP GetDSPPresets(BSTR* p);
	STDMETHODIMP GetFocusItem(VARIANT_BOOL force, IMetadbHandle** pp);
	STDMETHODIMP GetLibraryItems(IMetadbHandleList** pp);
	STDMETHODIMP GetLibraryRelativePath(IMetadbHandle* handle, BSTR* p);
	STDMETHODIMP GetNowPlaying(IMetadbHandle** pp);
	STDMETHODIMP GetOutputDevices(BSTR* p);
	STDMETHODIMP GetQueryItems(IMetadbHandleList* handles, BSTR query, IMetadbHandleList** pp);
	STDMETHODIMP GetSelection(IMetadbHandle** pp);
	STDMETHODIMP GetSelections(UINT flags, IMetadbHandleList** pp);
	STDMETHODIMP GetSelectionType(UINT* p);
	STDMETHODIMP IsLibraryEnabled(VARIANT_BOOL* p);
	STDMETHODIMP IsMetadbInMediaLibrary(IMetadbHandle* handle, VARIANT_BOOL* p);
	STDMETHODIMP LoadPlaylist();
	STDMETHODIMP Next();
	STDMETHODIMP Pause();
	STDMETHODIMP Play();
	STDMETHODIMP PlayOrPause();
	STDMETHODIMP Prev();
	STDMETHODIMP Random();
	STDMETHODIMP RunContextCommand(BSTR command, UINT flags, VARIANT_BOOL* p);
	STDMETHODIMP RunContextCommandWithMetadb(BSTR command, VARIANT handle, UINT flags, VARIANT_BOOL* p);
	STDMETHODIMP RunMainMenuCommand(BSTR command, VARIANT_BOOL* p);
	STDMETHODIMP SavePlaylist();
	STDMETHODIMP SetDSPPreset(UINT idx);
	STDMETHODIMP SetOutputDevice(BSTR output, BSTR device);
	STDMETHODIMP ShowConsole();
	STDMETHODIMP ShowLibrarySearchUI(BSTR query);
	STDMETHODIMP ShowPopupMessage(BSTR msg, BSTR title);
	STDMETHODIMP ShowPreferences();
	STDMETHODIMP Stop();
	STDMETHODIMP TitleFormat(BSTR expression, ITitleFormat** pp);
	STDMETHODIMP VolumeDown();
	STDMETHODIMP VolumeMute();
	STDMETHODIMP VolumeUp();
	STDMETHODIMP get_AlwaysOnTop(VARIANT_BOOL* p);
	STDMETHODIMP get_ComponentPath(BSTR* p);
	STDMETHODIMP get_CursorFollowPlayback(VARIANT_BOOL* p);
	STDMETHODIMP get_FoobarPath(BSTR* p);
	STDMETHODIMP get_IsPaused(VARIANT_BOOL* p);
	STDMETHODIMP get_IsPlaying(VARIANT_BOOL* p);
	STDMETHODIMP get_PlaybackFollowCursor(VARIANT_BOOL* p);
	STDMETHODIMP get_PlaybackLength(double* p);
	STDMETHODIMP get_PlaybackTime(double* p);
	STDMETHODIMP get_ProfilePath(BSTR* p);
	STDMETHODIMP get_ReplaygainMode(UINT* p);
	STDMETHODIMP get_StopAfterCurrent(VARIANT_BOOL* p);
	STDMETHODIMP get_Volume(float* p);
	STDMETHODIMP put_AlwaysOnTop(VARIANT_BOOL p);
	STDMETHODIMP put_CursorFollowPlayback(VARIANT_BOOL p);
	STDMETHODIMP put_PlaybackFollowCursor(VARIANT_BOOL p);
	STDMETHODIMP put_PlaybackTime(double time);
	STDMETHODIMP put_ReplaygainMode(UINT p);
	STDMETHODIMP put_StopAfterCurrent(VARIANT_BOOL p);
	STDMETHODIMP put_Volume(float value);
};

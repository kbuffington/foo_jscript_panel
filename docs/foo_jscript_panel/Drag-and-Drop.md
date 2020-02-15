## Drag and Drop

- This is a topic for advanced users who are capable of writing their own playlist/library viewers!

## Incoming

To handle incoming files from other panels or Windows Explorer, there are 4 callbacks.

```js
function on_drag_drop(action, x, y, mask) {}
function on_drag_enter(action, x, y, mask) {}
function on_drag_leave() {}
function on_drag_over(action, x, y, mask) {}
```

First we'll look at the `action.Effect` property which is documented here.

[https://msdn.microsoft.com/en-us/library/windows/desktop/ms693457.aspx](https://msdn.microsoft.com/en-us/library/windows/desktop/ms693457.aspx)

- Rather than set variables for `DROPEFFECT_NONE` and `DROPEFFECT_COPY`, I'll just use `0` and `1` in the examples below.

When used inside the `on_drag_over` callback, it can be used to provide feedback to the user as to whether you can drop files there or not. If you set the value to `0`, the mouse pointer will change to show that dropping files is prohibited. To show that dropping files is allowed, we would use the value of `1`.

If you had a playlist viewer script, usage could be as simple as this:

```js
function on_drag_over(action, x, y, mask) {
	if (plman.PlaylistCount == 0) {
		// no playlists, can't drop.
		action.Effect = 0;
	} else if (plman.IsPlaylistLocked(plman.ActivePlaylist)) {
		// autoplaylist or playlist is set to read-only with foo_utils, can't drop.
		action.Effect = 0;
	} else {
		// ok, we can drop here.
		action.Effect = 1;
	}
}
```

When it comes to handling the dropped files inside `on_drag_drop`, there are 3 extra properties we need to use:

- Playlist `number`
- Base `number`
- ToSelect `boolean` (if `true`, select files and set focus to first new file)

We can use the exact same logic as before to set `action.Effect` as we need to do this so the `source` of our files gets notified of our intent. We should only ever use values of `0` or `1`.

```js
function on_drag_drop(action, x, y, mask) {
	if (plman.PlaylistCount == 0) {
		// no playlists, can't drop.
		action.Effect = 0;
	} else if (plman.IsPlaylistLocked(plman.ActivePlaylist)) {
 		// autoplaylist or playlist is set to read-only with foo_utils, can't drop.
		action.Effect = 0;
	} else {
		// use extra properties here
		action.Playlist = plman.ActivePlaylist;
		 // append to end
		action.Base = plman.PlaylistItemCount(plman.ActivePlaylist);
		action.ToSelect = true;
		action.Effect = 1;
	}
}
```

## Outgoing

- Sorry if this is vague but I barely understand the basic concepts myself and don't have much clue when it comes to working examples!

There is a new `fb.DoDragDrop` method which allows you to drag a handle list to external panels/Windows Explorer. It takes 2 arguments: a `handle list` and `allowed effect`. It also returns `effect` which notifies you what the destination did with the files.

Your mouse button should be down when calling the method but you typically won't do it inside the `on_mouse_lbtn_down` callback - you might call it inside `on_mouse_move` when you know the mouse button is down and the mouse has moved a certain distance from the original click point.

```js
var items = plman.GetPlaylistSelectedItems(plman.ActivePlaylist);
// effects can be combined
var effect = fb.DoDragDrop(items, 0|1);
// nothing happens here until the mouse button is released
items.Dispose();
```

If your script is a library viewer with no incoming files support, you really don't need to know or care about the returned effect - it's entirely up to the `destination` to handle the dropped files.

Things can be more complicated if you have a playlist viewer and you're not sure if you're doing to be dropping the files externally or within your own panel to move a selection within a playlist. For now, check this comment...

[https://github.com/marc2k3/foo_jscript_panel/pull/19#issuecomment-371827355](https://github.com/marc2k3/foo_jscript_panel/pull/19#issuecomment-371827355)

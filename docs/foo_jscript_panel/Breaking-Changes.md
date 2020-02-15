## Breaking Changes

### v2.3.0

Support for [foobar2000](https://foobar2000.org) `v1.3.x` has been dropped. `v1.4` is new minimum requirement.

The following methods no longer support the previously optional `force` parameter.

```js
window.Repaint();
window.RepaintRect(x, y, w, h);
ITitleFormat Eval() // returns an empty string when not playing
```

These previously optional parameters have been removed:

- fb.RunContextCommand(command) `flags` (all commands are ran if they exist)
- fb.RunContextCommandWithMetadb(command, handle_or_handles) `flags` (all commands are ran if they exist)
- IContextMenuManager BuildMenu `max_id`
- IMainMenuManager BuildMenu `count`
- utils.CheckComponent `is_dll`
- utils.WriteTextFile `write_bom` (all files are written as UTF8 without BOM)

Some examples of how old code would need updating

```js
window.Repaint(true);
// replace with window.Repaint();

fb.RunContextCommand("Blah", 8);
// replace with fb.RunContextCommand("Blah");

utils.WriteTextFile("z:\\abc.txt", "abc", false);
// replace with utils.WriteTextFile("z:\\abc.txt, "abc");
```

If you really need to support this updated component and old versions at the same time, you can do this:

```js
if (utils.Version < 2300) {
	fb.RunContextCommand("Blah", 8);
} else {
	fb.RunContextCommand("Blah");
}
```

The `IPlaylistRecyclerManager` interface has been removed and replaced with `plman` properties and methods. All previous functionality remains. See `plman.RecyclerCount` for a full description with examples.

```js
var r = plman.PlaylistRecyclerManager; // no replacement
r.Count // replaced with plman.RecyclerCount
r.Content(idx) // replaced with plman.GetRecyclerItems(idx)
r.Name(idx) // replaced with plman.GetRecyclerName(idx)
r.Purge(arr) // replaced with plman.RecyclerPurge(arr)
r.Restore(idx) // replaced with plman.RecyclerRestore(idx)
```

`fb.GetLibraryRelativePath` has been removed as it's unlikely to be used on its own. Use `IMetadbHandleList` `GetLibraryRelativePaths` instead.

`utils.FileTest` has been removed and replaced with the following methods:

```js
utils.Chardet(filename)
utils.GetFileSize(filename)
utils.IsFile(filename)
utils.IsFolder(folder)
```

There is no replacement for `split` mode. It's easy enough to do with plain Javascript. Ask in [issues](https://github.com/marc2k3/foo_jscript_panel/issues) if you need help.

### v2.2.2

The `IGdiBitmap` `GetColourScheme` method has been removed. Use the better `GetColourSchemeJSON` instead.

Old code...

```js
var arr = []; // array to store colours
var img = utils.GetAlbumArtV2(fb.GetNowPlaying(), 0);
if (img) {
	arr = img.GetColourScheme(5).toArray();
}
if (arr.length) {
	// You'd now access the colours with arr[0], arr[1] etc
}
```

Updated code
```js
var arr = []; // array to store colours
var img = utils.GetAlbumArtV2(fb.GetNowPlaying(), 0);
if (img) {
	arr = JSON.parse(img.GetColourSchemeJSON(5));
}
if (arr.length) {
	// You'd now access the colours with arr[0].col, arr[1].col etc
}
```

### v2.2.0

`window.DlgCode` has been removed as the previous documentation was wrong and default behaviour wasn't consistent across `Default UI` and `Columns UI` so I've decided to remove it. So long as `Grab focus` is enabled in the [Configuration Window](Configuration-Window.md), support for all keys will be enabled.

All instances will need removing from scripts. If you need to support scripts that will work in this and previous versions of the component, you can replace the code with this:

```js
if ('DlgCode' in window) { window.DlgCode = 4; } // DLGC_WANTALLKEYS = 4
```

`fb.RunMainMenuCommand`, `fb.RunContextCommand`, `fb.RunContextCommandWithMetadb` have all been rewritten and must be supplied with the full path to their commands. Case is not important. You should use forward slashes with no spaces.

Commands like this will no longer work:

```js
fb.RunContextCommand('Rating/5'); // foo_playcount command
```

It will need updating to:

```js
fb.RunContextCommand('Playback Statistics/Rating/5');
```

### v2.1.0

The drag/drop functionality from external sources has been completely rewritten and this affects the included `JS Smooth Playlist` and `JSPlaylist-mod` scripts. From now on, dragging files on to them appends to the end of the playlist and you can no longer insert in a specific place. For this reason, [v2.0.6](Old-Versions.md) is still available if you want to go back.

For anyone maintaining their own scripts, they should see the this [page](Drag-and-Drop.md).

Users of `plman.AddLocations` should check the updated docs as there have been [minor changes](https://github.com/marc2k3/foo_jscript_panel/commit/67e9d9fbd7aafeca06ca56c56340f42632813ded) to the behaviour if the `select` argument is set to `true`.

### v2.0.3

`fb.ShowPopupMessage` was documented to accept an optional `iconid` argument. It seems this has never worked so it has been removed. Any script that specifies it will now throw an error.

The `IMenuObj` `EnableMenuItem` method has been removed. Consider using flags like `MF_GRAYED`/`MF_DISABLED` on `AppendMenuItem` instead. Also, the optional `bypos` arguments for `CheckMenuItem` and `CheckMenuRadioItem` have both been removed. See the updated [Interfaces](Interfaces.md) page for full details.

### v2.0.0

The component requires [foobar2000](https://foobar2000.org) `v1.4` or later. There have been some minor changes to the [Configuration Window](Configuration-Window.md).

All users of my previous scripts in the `complete` folder should import the updated `.txt` files in to their panels. Installing the [FontAwesome](https://github.com/FortAwesome/Font-Awesome/blob/fa-4/fonts/fontawesome-webfont.ttf?raw=true) font is now a requirement.

Anyone who has a `js_settings` folder in their [foobar2000](https://foobar2000.org) profile folder from using my previous samples should move the contents in to the `js_data` folder. Typically, this would be `autoplaylists.json`, `thumbs.ini`, `listenbrainz.ini`.

`fb.Trace` has been removed and replaced with a native `console.log` method so you'll need to do a simple find/replace in all your scripts. It takes multiple arguments as before.

All callbacks/methods with `Color` in the name have been renamed with `Colour`. Again, a simple find/replace should suffice.

```
on_colors_changed -> on_colours_changed
utils.ColorPicker -> utils.ColourPicker
utils.GetSysColor -> utils.GetSysColour
window.GetColorCUI -> window.GetColourCUI
window.GetColorDUI -> window.GetColourDUI
IGdiBitmap GetColorScheme -> GetColourScheme
```

The following `Playback Queue` methods have been removed.

```
plman.CreatePlaybackQueueItem()
plman.GetPlaybackQueueCount()
plman.IsPlaybackQueueActive()
```

- Update 03/03/2018. `plman.GetPlaybackQueueContents()` was removed in `v2.0.0` but restored in `v2.0.5`.

A new `plman.GetPlaybackQueueHandles()` method has been created and this returns a `handle list`. You can check the `Count` property to see if the `Playback Queue` is active or not.

`on_refresh_background_done` and `window.GetBackgroundImage` have both been removed.

The `IGdiBitmap` `BoxBlur` method has been removed. Try `StackBlur` instead. Updated samples have been included in the `basic` folder.

`IContextMenuManager` `InitContext` only accepts a `handle list` as an argument and no longer a `handle` like it did previously.

### v1.3.1

Remove the whole `IStyleTextRender` interface. Anything that uses `gdi.CreateStyleTextRender` or was based on `glow text sample.txt` will no longer work

### v1.3.0

The `UpdateFileInfoSimple` `handle` and `handle list` methods have been dropped.

The replacement is a `handle list` only method which requires a bit more explanation than can go in the plain text docs so I'm explaining here. An understanding of [JSON](http://json.org/) will help with this!!

The name is `UpdateFileInfoFromJSON`. It takes either an `array` or an `object` as the argument - and this must be ran through the `JSON.stringify` function before using.

This first example uses an `array` and its length must match the `handle list` count. A fail safe way of doing this is looping through the `handle list` and filling the array as you go.

```javascript
// assume we've selected one album
var handles = plman.GetPlaylistSelectedItems(plman.ActivePlaylist);

// an empty array
var arr = [];

for (var i = 0; i < handles.Count; i++) {
	// Each element of the array must be an object
	// of key names/values, indicated by the curly braces.
	arr.push({
		"tracknumber" : i + 1, // independent values per track are now supported
		"totaltracks" : handles.Count,
		"album" : "Greatest Hits", // a simple string for a single value
		"genre" : ["Rock", "Hard Rock"] // we can use an array here for multiple value tags
	});
}

var str = JSON.stringify(arr);

handles.UpdateFileInfoFromJSON(str);
handles.Dispose();
```

If you want to write the exact same tags to each and every track in the `handle list`, we can use an `object` instead.

```javascript
var handles = plman.GetPlaylistSelectedItems(plman.ActivePlaylist);

var obj = {
	"album" : "Greatest Hits",
	"genre" : ["Rock", "Hard Rock"] // again we can write multiple values
};

var str = JSON.stringify(obj);
handles.UpdateFileInfoFromJSON(str);
handles.Dispose();
```

Since there is no method for single handles, you can use the new `fb.CreateHandleList()` method to do something like...

```javascript
var handle = fb.GetFocusItem();
var handles = fb.CreateHandleList();
handles.Add(handle);
handles.UpdateFileInfoFromJSON(JSON.stringify({ "artist" : "marc2003" }));
handles.Dispose();
```

As before, you can use blank values to clear any existing tags.

```javascript
var obj = {
	"album" : ""
};
```

Do not try using any method of using empty objects or blank tag names to avoid tagging any files. You should filter your `handle list` first to make sure it only contains files you really want to update.

### v1.2.0

The following interfaces will require IE9 or later. There is a reminder next to each one in the docs.

```
IMetadbHandle FileSize
IMetadbHandleList CalcTotalSize
```

### v1.1.0

`plman.GetQueryItems` has been deprecated and replaced with `fb.GetQueryItems`. Scripts can be updated by doing a simple find/replace in any text editor.

### v1.0.0

See the [changelog](https://github.com/marc2k3/foo_jscript_panel/blob/master/CHANGELOG.md#v100-compared-to-wsh-panel-mod-156).

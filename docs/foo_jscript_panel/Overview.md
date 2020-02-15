## Overview

This component is based on [WSH Panel Mod](https://code.google.com/archive/p/foo-wsh-panel-mod/).

It allows the creation of customisable panels that can be written with `Javascript` rather than the `C++` required by the [foobar2000 SDK](https://www.foobar2000.org/SDK).

Under the hood, it uses `Windows Script host`. It is possible to use `ActiveX` objects like `FileSystemObject` and `XMLHttpRequest` to work with the local file system, access the internet, etc.

If your system has `IE9` or later installed, you'll have access to `ECMAScript5` features. You'll need to make sure the `Chakra` script engine is selected in the [Configuration Window](Configuration-Window.md).

As of `v2`, `Windows XP SP3` is now supported and support for `Vista/7/WINE` with `IE8` has been restored. Of course you'll be limited here to `ECMAScript3`!

Here are just some of the features provided by the component...

* Custom drawing of text, external images, lines, rectangles, etc.
* Use fonts/colours from the main preferences of whichever user interface you are using.
* Executing main/context menu commands.
* Ability to create custom buttons/menus.
* Capture keystrokes/mouse movement/clicks.
* Callbacks can be used to trigger code based on [foobar2000](https://foobar2000.org) events. See [callbacks.txt](https://raw.githubusercontent.com/marc2k3/foo_jscript_panel/master/foo_jscript_panel/docs/Callbacks.txt).
* Read/write file tags.
* Complete manipulation of playlists.
* Media Library display/sorting/filtering
* Save settings on a per panel basis. You can also save/load settings from .ini files or write your own functions to read and write plain text/JSON files etc.
* And much more...

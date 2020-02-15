## WINE

Since this subject occasionally crops up, I've tested `JScript Panel` under `WINE` with `Ubuntu 19.04`.

Use this command to install `WINE`:

```
sudo apt-get install wine32
```

Then run the following:

```
sudo apt-get install cabextract
wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks
chmod +x winetricks
sh winetricks gdiplus_winxp wsh57 fontsmooth=rgb msscript ie8
```

- Note that last command appears to download `Windows XP SP3` which is just over 300MB in size!

That should be it!

For scripts that require custom fonts, I found that copying fonts to `drive_c\windows\fonts` in the `.wine` folder didn't work but creating a `.fonts` folder inside your `HOME` directory works fine.

Although the component and most scripts work fine, you may find there are individual scripts that won't work.
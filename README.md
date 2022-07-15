# set-electron-app-accessible

This cli app tells an electron app to turn on accessibility, so it can be controlled by AppleScript.

# Build

Make sure you have the xcode cli tools installed and just simply run

```bash
make
```

# Run

```bash
./set-electron-app-accessible PID
```

Where PID is the process id of the app you are trying to control.

You can do this in in AppleScript by process name by doing:
```applescript
tell application "System Events"
    set pid to unix id of first process whose name is "The Electron App's Name"
    do shell script "./set-electron-app-accessible " & pid
end tell
```

# Attribution

This code is based on code given at:
* https://github.com/dane-harnett/workspace
* https://www.electronjs.org/docs/latest/tutorial/accessibility
* https://github.com/koekeishiya/skhd/blob/master/src/skhd.c

# unikware usage management
This software will start [unikware_systray](https://github.com/qoopooh/try_git/tree/master/qt4/unikware_systray) and unikware.exe.
unikware_systray will monitoring unikware.exe process every 2 seconds, if unikware runs over x minutes unikware_systray will show pop up to user to extend the time usage.
If user has no action, unikware_systray will kill unikware.exe and itself.


# Set up

## Tools
Please download [qt4 here](https://download.qt.io/archive/qt/4.7/qt-win-opensource-4.7.4-mingw.exe.mirrorlist).
We also need MinGW which is included in [Qt Creator](https://download.qt.io/archive/qtcreator/2.1/qt-creator-2.1.0-src.zip.mirrorlist)


# Deployment
Unikware.exe is located at [N:\Program](\\192.168.1.16\unikware). unikware_launcher.exe has to stay near by Unikware.exe, otherwise we might get German language loading problem.
## required libraries
We develop programs based on qt 4.7.4 with MinGW
### unikware_launcher.exe
1. libgcc_s_dw2-1.dll
2. mingwm10.dll
3. QtCore4.dll
### unikware_systray.exe
We have to deploy this program into [N:\Program\UnikwareMonitor]
1. libgcc_s_dw2-1.dll
2. mingwm10.dll
3. libmysql.dll
4. QtCore4.dll
5. QtGui4.dll
6. QtNetwork4.dll
7. QtSql4.dll


# Revision History
0.10 - Set timeout to 30 minutes
0.8 - Set timeout to 1 hour
0.4 - git snapshot

@echo off
rem PATH=C:\Program Files\WinAVR-20060421\bin;C:\Program Files\WinAVR-20060421\utils\bin;%PATH%

if "%ONVIM%" == "yes" (
	make -s -j2 -w -C %*
) else (
	make -s -j2 %*
	pause
)


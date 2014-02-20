@echo off
rem PATH=C:\Program Files\WinAVR-20060421\bin;C:\Program Files\WinAVR-20060421\utils\bin;%PATH%

set VIM_MAKEOPT=-s -j2 -w -C %*
if "%ONVIM%" == "yes" (
	cd %1
	(
		echo %% make %VIM_MAKEOPT%
		make %VIM_MAKEOPT%
		"C:\Program Files\WinAVR-20060421\utils\bin\date"
	) 2>&1 | tee log.txt
) else (
	make -s -j2 %*
	pause
)


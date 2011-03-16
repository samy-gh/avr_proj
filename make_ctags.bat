if "X%CTAGS_CMD%" == "X" (
	set CTAGS_CMD=C:\wintools\ctags\ctags
)
if "X%WINAVR_INCLUDE%" == "X" (
	set WINAVR_INCLUDE=C:\Program Files\WinAVR-20100110\avr\include
)

"%CTAGS_CMD%" -R --tag-relative=yes samylib
dir /B "%WINAVR_INCLUDE%\avr\io*.h" > ctags_exclude_files.txt
"%CTAGS_CMD%" -a -R --tag-relative=yes --exclude=@ctags_exclude_files.txt "%WINAVR_INCLUDE%"
"%CTAGS_CMD%" -a --tag-relative=yes "%WINAVR_INCLUDE%\avr\iom328p.h"
"%CTAGS_CMD%" -a --tag-relative=yes "%WINAVR_INCLUDE%\avr\iom168p.h"
"%CTAGS_CMD%" -a --tag-relative=yes "%WINAVR_INCLUDE%\avr\iotn2313.h"
"%CTAGS_CMD%" -a --tag-relative=yes "%WINAVR_INCLUDE%\avr\io.h"

@echo off
@rem PATH=c:\mingw\bin;c:\msys\1.0\bin;%PATH%

@rem ir_aircon <auto|cooler|heater|dehumi> <temp 17.0~30.0> <norm|powerful|saving> <auto|silent|slow|mid|fast>
@rem ir_aircon <dehumi>
@rem ir_aircon <stop>

ir_aircon.exe heater 17.5 norm auto
@rem ir_aircon.exe heater 30.0 saving slow
@rem ir_aircon.exe cooler 17.0 norm auto
@rem ir_aircon.exe dehumi 55 norm

pause

@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
cl /EHsc /std:c++20 /utf-8 /I D:/DEV/std_string/include test_string_algo.cpp /Fe:test.exe /Fo:test_obj.o 2>&1
echo ExitCode: %ERRORLEVEL%

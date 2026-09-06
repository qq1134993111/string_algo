call "C:/Program Files/Microsoft Visual Studio/18/Professional/VC/Auxiliary/Build/vcvarsall.bat" x64 >nul 2>&1
test.exe > test_output.txt 2>&1
echo ExitCode: %ERRORLEVEL%

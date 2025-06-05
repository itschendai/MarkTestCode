@echo off
REM Rename a folder
rename "libraries" "lib"

REM Rename a folder
rename "ui" "src"

REM Rename (move) ui.ino to main.cpp inside the new folder
move "src\ui.ino" "src\main.cpp"

echo Rename complete.
pause

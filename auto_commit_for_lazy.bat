@echo off
set /p "msg=Enter Commit Message: "
git add *
git commit -m "%msg%"
git push

PAUSE

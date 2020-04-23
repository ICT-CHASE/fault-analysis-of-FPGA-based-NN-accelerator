@echo off

echo put npu's output in ./output/
echo put errors' list in ./error_list.txt
set /p picnum="pic num: "

if exist "pic" (
    rd /s /q pic
)

if exist "compare" (
    rd /s /q compare
)

mkdir pic
mkdir compare

for /l %%i in (1,1,%picnum%) do (
    :: echo %%i
    out2pic.exe output\%%i pic\%%i.jpg
)

SSIM.exe %picnum%

pause
@echo off

echo will delete:
echo ./output/
echo ./compare/
echo ./pic/
echo ./error_list.txt
echo continue?
pause
rd /s /q output
rd /s /q compare
rd /s /q pic
del error_list.txt
pause

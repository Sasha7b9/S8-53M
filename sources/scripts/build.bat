@echo off

cd keil
call build.bat all
cd ..

cd vs_keil
call build.bat
cd ..

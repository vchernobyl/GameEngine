@echo off

set application=Game
set compile_flags=/nologo /DDEBUG /EHsc /MP /Zi /std:c++17
set link_flags=/DEBUG:FASTLINK /SUBSYSTEM:CONSOLE

set include_flags=/I ..\external\SDL\include^
    /I ..\external\glew\include^
    /I ..\external\glm\include

set libs=shell32.lib^
    ..\external\SDL\lib\x64\SDL2.lib^
    ..\external\SDL\lib\x64\SDL2main.lib^
    ..\external\glew\lib\x64\glew32.lib^
    opengl32.lib
    
if not exist build mkdir build
pushd build
cl %compile_flags% %include_flags% ..\src\*.cpp %libs% /link %link_flags% /out:%application%.exe
popd

copy external\SDL\lib\x64\SDL2.dll .\build
copy external\glew\lib\x64\glew32.dll .\build

echo.
echo [Build completed]

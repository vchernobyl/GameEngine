@echo off

set application=Game
set compile_flags=/nologo /DDEBUG /EHsc /MP /Zi /std:c++17
set link_flags=/DEBUG:FASTLINK /SUBSYSTEM:CONSOLE

set include_flags=/I ..\external\SDL\include^
    /I ..\external\glew\include^
    /I ..\external\glm\include

set libs=shell32.lib^
    ..\external\SDL\lib\x64\SDL2.lib^
    ..\external\SDL\lib\x64\SDL2_ttf.lib^
    ..\external\SDL\lib\x64\SDL2_mixer.lib^
    ..\external\SDL\lib\x64\SDL2main.lib^
    ..\external\glew\lib\x64\glew32.lib^
    opengl32.lib
    
if not exist build mkdir build
pushd build
cl %compile_flags% %include_flags%^ ..\src\*.cpp ..\src\ZombieGame\*.cpp ..\src\NinjaGame\*.cpp %libs% /link %link_flags% /out:%application%.exe
popd

if not exist .\build\SDL2.dll copy external\SDL\lib\x64\SDL2.dll .\build
if not exist .\build\SDL2_ttf.dll copy external\SDL\lib\x64\SDL2_ttf.dll .\build
if not exist .\build\SDL2_mixer.dll copy external\SDL\lib\x64\SDL2_mixer.dll .\build
if not exist .\build\libfreetype-6.dll copy external\SDL\lib\x64\libfreetype-6.dll .\build
if not exist .\build\glew32.dll copy external\glew\lib\x64\glew32.dll .\build

echo.
echo [Build completed]

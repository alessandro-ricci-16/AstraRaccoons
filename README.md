# AstraRaccoons - Computer Graphic Project

<img src="textures/icon.png" width=192px height=192px align="right"  alt="AstraRaccoons Logo"/>

Astra Raccoons is the final test of **"Computer Graphics"**, course of **"Computer Science Engineering"** 
held at Politecnico di Milano (2022/2023). <br />

**Professor**: [Marco Gribaudo](http://gribaudo.faculty.polimi.it/)

**The team**: 
- [Alessandro Ricci](https://github.com/alessandro-ricci-16)
- [Alessandro Sassi](https://github.com/AleSassi)
- [Andrea Sanguineti](https://github.com/AndreaNeti)

**Final Grade**: 30/30 *cum* laude ( ͡° ͜ʖ ͡°)

## Project info
The project consists of a 3D version of the game *Asteroids* done in C++ with Vulkan.

## Compile

Windows User:

1. create a bat file containing the following [code](https://media.licdn.com/dms/image/C4E03AQFkRf6_tRxaIg/profile-displayphoto-shrink_800_800/0/1517599267364?e=1713398400&v=beta&t=dL4Ahw6dhMU5JU5sY-gVuBVWVUrgJQGw932EW2Y6D2w):

   ```
    	@echo off
	chcp 65001
	setlocal enabledelayedexpansion
	
	set "ProjDir=<Project_Dir>"
	set "VulkDir=<Vulkan_Dir>"
	set "VulkVers=<Vulkan_Version>"
	set "GlslcDir=!VulkDir!\!VulkVers!\Bin\glslc.exe"
	set "Mingw64=<Mingw64_Dir>"
	
	set "compileOnlyShaders=false"
	IF "%1"=="-s" (
	    set "compileOnlyShaders=true"
	)
	
	cd "%ProjDir%"
	set "files="
	
	for /f "delims=" %%f in ('dir /s /b .\src\*.cpp') do (
	    set "file=%%~dpnxf"
	    set "file=.!file:%ProjDir%=!"
	    set "files=!files! !file!"
	)
	
	IF "%compileOnlyShaders%"=="false" (
		echo Compiling...
		g++ -w -std=c++17 -I"%ProjDir%" -I"%VulkDir%\%VulkVers%\Include" -I"%VulkDir%\glfw-3.3.8.bin.WIN64\include" -L"%VulkDir%\glfw-3.3.8.bin.WIN64\lib-mingw-w64" -L"%ProjDir%\glm" -L"%Mingw64%\lib" %files% -l:libglfw3.a -lgdi32 "%VulkDir%\%VulkVers%\Lib\vulkan-1.lib" -o .\AstraRaccoons
		IF %ERRORLEVEL% NEQ 0 (
			echo Compilation failed
		) ELSE (
			echo Compilation successful
		)
	)
	IF %ERRORLEVEL% NEQ 1 (    
	    echo Compiling shaders...
	
	    for /f "delims=" %%f in ('dir /s /b .\shaders\*.vert') do (
	        "%GlslcDir%" "%%~dpnxf" -o "%%~dpnf_Vert.spv"
	    )
	
	    for /f "delims=" %%f in ('dir /s /b .\shaders\*.frag') do (
	        "%GlslcDir%" "%%~dpnxf" -o "%%~dpnf_Frag.spv"
	    )
	    echo Done
	    echo Starting...
	    .\AstraRaccoons.exe
	)
	
	@echo on
	```

2. Run the file .bat and enjoy :)

### Commands

|     Commands     |   Actions   |
|:----------------:|:-----------:|
|         W        |  Accelerate |
|         Q        |   Roll Left |
|         E        |  Roll Right |
|   Up/Down Arrow  |    Pitch    |
| Left/Right Arrow |     Yaw     |
|     Space Bar    |    Shoot    |

### Recommendations

It is required to have installed [Mingw64](https://www.mingw-w64.org/downloads/) and [Vulkan](https://vulkan.lunarg.com/) to compile and run the project.

### External Assets

[Skybox](https://gamebanana.com/mods/285684)  
[Skybox License](https://creativecommons.org/licenses/by-nc-nd/4.0/)  
[Spaceship](https://sketchfab.com/3d-models/low-poly-spaceship-de307645e6c84b3a8046bd6fcbb7e744) - In the project with different colors.  
[Spaceship License](https://creativecommons.org/licenses/by-nd/4.0/)  




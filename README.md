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

## [Compile](https://media.licdn.com/dms/image/C4E03AQFkRf6_tRxaIg/profile-displayphoto-shrink_800_800/0/1517599267364?e=1713398400&v=beta&t=dL4Ahw6dhMU5JU5sY-gVuBVWVUrgJQGw932EW2Y6D2w)

Windows User:

1. create a bat file containing the following code:

   ```
    @echo off
	chcp 65001
	setlocal enabledelayedexpansion
	
	set "ProjDir=C:\Users\alexe\source\repos\alessandro-ricci-16\AstraRaccoons\AstraRaccoons"
	set "VulkDir=C:\VulkanSDK"
	set "VulkVers=1.3.246.0"
	set "GlslcDir=!VulkDir!\!VulkVers!\Bin\glslc.exe"
	set "Mingw64=C:\Program Files\msys64\mingw64"
	
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

|   Commands    |   Actions    |
|:-------------:|:------------:|
|       W       |  Accelerate  |
|       Q       |   Roll Left  |
|       E       |  Roll Right  |
|    Up Arrow   |   Pitch Up   |
|   Down Arrow  |  Pitch Down  |
|   Left Arrow  |   Yaw Left   |
|  Right Arrow  |   Yaw Right  |
|   Space Bar   |    Shoot     |







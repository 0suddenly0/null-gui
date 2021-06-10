# null-gui
gui framework

![image](https://user-images.githubusercontent.com/60057961/121350610-57077e00-c944-11eb-97a8-76ba754277d9.png)

Created only by the fact that there was nothing more to do and the idea of making your own gui was brewing for a long time.
Perhaps, if someone wants to make their own gui, they can pick something up for their project (although most of the code is blueprint with ImGui).

Warning! The code has a huge amount of shit and other garbage that someday, perhaps, will be fixed.

# Project settings
Pixel shaders:  
* blur_x
* blur_y
* pixel (only in dx11)

Vertex shaders:  
* vertex

To compile shaders correctly use these settings:  
**HLSL Compiler -> Output Files -> Header Variable Name | "%(Filename)"  
HLSL Compiler -> Output Files -> Header File Name | "%(RootDir)%(Directory)..\compiled\\%(Filename).h"  
HLSL Compiler -> Output Files -> Object File Name | empty  
HLSL Compiler -> General -> Shader Type | for pixel shaders "Pixel Shader (/ps)", for vertex shaders "Vertex Shader (/vs)"**

For directx9:  
**HLSL compiler -> General -> Shader Model | "Shader Model 3 (/3_0)"**

For directx11:  
**HLSL compiler -> General -> Shader Model | "Shader Model 4 (/4_0)"**

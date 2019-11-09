#pragma once
#ifndef BFENGINE
#define BFENGINE

#include <Windows.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

#include"BFMath.h"
#include"BFMesh.h"
#include"BFTypes.h"

#include"BFCamera.h"

#include"BFResourceManager.h"

#include"BFGeometry.h"


//#include"BFRenderPipeline.h"
//#include"BFDeviceRenderer.h"
//#include"BFDeviceRendererWin32.h"

#define MSG(msg) ::MessageBoxA(0, msg, 0, 0)
#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define PRINT(msg) std::cout<<msg<<std::endl

#endif // !BFENGINE

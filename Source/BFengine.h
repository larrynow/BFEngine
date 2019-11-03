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

#endif // !BFENGINE

#include "BFEngine.h"
#include "BFDeviceRendererWin32.h"
#include<Windows.h>

#pragma comment(lib, "winmm.lib ")

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}

int main()
{
	BFResourceManager resourceManager;
	//Mesh* rockMesh = new Mesh();
	//Texture* rockTexture = new Texture();
	//bool loaded = resourceManager.ImportMesh_OBJ("../Asset/rock2.obj", rockMesh);
	//loaded = resourceManager.ImportTexture_PPM("../Asset/rock.ppm", rockTexture);
	//rockMesh->BindTexture(rockTexture);

	//loaded = resourceManager.ImportTexture_BMP("../Asset/car.bmp", texture);
	Mesh* cubeMesh = new Mesh();
	Texture* cubeTexture = new Texture();
	BFGeometry::CreateCubeMesh(100.f, *cubeMesh);
	resourceManager.ImportTexture_BMP("../Asset/awesomeface.bmp", cubeTexture);


	cubeMesh->BindTexture(cubeTexture);

	IBFDeviceRendererWin32 deviceRenderer;
	deviceRenderer.Init(800, 600);

	int j = 300;
	auto RGB2Color = [](UINT r, UINT g, UINT b) {return COLOR3(r > 255 ? 1.0f : float(r) / 255, g > 255 ? 1.0f : float(g) / 255, b > 255 ? 1.0f : float(b) / 255); };
	COLOR3 color = RGB2Color(65, 105, 225);//RoyalBlue	65 105 225
	deviceRenderer.SetClearColor();

	DWORD lastTime = 0;
	DWORD curTime;
	UINT frame = 0;
	//mesh->RotateWithY(5.f);
	while (true)
	{
		curTime = timeGetTime();
		std::string fpsStr = std::to_string(1*1000 / float(curTime - lastTime));
		lastTime = curTime;
		//::SetConsoleTitleA(fpsStr.c_str());
		std::cout << fpsStr << std::endl;
		deviceRenderer.Clear();
		//deviceRenderer.DrawLine(color, 0, 0, 200, 300);
		//deviceRenderer.DrawTriangle(color, VEC2(100, 100), VEC2(200, 100), VEC2(150, 200));
		//mesh->MoveTo({ 0.f, 0.f, i });

		//rockMesh->RotateWithY(5.f);
		//deviceRenderer.RenderMesh(rockMesh);

		//cubeMesh->SetScale(100.f);
		//cubeMesh->MoveTo({ 0.f, 10.f, 0.f });
		cubeMesh->RotateWithY(1.f);
		deviceRenderer.RenderMesh(cubeMesh);

		deviceRenderer.Display();
	}
	//while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	//{
	//	deviceRenderer.Display();
	//	//screen_dispatch();
	//}
}

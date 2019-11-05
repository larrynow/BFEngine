//#include "BFEngine.h"
//#include "BFDeviceRendererWin32.h"
#include"BFContent.h"
#include<Windows.h>

#pragma comment(lib, "winmm.lib ")

int main()
{
	BFContent* content = new BFContent();
	content->StartUp(BFDevice_t::WIN_32, 800, 600);

	BFResourceManager* resourceManager = content->m_pContentResourceManager;
	IBFDeviceRenderer* renderer = content->m_pContentDeviceRenderer;

	Texture* cubeTexture = new Texture();
	Mesh* cubeMesh = new Mesh();
	BFGeometry::CreateCubeMesh(100.f, *cubeMesh);
	resourceManager->ImportTexture_BMP("../Asset/awesomeface.bmp", cubeTexture);
	cubeMesh->BindTexture(cubeTexture);

	// Note, the BindInput should be excuted in actor(or controller) class.
	content->RegisterInput(BFInput::KEY_W, "forward");
	content->BindInput("forward", std::bind(&ABFActor::MoveForward, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_S, "backward");
	content->BindInput("backward", std::bind(&ABFActor::MoveBack, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_A, "left");
	content->BindInput("left", std::bind(&ABFActor::MoveLeft, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_D, "right");
	content->BindInput("right", std::bind(&ABFActor::MoveRight, content->m_pControlledActor));
	//content->RegisterInput(BFInput::KEY_D, "up");
	//content->BindInput("up", std::bind(&ABFActor::MoveUp, content->m_pControlledActor));
	//content->RegisterInput(BFInput::KEY_D, "down");
	//content->BindInput("down", std::bind(&ABFActor::MoveDown, content->m_pControlledActor));

	//BFResourceManager resourceManager;
	////Mesh* rockMesh = new Mesh();
	////Texture* rockTexture = new Texture();
	////bool loaded = resourceManager.ImportMesh_OBJ("../Asset/rock2.obj", rockMesh);
	////loaded = resourceManager.ImportTexture_PPM("../Asset/rock.ppm", rockTexture);
	////rockMesh->BindTexture(rockTexture);

	////loaded = resourceManager.ImportTexture_BMP("../Asset/car.bmp", texture);
	//Mesh* cubeMesh = new Mesh();
	//Texture* cubeTexture = new Texture();
	//BFGeometry::CreateCubeMesh(100.f, *cubeMesh);
	//resourceManager.ImportTexture_BMP("../Asset/awesomeface.bmp", cubeTexture);


	//cubeMesh->BindTexture(cubeTexture);

	//IBFDeviceRendererWin32 deviceRenderer;
	//deviceRenderer.Init(800, 600);

	int j = 300;
	auto RGB2Color = [](UINT r, UINT g, UINT b) {return COLOR3(r > 255 ? 1.0f : float(r) / 255, g > 255 ? 1.0f : float(g) / 255, b > 255 ? 1.0f : float(b) / 255); };
	COLOR3 color = RGB2Color(65, 105, 225);//RoyalBlue	65 105 225
	renderer->SetClearColor();

	DWORD lastTime = 0;
	DWORD curTime;
	UINT frame = 0;
	//mesh->RotateWithY(5.f);
	while (true)
	{
		screen_dispatch();
		curTime = timeGetTime();
		std::string fpsStr = std::to_string(1*1000 / float(curTime - lastTime));
		lastTime = curTime;
		//::SetConsoleTitleA(fpsStr.c_str());
		//std::cout << fpsStr << std::endl;
		renderer->Clear();
		//deviceRenderer.DrawLine(color, 0, 0, 200, 300);
		//deviceRenderer.DrawTriangle(color, VEC2(100, 100), VEC2(200, 100), VEC2(150, 200));
		//mesh->MoveTo({ 0.f, 0.f, i });

		//rockMesh->RotateWithY(5.f);
		//deviceRenderer.RenderMesh(rockMesh);

		//cubeMesh->SetScale(100.f);
		//cubeMesh->MoveTo({ 0.f, 10.f, 0.f });
		
		// Input.
		//std::cout << screen_keys[87] << std::endl;
		for (auto it = content->input_name_map.begin(); it != content->input_name_map.end(); ++it)
		{
			auto key = (*it).first;
			if (BFContent::screen_keys[int(key) - int(BFInput::KEY_A)+65] == 1)
			{
				auto input = (*it).second;
				auto op = content->input_op_map.at(input);
				op();
			}
		}
		//if (BFContent::screen_keys[87]==1)
		//{
		//	//std::cout << BFContent::screen_keys[87] << std::endl;

		//	auto input = content->input_name_map[BFInput::KEY_W];
		//	auto op = content->input_op_map.at(input);
		//	op();
		//}

		cubeMesh->RotateWithY(1.f);
		renderer->RenderMesh(cubeMesh);



		renderer->Display();
		
	}
	//while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0)
	//{
	//	deviceRenderer.Display();
	//	//screen_dispatch();
	//}
}

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

	Texture* coolTexture = new Texture();
	Mesh* cuteMesh = new Mesh();
	resourceManager->ImportMesh_OBJ("../Asset/nanosuit.obj", cuteMesh);
	//resourceManager->ImportTexture_PPM("../Asset/rock.ppm", coolTexture);
	resourceManager->ImportTexture_BMP("../Asset/awesomeface.bmp", coolTexture);
	cuteMesh->BindTexture(coolTexture);

	BFContent::m_pLights[0] = new SpotLight(VEC3{0.f, 0.f, -10.f});

	// Note, the BindInput should be excuted in actor(or controller) class.
	content->RegisterInput(BFInput::KEY_W, "forward");
	// content->BindInput("forward", std::bind(&ABFActor::MoveForward, content->m_pControlledActor));
	// Replace std::bind with lambda for ckeckable and inline faster.
	content->BindInput("forward", [content]() {if (content->m_pControlledActor) content->m_pControlledActor->MoveForward(); });
	content->RegisterInput(BFInput::KEY_S, "backward");
	content->BindInput("backward", std::bind(&ABFActor::MoveBack, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_A, "left");
	content->BindInput("left", std::bind(&ABFActor::MoveLeft, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_D, "right");
	content->BindInput("right", std::bind(&ABFActor::MoveRight, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_SPACE, "up");
	content->BindInput("up", std::bind(&ABFActor::MoveUp, content->m_pControlledActor));
	content->RegisterInput(BFInput::KEY_CTRL, "down");
	content->BindInput("down", std::bind(&ABFActor::MoveDown, content->m_pControlledActor));

	int j = 300;
	auto RGB2Color = [](UINT r, UINT g, UINT b) {return COLOR3(r > 255 ? 1.0f : float(r) / 255, g > 255 ? 1.0f : float(g) / 255, b > 255 ? 1.0f : float(b) / 255); };
	COLOR3 color = RGB2Color(65, 105, 225);//RoyalBlue	65 105 225
	renderer->SetClearColor();

	DWORD lastTime = 0;
	DWORD curTime;
	UINT frame = 0;
	//float max_fps = -1;
	cuteMesh->RotateWithY(1.f);

	while (!BFContent::ShouldFinish())
	//for (size_t i = 0; i < 1; i++)
	{
		screen_dispatch();
		curTime = timeGetTime();
		float fps = 1 * 1000 / float(curTime - lastTime);
		//if (fps > max_fps) max_fps = fps;
		std::string fpsStr = std::to_string(fps);
		lastTime = curTime;
		::SetConsoleTitleA(fpsStr.c_str());
		//std::cout << fpsStr << '\r';
		renderer->Clear();
		
		// Input.
		for (auto it = content->input_name_map.begin(); it != content->input_name_map.end(); ++it)
		{
			auto key = (*it).first;
			auto key_id = BFContent::MapKey(key);
			if (key_id !=-1 && BFContent::screen_keys[key_id] == 1)
			{
				//std::cout << key_id << std::endl;
				auto input = (*it).second;
				auto op = content->input_op_map.at(input);
				op();
			}
		}
		//cubeMesh->RotateWithY(1.f);
		//renderer->RenderMesh(cubeMesh);

		cuteMesh->RotateWithY(1.f);
		renderer->RenderMesh(cuteMesh);

		renderer->Display();
		
	}

	system("pause");
}

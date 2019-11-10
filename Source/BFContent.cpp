#include "BFContent.h"

int BFContent::screen_keys[512] = { 0 };
int BFContent::screen_exit = 0;
Light* BFContent::m_pLights[BFContent::MAXLIGHTNUM] = {nullptr};

void BFContent::StartUp(BFDevice_t content_device, UINT screenWidth, UINT screenHeight)
{
	m_pContentResourceManager = new BFResourceManager();
	SelectDevice(content_device);
	m_pGod = new ABFActor();// Always exist a godview actor.
	ControllActor(*m_pGod);
	m_pContentDeviceRenderer->Init(screenWidth, screenHeight, m_pControlledActor->GetCamera());
}

void BFContent::SelectDevice(BFDevice_t & device)
{
	mContent_device = device;

	switch (device)
	{
	case (BFDevice_t::WIN_32):
		m_pContentDeviceRenderer = new IBFDeviceRendererWin32();

			break;
	default:
		break;
	}
}

void BFContent::ControllActor(ABFActor & actor)
{
	m_pControlledActor = &actor;
}

// Windows inputMap, from VK to BFInput.
int BFContent::MapKey(BFInput input)
{
	int ret = -1;
	if (input >= BFInput::KEY_A && input <= BFInput::KEY_Z)
	{
		//  Key A to Z is ascii code of 'A' to 'Z'.
		ret = int(input) - int(BFInput::KEY_A) + 65;
	}
	else
	{
		switch (input)
		{
		case(BFInput::KEY_SPACE):
			ret = VK_SPACE;
			break;
		case(BFInput::KEY_SHIFT):
			ret = VK_SHIFT;
			break;
		case(BFInput::KEY_CTRL):
			ret = VK_CONTROL;
			break;
		default:
			break;
		}
	}

	return ret;
}

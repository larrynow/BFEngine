#include "BFContent.h"

int BFContent::screen_keys[512] = { 0 };
int BFContent::screen_exit = 0;

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



#pragma once
#ifndef BFCONTENT
#define BFCONTENT

#include"BFTypes.h"
#include"BFActor.h"
#include"BFDeviceRenderer.h"
#include "BFDeviceRendererWin32.h"

#include<unordered_map>
#include<string>
#include<functional>



class BFContent
{
public:
	BFContent() : m_pContentDeviceRenderer(nullptr), m_pContentResourceManager(nullptr) { input_name_map.clear(); input_op_map.clear(); };

	void StartUp(BFDevice_t content_device, UINT screenWidth, UINT screenHeight);

	void SelectDevice(BFDevice_t& device);

	void ControllActor(ABFActor& actor);

	// Input setting before start up.
	inline void RegisterInput(BFInput input, std::string input_name) { input_name_map.insert(std::make_pair(input, input_name)); };
	inline void BindInput(std::string input_name, std::function<void()> operation) { input_op_map.insert(std::make_pair(input_name, operation)); };

	~BFContent() { delete m_pContentDeviceRenderer; delete m_pContentResourceManager; delete m_pGod; };
//private:

	BFDevice_t mContent_device;
	IBFDeviceRenderer* m_pContentDeviceRenderer;

	ABFActor* m_pGod;// God view actor.
	ABFActor* m_pControlledActor;// An actor that on controll.

	////////////////////////////////////////////////////////////
	// Sub managers.

	BFResourceManager* m_pContentResourceManager;

	///////////////////////////////////////////////////////////
	// Input.

	std::unordered_map<BFInput, std::string> input_name_map;// From a user input to a input name.
	std::unordered_map<std::string, std::function<void()>> input_op_map;// From a input to a operation.

	static int screen_keys[512];// Static input keys.
	static int screen_exit;

	///////////////////////////////////////////////////////////
	// Lights.

	static const UINT MAXLIGHTNUM = 8;
	static Light* m_pLights[MAXLIGHTNUM];// Or vector?

};

#endif // !BFCONTENT




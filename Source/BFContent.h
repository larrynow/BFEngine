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
	BFContent() : mContent_device_render(nullptr){ input_name_map.clear(); input_op_map.clear(); };

	void SelectDevice(BFDevice_t& device);

	inline void RegisterInput(const BFInput& input, const std::string& input_name) { input_name_map.insert(std::make_pair(input, input_name)); };
	inline void BindInput(const std::string& input_name, std::function<void()> operation) { input_op_map.insert(std::make_pair(input_name, operation)); };

	~BFContent() { delete mContent_device_render; };
private:

	BFDevice_t mContent_device;

	IBFDeviceRenderer* mContent_device_render;

	///////////////////////////////////////////////////////////
	// Input.
	std::unordered_map<BFInput, std::string> input_name_map;// From a user input to a input name.
	std::unordered_map<std::string, std::function<void()>> input_op_map;// From a input to a operation.
};

#endif // !BFCONTENT




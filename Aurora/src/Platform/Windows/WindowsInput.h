#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Core/Input.h"
namespace Aurora
{
	class WindowsInput :public Input
	{
	protected:
		bool IsKeyPressedImpl(int keyCode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		float GetMouseXImpl()override;
		float GetMouseYImpl()override;
		std::pair<float, float> GetMousePosImpl()override;
	};
}
#pragma once
#include "Core.h"
namespace Aurora
{
	class AURORA_API Input
	{
	public:
		static bool IsKeyPressed(int keyCode) 
		{
			return s_pInstance->IsKeyPressedImpl(keyCode);
		}
		static bool IsMouseButtonPressed(int button)
		{
			return s_pInstance->IsMouseButtonPressedImpl(button);
		}

		static float GetMouseX() 
		{
			return s_pInstance->GetMouseXImpl();
		}

		static float GetMouseY()
		{
			return s_pInstance->GetMouseYImpl();
		}

		static std::pair<float, float> GetMousePos()
		{
			return s_pInstance->GetMousePosImpl();
		}
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
	private:
		static Input* s_pInstance;
	};
}
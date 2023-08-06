#pragma once

#include "Aurora/Renderer/Shader.h"

namespace Aurora
{
	class AURORA_API OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const std::string& vs, const std::string& fs);
		~OpenGLShader();
	public:
		void Bind()const override;
		void UnBind()const override;
	private:
		static unsigned int CreateShader(int type, const std::string& src);
	private:
		unsigned int m_rendererID;
	};
}
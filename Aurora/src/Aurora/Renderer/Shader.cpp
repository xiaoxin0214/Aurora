#include "pch.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace Aurora
{
	Shader* Shader::Create(const std::string& vs, const std::string& fs)
	{
		return new OpenGLShader(vs,fs);
	}
}
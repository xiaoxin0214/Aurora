#include "pch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
namespace Aurora
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vs, const std::string& fs)
	{
		return std::make_shared<OpenGLShader>(name, vs, fs);
	}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		RendererAPI::API currentApi = RendererAPI::GetCurrentAPI();
		if (RendererAPI::API::None == currentApi)
		{
			AURORA_CORE_ASSERT(false, "�ݲ�֧��RendererAPI::None!");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentApi)
		{
			return std::make_shared<OpenGLShader>(path);
		}


		AURORA_CORE_ASSERT(false, "�����RendererAPI");
		return NULL;
	}

	bool ShaderLibrary::Exist(const std::string& name)const
	{
		return m_shaders.find(name) != m_shaders.end();
	}

	void ShaderLibrary::AddShader(const std::string& name, const Ref<Shader>& shader)
	{
		AURORA_CORE_ASSERT(!Exist(name), "{0}��Ӧ��shader�Ѿ�����!", name.c_str());
		m_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& path)
	{
		auto shader = Shader::Create(path);
		AddShader(shader->GetName(), shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::GetShader(const std::string& name)
	{
		AURORA_CORE_ASSERT(Exist(name), "{0}��Ӧ��shader������!", name.c_str());
		return m_shaders[name];
	}
}
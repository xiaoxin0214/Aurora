#pragma once

#include "Aurora/Renderer/Shader.h"
#include "glad/glad.h"

namespace Aurora
{
	class AURORA_API OpenGLShader :public Shader
	{
	public:
		OpenGLShader(const std::string&name,const std::string& vs, const std::string& fs);
		OpenGLShader(const std::string& path);
		~OpenGLShader();
	public:
		void Bind()const override;
		void UnBind()const override;
		const std::string& GetName()const override {
			return m_name;
		}
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix)override;
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix)override;
		void SetUniformFloat(const std::string& name, float value)override;
		void SetUniformFloat2(const std::string& name, const glm::vec2& value)override;
		void SetUniformFloat3(const std::string& name, const glm::vec3& value)override;
		void SetUniformFloat4(const std::string& name, const glm::vec4& value)override;
		void SetUniformInt(const std::string& name, int value) override;
		void SetUniformIntArray(const std::string& name, int* values, int count)override;
	private:
		static unsigned int CreateShader(int type, const std::string& src);
		void CompileShader(const std::string& vs, const std::string& fs);
		void CompileShader(std::unordered_map<GLenum,std::string>&shaderSources);
	private:
		std::uint32_t m_rendererID;
		std::string  m_name;
	};
}
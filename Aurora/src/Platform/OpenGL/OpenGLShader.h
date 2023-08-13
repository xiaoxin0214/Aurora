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
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix)override;
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix)override;
		void SetUniformFloat(const std::string& name, float value)override;
		void SetUniformFloat2(const std::string& name, const glm::vec2& value)override;
		void SetUniformFloat3(const std::string& name, const glm::vec3& value)override;
		void SetUniformFloat4(const std::string& name, const glm::vec4& value)override;
		void SetUniformInt(const std::string& name, int value) override;
	private:
		static unsigned int CreateShader(int type, const std::string& src);
	private:
		unsigned int m_rendererID;
	};
}
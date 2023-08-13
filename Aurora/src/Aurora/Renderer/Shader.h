#pragma once

#include "Aurora/Core.h"
#include <string>
#include "glm.hpp"
namespace Aurora
{
	class AURORA_API Shader
	{
	public:
		Shader() {};
		virtual ~Shader() {};
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual const std::string& GetName()const = 0;

		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetUniformFloat(const std::string& name, float value) = 0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetUniformInt(const std::string& name, int value) = 0;
		static Ref<Shader> Create(const std::string& name, const std::string& vs, const std::string& fs);
		static Ref<Shader> Create(const std::string& path);
	private:

	};

	class AURORA_API ShaderLibrary
	{
	public:
		void AddShader(const std::string& name, const Ref<Shader>& sahder);
		Ref<Shader> LoadShader(const std::string& path);

		Ref<Shader> GetShader(const std::string& name);

		bool Exist(const std::string& name)const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}
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

		virtual void SetUniformMat4(const std::string&name,const glm::mat4&matrix)=0;
		virtual void SetUniformMat3(const std::string&name,const glm::mat3&matrix)=0;
		virtual void SetUniformFloat(const std::string& name, float value)=0;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetUniformInt(const std::string& name, int value) = 0;
		static Shader* Create(const std::string& vs, const std::string& fs);
	private:

	};
}
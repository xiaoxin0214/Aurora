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
		static Shader* Create(const std::string& vs, const std::string& fs);
	private:

	};
}
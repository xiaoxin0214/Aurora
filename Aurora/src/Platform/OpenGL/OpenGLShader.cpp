#include "pch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "Aurora/Log.h"
#include "gtc/type_ptr.hpp"
namespace Aurora
{
	unsigned int OpenGLShader::CreateShader(int type, const std::string& src)
	{
		unsigned int shader = glCreateShader(type);
		const GLchar* source = src.c_str();
		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint infoLen;
			// infoLen°üº¬½áÊø·û
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			std::vector<GLchar> log(infoLen);

			glGetShaderInfoLog(shader, infoLen, &infoLen, &log[0]);
			glDeleteShader(shader);
			AURORA_CORE_INFO("shader±àÒëÊ§°Ü£º{0}", log.data());
			return 0;
		}


		return shader;
	}

	OpenGLShader::OpenGLShader(const std::string& vs, const std::string& fs)
	{
		unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
		unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs);

		unsigned int program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		m_rendererID = program;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}
	void OpenGLShader::Bind()const
	{
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::UnBind()const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location,value.x,value.y);
	}
	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location,value.x,value.y,value.z);
	}
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location,value);
	}
}
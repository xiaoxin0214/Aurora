#include "pch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "Aurora/Log.h"
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
}
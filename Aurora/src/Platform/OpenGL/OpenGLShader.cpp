#include "pch.h"
#include "OpenGLShader.h"
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
			// infoLen包含结束符
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			std::vector<GLchar> log(infoLen);

			glGetShaderInfoLog(shader, infoLen, &infoLen, &log[0]);
			glDeleteShader(shader);
			AURORA_CORE_INFO("shader编译失败：{0}", log.data());
			return 0;
		}


		return shader;
	}

	static GLenum ShaderTypeFromString(std::string& shaderType)
	{
		if (shaderType == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (shaderType == "fragment")
		{
			return GL_FRAGMENT_SHADER;
		}
		else
		{
			AURORA_CORE_ASSERT(false, "暂不支持的shaderType:{0}", shaderType);
			return 0;
		}
	}

	static bool ReadFile(const std::string& path, std::string& content)
	{
		std::ifstream inFile(path,std::ios::in|std::ios::binary);
		if (inFile)
		{
			inFile.seekg(0,std::ios::end);
			content.resize(inFile.tellg());
			inFile.seekg(0,std::ios::beg);
			inFile.read(&content[0],content.size());
			inFile.close();
			return true;
		}
		else
		{
			AURORA_CORE_WARN("文件{0}打开失败",path.c_str());
			return false;
		}
	}

	static void ParseShaderSource(std::string& sourceCode, std::unordered_map<GLenum, std::string>& shaderSources)
	{
		const char* shaderTypeToken = "#shadertype";
		std::size_t shaderTypeTokenLen = strlen(shaderTypeToken);
		std::size_t pos = sourceCode.find(shaderTypeToken);
		while (pos != std::string::npos)
		{
			std::size_t eol = sourceCode.find_first_of("\r\n",pos);
			AURORA_CORE_ASSERT(eol != std::string::npos, "语义错误!shadertype关键字后面必须指定着色器类型!");
			std::size_t begin = pos + shaderTypeTokenLen + 1;
			std::string shderType = sourceCode.substr(begin,eol-begin);
			AURORA_CORE_ASSERT(ShaderTypeFromString(shderType), "暂不支持的着色器类型!");
			std::size_t nextLinePos = sourceCode.find_first_not_of("\r\n",eol);
			pos = sourceCode.find(shaderTypeToken,nextLinePos);
			shaderSources[ShaderTypeFromString(shderType)] = sourceCode.substr(nextLinePos,pos-nextLinePos);
		}
	}

	void OpenGLShader::CompileShader(const std::string& vs, const std::string& fs)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources.emplace(GL_VERTEX_SHADER,vs);
		shaderSources.emplace(GL_FRAGMENT_SHADER, fs);
		CompileShader(shaderSources);
	}

	void OpenGLShader::CompileShader(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		std::vector<unsigned int> shaderIDLst;
		unsigned int program = glCreateProgram();
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			std::string& source = kv.second;
			unsigned int shader = CreateShader(type, source);
			glAttachShader(program, shader);
			shaderIDLst.emplace_back(shader);
		}


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
			for (auto shaderID : shaderIDLst)
			{
				glDeleteShader(shaderID);
			}

			return;
		}

		for (auto shaderID : shaderIDLst)
		{
			glDetachShader(program,shaderID);
		}

		m_rendererID = program;
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string content;
		ReadFile(path, content);
		std::unordered_map<GLenum, std::string> shaderSources;
		ParseShaderSource(content, shaderSources);
		CompileShader(shaderSources);

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vs, const std::string& fs)
	{
		CompileShader(vs,fs);
		m_name = name;
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
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}
}
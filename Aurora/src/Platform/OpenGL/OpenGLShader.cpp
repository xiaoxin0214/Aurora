#include "pch.h"
#include "OpenGLShader.h"
#include "gtc/type_ptr.hpp"

#include <filesystem>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

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
		std::ifstream inFile(path, std::ios::in | std::ios::binary);
		if (inFile)
		{
			inFile.seekg(0, std::ios::end);
			content.resize(inFile.tellg());
			inFile.seekg(0, std::ios::beg);
			inFile.read(&content[0], content.size());
			inFile.close();
			return true;
		}
		else
		{
			AURORA_CORE_WARN("文件{0}打开失败", path.c_str());
			return false;
		}
	}

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}
		AURORA_CORE_ASSERT(false,"");
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
		}
		AURORA_CORE_ASSERT(false,"");
		return "";
	}


	static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
		}
		AURORA_CORE_ASSERT(false,"");
		return "";
	}

	static void ParseShaderSource(std::string& sourceCode, std::unordered_map<GLenum, std::string>& shaderSources)
	{
		const char* shaderTypeToken = "#shadertype";
		std::size_t shaderTypeTokenLen = strlen(shaderTypeToken);
		std::size_t pos = sourceCode.find(shaderTypeToken);
		while (pos != std::string::npos)
		{
			std::size_t eol = sourceCode.find_first_of("\r\n", pos);
			AURORA_CORE_ASSERT(eol != std::string::npos, "语义错误!shadertype关键字后面必须指定着色器类型!");
			std::size_t begin = pos + shaderTypeTokenLen + 1;
			std::string shderType = sourceCode.substr(begin, eol - begin);
			AURORA_CORE_ASSERT(ShaderTypeFromString(shderType), "暂不支持的着色器类型!");
			std::size_t nextLinePos = sourceCode.find_first_not_of("\r\n", eol);
			pos = sourceCode.find(shaderTypeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(shderType)] = sourceCode.substr(nextLinePos, pos - nextLinePos);
		}
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = GetCacheDirectory();

		auto& shaderData = m_vulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), m_filePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					AURORA_CORE_ERROR(module.GetErrorMessage());
					AURORA_CORE_ASSERT(false,"");
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_openGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = GetCacheDirectory();

		shaderData.clear();
		m_openGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_vulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_openGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_openGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), m_filePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					AURORA_CORE_ERROR(module.GetErrorMessage());
					AURORA_CORE_ERROR(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateCacheDirectoryIfNeeded()
	{
		std::string cacheDirectory = GetCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
			std::filesystem::create_directories(cacheDirectory);
	}


	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_openGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			AURORA_CORE_ERROR("Shader linking failed ({0}):\n{1}",m_filePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_rendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();
		}
	}

	OpenGLShader::OpenGLShader(const std::string& path):m_filePath(path)
	{
		CreateCacheDirectoryIfNeeded();

		std::string content;
		ReadFile(path, content);
		std::unordered_map<GLenum, std::string> shaderSources;
		ParseShaderSource(content, shaderSources);

		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash;
		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vs, const std::string& fs)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources.emplace(GL_VERTEX_SHADER, vs);
		shaderSources.emplace(GL_FRAGMENT_SHADER, fs);

		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

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
	void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, int count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}
}
#include "pch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"
namespace Aurora
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
	{
		glViewport(x,y,width,height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray,std::uint32_t count)
	{
		std::uint32_t indicesCount = 0 == count ? vertexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);
	}
}
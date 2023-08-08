#include "pch.h"
#include "Renderer.h"
#include "RendererCommand.h"
namespace Aurora
{
	void Renderer::BeginScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {

	}
}
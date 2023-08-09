#include "pch.h"
#include "Renderer.h"
#include "RendererCommand.h"
namespace Aurora
{
	Renderer::SceneData* Renderer::s_pSceneData = new SceneData();

	void Renderer::BeginScene(const OrthographicCamera&camera)
	{
		s_pSceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray,const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniformMat4("u_viewProjection",s_pSceneData->viewProjectionMatrix);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {

	}
}
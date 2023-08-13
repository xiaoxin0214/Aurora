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

	void Renderer::Submit(const Ref<VertexArray>& vertexArray,const Ref<Shader>& shader,const glm::mat4&modelMatrix)
	{
		shader->Bind();
		shader->SetUniformMat4("u_viewProjection",s_pSceneData->viewProjectionMatrix);
		shader->SetUniformMat4("u_modelMatrix",modelMatrix);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {

	}
}
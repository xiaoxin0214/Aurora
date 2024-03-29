#include "pch.h"
#include "Renderer.h"
#include "RendererCommand.h"
#include "Renderer2D.h"
namespace Aurora
{
	Renderer::SceneData* Renderer::s_pSceneData = new SceneData();

	void Renderer::Init()
	{
		Renderer2D::Init();
		RendererCommand::Init();
	}

	void Renderer::OnWindowResize(std::uint32_t width, std::uint32_t height)
	{
		RendererCommand::SetViewport(0,0,width,height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_pSceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelMatrix)
	{
		shader->Bind();
		shader->SetUniformMat4("u_viewProjection", s_pSceneData->viewProjectionMatrix);
		shader->SetUniformMat4("u_modelMatrix", modelMatrix);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {

	}
}
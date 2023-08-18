#include "pch.h"
#include "Renderer2D.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"
#include "gtc/matrix_transform.hpp"
namespace Aurora
{

	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Texture>     whiteTexture;
		Ref<Shader>      shader;
	};

	static Renderer2DStorage* s_pData;

	void Renderer2D::Init()
	{
		s_pData = new Renderer2DStorage();
		float vertices[4 * 5] = {
			-1.0f,-1.0f,0.0f,0.0f,0.0f,
			1.0f,-1.0f,0.0f,1.0f,0.0f,
			1.0f,1.0f,0.0f,1.0f,1.0f,
			-1.0f,1.0f,0.0f,0.0f,1.0f
		};

		BufferLayout layout({
			{ShaderDataType::Float3,"a_pos"},
			{ShaderDataType::Float2,"a_texcoord"}
			});

		unsigned int indices[6] = {
			0,1,2,2,3,0
		};

		Ref<IndexBuffer> indexBuffer;
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
		vertexBuffer->SetLayout(layout);

		s_pData->vertexArray = VertexArray::Create();
		s_pData->vertexArray->AddVertexBuffer(vertexBuffer);
		s_pData->vertexArray->SetIndexBuffer(indexBuffer);

		s_pData->shader = Shader::Create("asset\\shaders\\test.glsl");
		s_pData->whiteTexture = Texture2D::Create("asset\\textures\\white.png");
	}

	void Renderer2D::Shutdown()
	{
		delete s_pData;
		s_pData = NULL;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_pData->shader->Bind();
		s_pData->shader->SetUniformMat4("u_viewProjection", camera.GetViewProjectionMatrix());

	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_pData->shader->Bind();
		s_pData->vertexArray->Bind();
		s_pData->shader->SetUniformFloat4("u_color", color);
		s_pData->whiteTexture->Bind(0);
		s_pData->shader->SetUniformInt("u_texture", 0);
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f))
			* glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0.0, 0.0, 1.0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_pData->shader->SetUniformMat4("u_modelMatrix", modelMatrix);
		RendererCommand::DrawIndexed(s_pData->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		s_pData->shader->Bind();
		s_pData->vertexArray->Bind();
		s_pData->shader->SetUniformFloat4("u_color", tintColor);
		texture->Bind(0);
		s_pData->shader->SetUniformInt("u_texture", 0);
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f))
			* glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_pData->shader->SetUniformMat4("u_modelMatrix", modelMatrix);
		RendererCommand::DrawIndexed(s_pData->vertexArray);
	}
}
#include "pch.h"
#include "Renderer2D.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"
#include "gtc/matrix_transform.hpp"
namespace Aurora
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float     textureIndex;
	};

	struct Renderer2DStorage
	{
		static const std::uint32_t maxQuadNum = 10000;
		static const std::uint32_t maxVertexNum = maxQuadNum * 4;
		static const std::uint32_t maxIndexNum = maxQuadNum * 6;
		static const std::uint32_t maxTextureSlotNum = 32;

		Ref<VertexBuffer> vertexBuffer;
		Ref<VertexArray> vertexArray;
		Ref<Texture>     whiteTexture;
		Ref<Shader>      shader;
		Vertex* pVertexBase;
		Vertex* pVertex;
		std::array<Ref<Texture>,maxTextureSlotNum> textureSlots;
		std::uint32_t    indicesCount = 0;
		std::uint32_t    textureSlotIndex = 1;

		Renderer2DStorage() :pVertexBase(NULL), pVertex(NULL)
		{

		}
	};

	static Renderer2DStorage* s_pData;


	void Renderer2D::Init()
	{
		s_pData = new Renderer2DStorage();

		BufferLayout layout({
			{ShaderDataType::Float3,"a_pos"},
			{ShaderDataType::Float4,"a_color"},
			{ShaderDataType::Float2,"a_texcoord"},
			{ShaderDataType::Float,"a_texIndex"}
			});

		s_pData->vertexBuffer.reset(VertexBuffer::Create(s_pData->maxVertexNum));

		s_pData->pVertexBase = s_pData->pVertex = new Vertex[s_pData->maxVertexNum];

		std::uint32_t offset=0;
		unsigned int *pIndices = new unsigned int[s_pData->maxIndexNum];
		for (std::int32_t i = 0; i < s_pData->maxIndexNum; i += 6)
		{
			pIndices[i + 0] = offset + 0;
			pIndices[i + 1] = offset + 1;
			pIndices[i + 2] = offset + 2;

			pIndices[i + 3] = offset + 2;
			pIndices[i + 4] = offset + 3;
			pIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(pIndices, s_pData->maxIndexNum));
		delete[]pIndices;

		s_pData->vertexBuffer->SetLayout(layout);

		s_pData->vertexArray = VertexArray::Create();
		s_pData->vertexArray->AddVertexBuffer(s_pData->vertexBuffer);
		s_pData->vertexArray->SetIndexBuffer(indexBuffer);

		s_pData->shader = Shader::Create("asset\\shaders\\test.glsl");
		s_pData->whiteTexture = Texture2D::Create("asset\\textures\\white.png");

		s_pData->textureSlots[0] = s_pData->whiteTexture;
		int samplers[s_pData->maxTextureSlotNum];
		for (std::uint32_t i = 0; i < s_pData->maxTextureSlotNum; ++i)
			samplers[i] = i;

		s_pData->shader->Bind();
		s_pData->shader->SetUniformIntArray("u_textures", samplers, s_pData->maxTextureSlotNum);
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
		s_pData->pVertex = s_pData->pVertexBase;
		s_pData->indicesCount = 0;
		s_pData->textureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		s_pData->vertexBuffer->SetData(s_pData->pVertexBase,(std::uint8_t*)s_pData->pVertex- (std::uint8_t*)s_pData->pVertexBase);
		Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		float whiteTextureIndex = 0.0f;
		s_pData->pVertex->position = glm::vec3(pos.x,pos.y,0.0f);
		s_pData->pVertex->color = color;
		s_pData->pVertex->texCoord = glm::vec2(0.0f,0.0f);
		s_pData->pVertex->textureIndex = whiteTextureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x+size.x, pos.y, 0.0f);
		s_pData->pVertex->color = color;
		s_pData->pVertex->texCoord = glm::vec2(1.0f, 0.0f);
		s_pData->pVertex->textureIndex = whiteTextureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x+size.x, pos.y+size.y, 0.0f);
		s_pData->pVertex->color = color;
		s_pData->pVertex->texCoord = glm::vec2(1.0f, 1.0f);
		s_pData->pVertex->textureIndex = whiteTextureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x, pos.y+size.y, 0.0f);
		s_pData->pVertex->color = color;
		s_pData->pVertex->texCoord = glm::vec2(0.0f, 1.0f);
		s_pData->pVertex->textureIndex = whiteTextureIndex;
		s_pData->pVertex++;

		s_pData->indicesCount += 6;
		//s_pData->shader->SetUniformInt("u_texture", 0);
		//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f))
		//	* glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0.0, 0.0, 1.0))
		//	* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		//s_pData->shader->SetUniformMat4("u_modelMatrix", modelMatrix);

		//RendererCommand::DrawIndexed(s_pData->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		float textureIndex = 0.0f;
		for (std::uint32_t i = 1; i < s_pData->textureSlotIndex; ++i)
		{
			if (*texture == *s_pData->textureSlots[i])
			{
				textureIndex = i;
			}
		}

		if (0.0f == textureIndex)
		{
			s_pData->textureSlots[s_pData->textureSlotIndex] = texture;
			textureIndex = s_pData->textureSlotIndex;
			s_pData->textureSlotIndex++;
		}

		s_pData->pVertex->position = glm::vec3(pos.x, pos.y, 0.0f);
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = glm::vec2(0.0f, 0.0f);
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x + size.x, pos.y, 0.0f);
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = glm::vec2(1.0f, 0.0f);
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x + size.x, pos.y + size.y, 0.0f);
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = glm::vec2(1.0f, 1.0f);
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(pos.x, pos.y + size.y, 0.0f);
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = glm::vec2(0.0f, 1.0f);
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex++;

		s_pData->indicesCount += 6;

		//s_pData->shader->Bind();
		//s_pData->vertexArray->Bind();
		//s_pData->shader->SetUniformFloat4("u_color", tintColor);
		//texture->Bind(0);
		//s_pData->shader->SetUniformInt("u_texture", 0);
		//glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f))
		//	* glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
		//	* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		//s_pData->shader->SetUniformMat4("u_modelMatrix", modelMatrix);
		//RendererCommand::DrawIndexed(s_pData->vertexArray);
	}

	void Renderer2D::Flush()
	{
		for (std::uint32_t i = 0; i < s_pData->textureSlotIndex; ++i)
		{
			s_pData->textureSlots[i]->Bind(i);
		}
		s_pData->shader->Bind();
		s_pData->vertexArray->Bind();
		s_pData->shader->SetUniformMat4("u_modelMatrix", glm::mat4(1.0f));
		RendererCommand::DrawIndexed(s_pData->vertexArray, s_pData->indicesCount);
	}
}
#include "pch.h"
#include "Renderer2D.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RendererCommand.h"
#include "UniformBuffer.h"
#include "gtc/matrix_transform.hpp"
namespace Aurora
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float     textureIndex;
		int       entityID;
	};

	struct Renderer2DStorage
	{
		static const std::uint32_t maxQuadNum = 10;
		static const std::uint32_t maxVertexNum = maxQuadNum * 4;
		static const std::uint32_t maxIndexNum = maxQuadNum * 6;
		static const std::uint32_t maxTextureSlotNum = 32;

		Ref<VertexBuffer> vertexBuffer;
		Ref<VertexArray> vertexArray;
		Ref<Texture>     whiteTexture;
		Ref<Shader>      shader;
		Vertex* pVertexBase;
		Vertex* pVertex;
		std::array<Ref<Texture>, maxTextureSlotNum> textureSlots;
		std::uint32_t    indicesCount = 0;
		std::uint32_t    textureSlotIndex = 1;
		Renderer2D::Statistics       stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};
		CameraData cameraData;
		Ref<UniformBuffer> cameraUniformBuffer;

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
			{ShaderDataType::Float,"a_texIndex"},
			{ShaderDataType::Int,"a_entityID"}
			});

		s_pData->vertexBuffer.reset(VertexBuffer::Create(s_pData->maxVertexNum * sizeof(Vertex)));

		s_pData->pVertexBase = s_pData->pVertex = new Vertex[s_pData->maxVertexNum];

		std::uint32_t offset = 0;
		unsigned int* pIndices = new unsigned int[s_pData->maxIndexNum];
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
		s_pData->cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_pData;
		s_pData = NULL;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_pData->cameraData.viewProjection = camera.GetProjection() * glm::inverse(transform);
		s_pData->cameraUniformBuffer->SetData(&s_pData->cameraData, sizeof(Renderer2DStorage::CameraData));
		s_pData->pVertex = s_pData->pVertexBase;
		s_pData->indicesCount = 0;
		s_pData->textureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& editorCamera)
	{
		s_pData->cameraData.viewProjection = editorCamera.GetViewProjection();
		s_pData->cameraUniformBuffer->SetData(&s_pData->cameraData, sizeof(Renderer2DStorage::CameraData));
		s_pData->pVertex = s_pData->pVertexBase;
		s_pData->indicesCount = 0;
		s_pData->textureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_pData->cameraData.viewProjection = camera.GetViewProjectionMatrix();
		s_pData->cameraUniformBuffer->SetData(&s_pData->cameraData, sizeof(Renderer2DStorage::CameraData));
		s_pData->pVertex = s_pData->pVertexBase;
		s_pData->indicesCount = 0;
		s_pData->textureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		s_pData->vertexBuffer->SetData(s_pData->pVertexBase, (std::uint8_t*)s_pData->pVertex - (std::uint8_t*)s_pData->pVertexBase);
		Flush();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor,int entityID)
	{
		if (s_pData->indicesCount >= Renderer2DStorage::maxIndexNum)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (std::uint32_t i = 1; i < s_pData->textureSlotIndex; ++i)
		{
			if (*(subTexture->GetTexture2D()) == *s_pData->textureSlots[i])
			{
				textureIndex = i;
			}
		}

		if (0.0f == textureIndex)
		{
			s_pData->textureSlots[s_pData->textureSlotIndex] = subTexture->GetTexture2D();
			textureIndex = s_pData->textureSlotIndex;
			s_pData->textureSlotIndex++;
		}

		//s_pData->shader->SetUniformMat4("u_modelMatrix", transform);

		const glm::vec2* texCoords = subTexture->GetTextureCoords();
		s_pData->pVertex->position = glm::vec3(transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = texCoords[0];
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex->entityID = entityID;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f));
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = texCoords[1];
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex->entityID = entityID;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = texCoords[2];
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex->entityID = entityID;
		s_pData->pVertex++;

		s_pData->pVertex->position = glm::vec3(transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f));
		s_pData->pVertex->color = tintColor;
		s_pData->pVertex->texCoord = texCoords[3];
		s_pData->pVertex->textureIndex = textureIndex;
		s_pData->pVertex->entityID = entityID;
		s_pData->pVertex++;

		s_pData->indicesCount += 6;
		s_pData->stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawQuad(pos, size, rotation, s_pData->whiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		auto subTexture2D = SubTexture2D::Create(s_pData->whiteTexture, glm::vec2(0.0f), glm::vec2(1.0f));
		DrawQuad(transform, subTexture2D, color, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID)
	{
		auto subTexture2D = SubTexture2D::Create(texture, glm::vec2(0.0f), glm::vec2(1.0f));
		DrawQuad(transform, subTexture2D, tintColor, entityID);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture>& texture, const glm::vec4& tintColor)
	{
		auto subTexture2D = SubTexture2D::Create(texture, glm::vec2(0.0f), glm::vec2(1.0f));
		DrawQuad(pos, size, rotation, subTexture2D, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z))
			* glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		DrawQuad(modelMatrix, subTexture, tintColor);
	}

	void Renderer2D::Flush()
	{
		if (s_pData->indicesCount < 1)
			return;

		for (std::uint32_t i = 0; i < s_pData->textureSlotIndex; ++i)
		{
			s_pData->textureSlots[i]->Bind(i);
		}
		s_pData->shader->Bind();
		s_pData->vertexArray->Bind();
		//s_pData->shader->SetUniformMat4("u_modelMatrix", glm::mat4(1.0f));
		RendererCommand::DrawIndexed(s_pData->vertexArray, s_pData->indicesCount);
		s_pData->stats.drawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		s_pData->pVertex = s_pData->pVertexBase;
		s_pData->indicesCount = 0;
		s_pData->textureSlotIndex = 1;
	}

	void Renderer2D::ResetStatistics()
	{
		s_pData->stats.drawCalls = 0;
		s_pData->stats.quadCount = 0;
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_pData->stats;
	}
}
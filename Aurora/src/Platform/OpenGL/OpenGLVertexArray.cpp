#include "pch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"
namespace Aurora
{
	static GLenum ShaderDataType2OpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:return GL_FLOAT;
		case ShaderDataType::Float2:return GL_FLOAT;
		case ShaderDataType::Float3:return	GL_FLOAT;
		case ShaderDataType::Float4:return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2:return GL_INT;
		case ShaderDataType::Int3:return GL_INT;
		case ShaderDataType::Int4:return GL_INT;
		case ShaderDataType::Mat3:return GL_FLOAT;
		case ShaderDataType::Mat4:return GL_FLOAT;
		case ShaderDataType::Bool:return GL_BOOL;
		}

		AURORA_CORE_ASSERT(false, "�ݲ�֧�ֵ�����");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_vertexArrayID);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1,&m_vertexArrayID);
	}

	void OpenGLVertexArray::Bind()const
	{
		glBindVertexArray(m_vertexArrayID);
	}

	void OpenGLVertexArray::UnBind()const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer>& vb)
	{
		AURORA_CORE_ASSERT(vb->GetLayout().GetElements().size(),"VertexBuffer δ����Layout!")
		Bind();
		vb->Bind();

		std::uint32_t index = 0;
		const auto& layout = vb->GetLayout();
		for (const auto& elem : layout)
		{
			switch (elem.dataType)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, elem.GetComponentCount(), ShaderDataType2OpenGLType(elem.dataType), elem.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)elem.offset);
				break;
			}
			case ShaderDataType::Int: 
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, elem.GetComponentCount(), ShaderDataType2OpenGLType(elem.dataType), layout.GetStride(), (const void*)elem.offset);
				break;
			}

			}

			index++;
		}
		m_vertexBufferLst.push_back(vb);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& ib)
	{
		Bind();
		ib->Bind();
		m_indexBuffer = ib;
	}
}
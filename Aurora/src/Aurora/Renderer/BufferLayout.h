#pragma once
#include "Aurora/Core.h"
#include "Aurora/Log.h"
#include <vector>
#include <string>
namespace Aurora
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
		Mat3,
		Mat4

	};

	static std::uint32_t CalcDataTypeSize(ShaderDataType dataType)
	{
		switch (dataType)
		{
		case ShaderDataType::Float:      return sizeof(float);
		case ShaderDataType::Float2:     return sizeof(float) * 2;
		case ShaderDataType::Float3:     return sizeof(float) * 3;
		case ShaderDataType::Float4:     return sizeof(float) * 4;
		case ShaderDataType::Int:        return sizeof(int);
		case ShaderDataType::Int2:       return sizeof(int) * 2;
		case ShaderDataType::Int3:       return sizeof(int) * 3;
		case ShaderDataType::Int4:       return sizeof(int) * 4;
		case ShaderDataType::Bool:       return sizeof(bool);
		case ShaderDataType::Mat3:       return sizeof(float) * 3 * 3;
		case ShaderDataType::Mat4:       return sizeof(float) * 4 * 4;
		}

		AURORA_CORE_ASSERT(false, "暂不支持的ShaderDataType!");
		return 0;
	}
	struct BufferElement
	{
		std::string       name;
		std::uint32_t     size;
		std::uint32_t     offset;
		ShaderDataType    dataType;
		bool              normalized;
		BufferElement(ShaderDataType dataTypeIn, const std::string& nameIn, bool normalizedIn = false) :name(nameIn), dataType(dataTypeIn), size(CalcDataTypeSize(dataTypeIn)), offset(0), normalized(normalizedIn)
		{

		}

		std::uint32_t GetComponentCount()const
		{
			switch (dataType)
			{
			case ShaderDataType::Float:       return 1;
			case ShaderDataType::Float2:      return 2;
			case ShaderDataType::Float3:      return 3;
			case ShaderDataType::Float4:      return 4;
			case ShaderDataType::Int:         return 1;
			case ShaderDataType::Int2:        return 2;
			case ShaderDataType::Int3:        return 3;
			case ShaderDataType::Int4:        return 4;
			case ShaderDataType::Bool:        return 1;
			case ShaderDataType::Mat3:        return  3 * 3;
			case ShaderDataType::Mat4:        return  4 * 4;

			}
			AURORA_CORE_ASSERT(false, "暂不支持的ShaderDataType!");
			return 0;

		}
	};

	class AURORA_API BufferLayout
	{
	public:
		BufferLayout():m_stride(0)
		{

		}

		BufferLayout(const std::vector<BufferElement>& layout) :m_elements(layout)
		{
			CalcOffsetAndStride();
		}
		BufferLayout(std::initializer_list<BufferElement>& layout) :m_elements(layout)
		{
			CalcOffsetAndStride();
		}
		const std::vector<BufferElement>& GetElements()const
		{
			return m_elements;
		}

		std::vector<BufferElement>::iterator begin()
		{
			return m_elements.begin();
		}

		std::vector<BufferElement>::iterator end()
		{
			return m_elements.end();
		}

		std::vector<BufferElement>::const_iterator begin() const
		{
			return m_elements.begin();
		}

		std::vector<BufferElement>::const_iterator end() const
		{
			return m_elements.end();
		}

		const std::uint32_t GetStride()const
		{
			return  m_stride;
		}
	private:
		void CalcOffsetAndStride()
		{
			std::uint32_t offset = 0;
			m_stride = 0;
			for (auto& elem : m_elements)
			{
				elem.offset = offset;
				offset += elem.size;
				m_stride += elem.size;
			}
		}
	private:
		std::vector<BufferElement> m_elements;
		std::uint32_t              m_stride;
	};
}
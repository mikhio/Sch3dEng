#pragma once

#include "VertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
	unsigned int m_LastStride;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetStride() const { return m_LastStride; };
};

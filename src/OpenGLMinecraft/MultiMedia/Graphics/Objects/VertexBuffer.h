#pragma once

class VertexBuffer 
{
public:
	VertexBuffer(const void* p_Data, size_t p_SizeBytes);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
};

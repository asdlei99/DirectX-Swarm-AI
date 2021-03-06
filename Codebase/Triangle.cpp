#include "Triangle.h"

Triangle::Triangle()
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

Triangle::~Triangle()
{
}

//Creates an equalateral triangle
void Triangle::SetupVertices()
{
	m_vertices = new VertexType[m_vertexCount];

	//bottom left
	m_vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
	m_vertices[0].color = DirectX::XMFLOAT4(m_color);

	//top middle
	m_vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_vertices[1].color = DirectX::XMFLOAT4(m_color);

	//bottom right
	m_vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
	m_vertices[2].color = DirectX::XMFLOAT4(m_color);
	return;
}


void Triangle::SetupIndices()
{
	m_indices = new unsigned long[m_indexCount];

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;
	return;
}

void Triangle::SetupCounts()
{
	m_vertexCount = 3;
	m_indexCount = 3;
	return;
}
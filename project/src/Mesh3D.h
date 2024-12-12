#pragma once
#include "Vector3.h"
#include "pch.h"
#include "ColorRGB.h"
#include <vector>
#include "Effect.h"
using namespace dae;

struct Vertex
{
	Vector3 position;
	ColorRGB color;
};

class Mesh3D final
{
public:
	Mesh3D(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	~Mesh3D();

	Mesh3D(const Mesh3D& other) = delete;
	Mesh3D& operator=(const Mesh3D& rhs) = delete;
	Mesh3D(Mesh3D&& other) = delete;
	Mesh3D& operator=(Mesh3D&& rhs) = delete;

	void Render(const Matrix& pWorldViewProjectionMatrix, ID3D11DeviceContext* pDeviceContext) const;

private:
	uint32_t				m_NumIndices{};
	Effect*					m_pEffect;

	ID3D11Buffer*			m_pVertexBuffer{};
	ID3D11InputLayout*		m_pVertexLayout{};
	ID3D11Buffer*			m_pIndexBuffer{};
};

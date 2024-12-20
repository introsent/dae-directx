#pragma once
#include "Vector3.h"
#include "pch.h"
#include "ColorRGB.h"
#include <vector>
#include "Effect.h"
#include "VehicleEffect.h"
using namespace dae;

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal; 
	Vector3 tangent; 
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

	void Render(const Vector3& cameraPosition, const Matrix& pWorldMatrix, const Matrix& pWorldViewProjectionMatrix, ID3D11DeviceContext* pDeviceContext) const;

	enum FilteringTechnique
	{
		Point,
		Linear,
		Anisotropic
	};

	void SetFilteringTechnique(FilteringTechnique filteringTechnique)
	{
		m_FilteringTechnique = filteringTechnique;
		switch (m_FilteringTechnique)
		{
		case FilteringTechnique::Point:
			m_pVehicleEffect->SetPointSampling();
			break;
		case FilteringTechnique::Linear:
			m_pVehicleEffect->SetLinearSampling();
			break;
		case FilteringTechnique::Anisotropic:
			m_pVehicleEffect->SetAnisotropicSampling();
			break;
		}
	}

	FilteringTechnique GetFilteringTechnique()
	{
		return m_FilteringTechnique;
	}

	
private:
	uint32_t				m_NumIndices{};
	VehicleEffect*			m_pVehicleEffect;
	FilteringTechnique      m_FilteringTechnique{ FilteringTechnique::Point };

	ID3D11Buffer*			m_pVertexBuffer{};
	ID3D11InputLayout*		m_pVertexLayout{};
	ID3D11Buffer*			m_pIndexBuffer{};
};

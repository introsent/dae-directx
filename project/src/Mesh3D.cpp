#include "Mesh3D.h"
#include "Camera.h"
#include "Texture.h"
#include <memory.h>

Mesh3D::Mesh3D(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_pEffect = new Effect{ pDevice, L"resources/PosCol3D.fx" };

	//1. Create Vertex Layout
	static constexpr uint32_t numElements{ 3 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "COLOR";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[2].SemanticName = "TEXCOORD";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[2].AlignedByteOffset = 24;
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;


	//2. Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pEffect->GetTechniquePoint()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT result{ pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pVertexLayout )};
	if (FAILED(result)) return;

	m_pEffect->GetTechniqueLinear()->GetPassByIndex(0)->GetDesc(&passDesc);
	result = pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pVertexLayout);
	if (FAILED(result)) return;

	m_pEffect->GetTechniqueAnisotropic()->GetPassByIndex(0)->GetDesc(&passDesc);
	result = pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pVertexLayout);
	if (FAILED(result)) return;

	//3. Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result)) return;

	//4. Create index buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) return;

	std::unique_ptr<Texture> pTexture = Texture::LoadFromFile(pDevice, "resources/vehicle_diffuse.png");
	m_pEffect->SetDiffuseMap(pTexture.get());
}

Mesh3D::~Mesh3D()
{
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}

	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	if (m_pVertexLayout) 
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}

	delete m_pEffect;

}

void Mesh3D::Render(const Matrix& pWorldViewProjectionMatrix, ID3D11DeviceContext* pDeviceContext) const
{	
	//1. Set primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//2. Set input layout
	pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//3. Set vertex buffer
	constexpr UINT stride{ sizeof(Vertex) };
	constexpr UINT offset{};
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//4. Set index buffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//5. Set World View Projection Matrix
	m_pEffect->GetWorldViewProjectionMatrix()->SetMatrix(reinterpret_cast<const float*>(&pWorldViewProjectionMatrix));

	//6. Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	switch (m_FilteringTechnique)
	{
	case FilteringTechnique::Point:
		m_pEffect->GetTechniquePoint()->GetDesc(&techniqueDesc);
		for (UINT p{}; p < techniqueDesc.Passes; ++p)
		{
			auto passIndexPoint = m_pEffect->GetTechniquePoint()->GetPassByIndex(p);
			passIndexPoint->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
			if (passIndexPoint) passIndexPoint->Release();
		}
		break;
	case FilteringTechnique::Linear:
		m_pEffect->GetTechniqueLinear()->GetDesc(&techniqueDesc);
		for (UINT p{}; p < techniqueDesc.Passes; ++p)
		{
			auto passIndexLinear = m_pEffect->GetTechniqueLinear()->GetPassByIndex(p);
			passIndexLinear->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
			if (passIndexLinear) passIndexLinear->Release();
		}
		break;
	case FilteringTechnique::Anisotropic:
		m_pEffect->GetTechniqueAnisotropic()->GetDesc(&techniqueDesc);
		for (UINT p{}; p < techniqueDesc.Passes; ++p)
		{
			auto passIndexAnisotropic = m_pEffect->GetTechniqueAnisotropic()->GetPassByIndex(p);
			passIndexAnisotropic->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
			if (passIndexAnisotropic) passIndexAnisotropic->Release();
		}
		break;

	}

}

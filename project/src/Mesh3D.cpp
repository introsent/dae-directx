#include "Mesh3D.h"
#include "Camera.h"
#include "Texture.h"
#include <memory.h>

Mesh3D::Mesh3D(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_pVehicleEffect = new VehicleEffect{ pDevice, L"resources/PosCol3D.fx" };

	//1. Create Vertex Layout
	static constexpr uint32_t numElements{ 4 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "TEXCOORD";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[2].SemanticName = "NORMAL";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[2].AlignedByteOffset = 20;
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[3].SemanticName = "TANGENT";
	vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[3].AlignedByteOffset = 32;
	vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//2. Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pVehicleEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT result{ pDevice->CreateInputLayout
		(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pVertexLayout )};
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

	std::unique_ptr<Texture> pDiffuseTexture = Texture::LoadFromFile(pDevice, "resources/vehicle_diffuse.png");
	m_pVehicleEffect->SetDiffuseMap(pDiffuseTexture.get());

	std::unique_ptr<Texture> pNormalTexture = Texture::LoadFromFile(pDevice, "resources/vehicle_normal.png");
	m_pVehicleEffect->SetNormalMap(pNormalTexture.get());

	std::unique_ptr<Texture> pSpecularTexture = Texture::LoadFromFile(pDevice, "resources/vehicle_specular.png");
	m_pVehicleEffect->SetSpecularMap(pSpecularTexture.get());

	std::unique_ptr<Texture> pGlossinessTexture = Texture::LoadFromFile(pDevice, "resources/vehicle_gloss.png");
	m_pVehicleEffect->SetGlossinessMap(pGlossinessTexture.get());

}

Mesh3D::~Mesh3D()
{
	delete m_pVehicleEffect;

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
}

void Mesh3D::Render(const Vector3& cameraPosition, const Matrix& pWorldMatrix, const Matrix& pWorldViewProjectionMatrix, ID3D11DeviceContext* pDeviceContext) const
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
	m_pVehicleEffect->GetCameraPosition()->SetFloatVector(reinterpret_cast<const float*>(&cameraPosition));
	m_pVehicleEffect->GetWorldMatrix()->SetMatrix(reinterpret_cast<const float*>(&pWorldMatrix));
	m_pVehicleEffect->GetWorldViewProjectionMatrix()->SetMatrix(reinterpret_cast<const float*>(&pWorldViewProjectionMatrix));

	//6. Draw
	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	m_pVehicleEffect->GetTechnique()->GetDesc(&techniqueDesc);
	for (UINT p{}; p < techniqueDesc.Passes; ++p)
	{
		auto passIndexPoint = m_pVehicleEffect->GetTechnique()->GetPassByIndex(p);
		passIndexPoint->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		if (passIndexPoint) passIndexPoint->Release();
	}
}

#include "Effect.h"
#include "VehicleEffect.h"

VehicleEffect::VehicleEffect(ID3D11Device* pDevice, const std::wstring& assetFile) : Effect(pDevice, assetFile)
{
	m_EffectSamplerVariable = m_pEffect->GetVariableByName("gSamplerState")->AsSampler();
	if (!m_EffectSamplerVariable->IsValid())
	{
		std::wcout << L"Effect sampler veriable is not valid\n";
		return;
	}

	// Point sampler
	D3D11_SAMPLER_DESC pointDesc = {};
	pointDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	pointDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pointDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	pointDesc.MinLOD = 0;
	pointDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = pDevice->CreateSamplerState(&pointDesc, &m_pSamplerPoint);
	if (FAILED(hr))
	{
		std::wcout << L"Point Sampling state not valid\n";
	}

	// Linear sampler
	D3D11_SAMPLER_DESC linearDesc = {};
	linearDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	linearDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	linearDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	linearDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	linearDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	linearDesc.MinLOD = 0;
	linearDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pDevice->CreateSamplerState(&linearDesc, &m_pSamplerLinear);
	if (FAILED(hr))
	{
		std::wcout << L"Linear Sampling state not valid\n";
	}

	// Anisotropic sampler
	D3D11_SAMPLER_DESC anisotropicDesc = {};
	anisotropicDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	anisotropicDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	anisotropicDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	anisotropicDesc.MinLOD = 0;
	anisotropicDesc.MaxLOD = D3D11_FLOAT32_MAX;
	anisotropicDesc.MaxAnisotropy = 16; // Maximum anisotropy level
	hr = pDevice->CreateSamplerState(&anisotropicDesc, &m_pSamplerAnisotropic);
	if (FAILED(hr))
	{
		std::wcout << L"Anisotropic Sampling state not valid\n";
	}

	if (m_pSamplerPoint != nullptr)
	{
		m_EffectSamplerVariable->SetSampler(0, m_pSamplerPoint);
	}

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid())
	{
		std::wcout << L"m_pDiffuseMapVariable not valid!\n";
	}

	m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	if (!m_pNormalMapVariable->IsValid())
	{
		std::wcout << L"m_pNormalMapVariable not valid!\n";
	}

	m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
	if (!m_pSpecularMapVariable->IsValid())
	{
		std::wcout << L"m_pSpecularMapVariable not valid!\n";
	}

	m_pGlossinessMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
	if (!m_pGlossinessMapVariable->IsValid())
	{
		std::wcout << L"m_pGlossinessMapVariable not valid!\n";
	}

}

VehicleEffect::~VehicleEffect()
{
	//Release maps
	if (m_pGlossinessMapVariable)
	{
		m_pGlossinessMapVariable->Release();
		m_pGlossinessMapVariable = nullptr;
	}

	if (m_pSpecularMapVariable)
	{
		m_pSpecularMapVariable->Release();
		m_pSpecularMapVariable = nullptr;
	}

	if (m_pNormalMapVariable)
	{
		m_pNormalMapVariable->Release();
		m_pNormalMapVariable = nullptr;
	}

	if (m_pDiffuseMapVariable)
	{
		m_pDiffuseMapVariable->Release();
		m_pDiffuseMapVariable = nullptr;
	}

	//Release techniques
	if (m_pTechnique)
	{
		m_pTechnique->Release();
		m_pTechnique = nullptr;
	}

	//Release samplers
	if (m_pSamplerAnisotropic)
	{
		m_pSamplerAnisotropic->Release();
		m_pSamplerAnisotropic = nullptr;
	}

	if (m_pSamplerLinear)
	{
		m_pSamplerLinear->Release();
		m_pSamplerLinear = nullptr;
	}

	if (m_pSamplerPoint)
	{
		m_pSamplerPoint->Release();
		m_pSamplerPoint = nullptr;
	}

	if (m_EffectSamplerVariable)
	{
		m_EffectSamplerVariable->Release();
		m_EffectSamplerVariable = nullptr;
	}
}

void VehicleEffect::SetPointSampling()
{
	HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerPoint);
	if (SUCCEEDED(hr))
	{
		printf("Sampler state set to Point.\n");
	}
}

void VehicleEffect::SetLinearSampling()
{
	HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerLinear);
	if (SUCCEEDED(hr))
	{
		printf("Sampler state set to Linear.\n");
	}
}

void VehicleEffect::SetAnisotropicSampling()
{
	HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerAnisotropic);
	if (SUCCEEDED(hr))
	{
		printf("Sampler state set to Anisotropic.\n");
	}
}

void VehicleEffect::SetDiffuseMap(Texture* pDiffuseTexture)
{
	if (m_pDiffuseMapVariable) {
		m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
	}
}

void VehicleEffect::SetNormalMap(Texture* pNormalTexture)
{
	if (m_pNormalMapVariable) {
		m_pNormalMapVariable->SetResource(pNormalTexture->GetShaderResourceView());
	}
}

void VehicleEffect::SetGlossinessMap(Texture* pGlossinessTexture)
{
	if (m_pGlossinessMapVariable) {
		m_pGlossinessMapVariable->SetResource(pGlossinessTexture->GetShaderResourceView());
	}
}

void VehicleEffect::SetSpecularMap(Texture* pSpecularTexture)
{
	if (m_pSpecularMapVariable) {
		m_pSpecularMapVariable->SetResource(pSpecularTexture->GetShaderResourceView());
	}
}

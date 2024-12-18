#include "Effect.h"
#include <sstream>
#include "Mesh3D.h"
using namespace dae;

Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);

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
	

	m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechnique->IsValid())
	{
		std::wcout << L"Technique not valid\n";
	}

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProjectionMatrix")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())
	{
		std::wcout << L"m_pMatWorldViewProjVariable not valid\n";
	}

	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
	if (!m_pMatWorldVariable->IsValid())
	{
		std::wcout << L"m_pMatWorldVariable not valid\n";
	}

	m_pVecCameraVariable = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();
	if (!m_pVecCameraVariable->IsValid())
	{
		std::wcout << L"m_pVecCameraVariable not valid\n";
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

Effect::~Effect()
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

	//Release vectors
	if (m_pVecCameraVariable)
	{
		m_pVecCameraVariable->Release();
		m_pVecCameraVariable = nullptr;
	}
	
	//Release matrices
	if (m_pMatWorldViewProjVariable)
	{
		m_pMatWorldViewProjVariable->Release();
		m_pMatWorldViewProjVariable = nullptr;
	}

	if (m_pMatWorldVariable)
	{
		m_pMatWorldVariable->Release();
		m_pMatWorldVariable = nullptr;
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

	//Release Effect
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
}

ID3DX11Effect* Effect::GetEffect() const
{
	return m_pEffect;
}

ID3DX11EffectTechnique* Effect::GetTechnique() const
{
	return m_pTechnique;
}

void Effect::SetDiffuseMap(Texture* pDiffuseTexture)
{
	if (m_pDiffuseMapVariable) {
		m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetShaderResourceView());
	}
}

void Effect::SetNormalMap(Texture* pNormalTexture)
{
	if (m_pNormalMapVariable) {
		m_pNormalMapVariable->SetResource(pNormalTexture->GetShaderResourceView());
	}
}

void Effect::SetGlossinessMap(Texture* pGlossinessTexture)
{
	if (m_pGlossinessMapVariable) {
		m_pGlossinessMapVariable->SetResource(pGlossinessTexture->GetShaderResourceView());
	}
}

void Effect::SetSpecularMap(Texture* pSpecularTexture)
{
	if (m_pSpecularMapVariable) {
		m_pSpecularMapVariable->SetResource(pSpecularTexture->GetShaderResourceView());
	}
}

ID3DX11EffectVectorVariable* Effect::GetCameraPosition() const
{
	return m_pVecCameraVariable;
}

ID3DX11EffectMatrixVariable* Effect::GetWorldViewProjectionMatrix() const
{
	return m_pMatWorldViewProjVariable;
}

ID3DX11EffectMatrixVariable* Effect::GetWorldMatrix() const
{
	return m_pMatWorldVariable;
}

ID3DX11Effect* Effect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result = S_OK;
	ID3D10Blob* pErrorBlob = nullptr;
	ID3DX11Effect* pEffect;

	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			char* pErrors = (char*)pErrorBlob->GetBufferPointer();

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss << pErrors[i];

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << '\n';
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::wcout << ss.str() << '\n';
			return nullptr;
		}
	}
	pErrorBlob->Release();
	return pEffect;
}

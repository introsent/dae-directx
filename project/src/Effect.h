#pragma once
#include "pch.h"
#include "Texture.h"
class Mesh3D;
class Effect final
{
public:
	Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
	~Effect();

	Effect(const Effect& other) = delete;
	Effect& operator=(const Effect& rhs) = delete;
	Effect(Effect&& other) = delete;
	Effect& operator=(Effect&& rhs) = delete;

	ID3DX11Effect* GetEffect() const;
	ID3DX11EffectTechnique* GetTechnique() const;

	void SetPointSampling()
	{
		HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerPoint);
		if (SUCCEEDED(hr))
		{
			printf("Sampler state set to Point.\n");
		}
	}

	void SetLinearSampling()
	{
		HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerLinear);
		if (SUCCEEDED(hr))
		{
			printf("Sampler state set to Linear.\n");
		}
	}

	void SetAnisotropicSampling()
	{
		HRESULT hr = m_EffectSamplerVariable->SetSampler(0, m_pSamplerAnisotropic);
		if (SUCCEEDED(hr))
		{
			printf("Sampler state set to Anisotropic.\n");
		}
	}

	void SetDiffuseMap(Texture* pDiffuseTexture);
	void SetNormalMap(Texture* pNormalTexture);
	void SetGlossinessMap(Texture* pGlossinessTexture);
	void SetSpecularMap(Texture* pSpecularTexture);


	ID3DX11EffectVectorVariable* GetCameraPosition() const;
	ID3DX11EffectMatrixVariable* GetWorldViewProjectionMatrix() const;
	ID3DX11EffectMatrixVariable* GetWorldMatrix() const;
private:
	ID3DX11Effect* m_pEffect;

	ID3D11SamplerState* m_pSamplerPoint;
	ID3D11SamplerState* m_pSamplerLinear;
	ID3D11SamplerState* m_pSamplerAnisotropic;
	ID3DX11EffectSamplerVariable* m_EffectSamplerVariable;

	ID3DX11EffectTechnique* m_pTechnique;

	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable;

	ID3DX11EffectVectorVariable* m_pVecCameraVariable;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

};
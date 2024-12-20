#pragma once
#include "pch.h"
#include "Effect.h"
class VehicleEffect : public Effect
{
public:
    VehicleEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
    virtual ~VehicleEffect();

    VehicleEffect(const Effect& other) = delete;
    VehicleEffect& operator=(const Effect& rhs) = delete;
    VehicleEffect(Effect&& other) = delete;
    VehicleEffect& operator=(Effect&& rhs) = delete;

	void SetPointSampling();
	void SetLinearSampling();
    void SetAnisotropicSampling();

	void SetDiffuseMap(Texture* pDiffuseTexture);
	void SetNormalMap(Texture* pNormalTexture);
	void SetGlossinessMap(Texture* pGlossinessTexture);
    void SetSpecularMap(Texture* pSpecularTexture);

protected:
    ID3D11SamplerState* m_pSamplerPoint;
    ID3D11SamplerState* m_pSamplerLinear;
    ID3D11SamplerState* m_pSamplerAnisotropic;
    ID3DX11EffectSamplerVariable* m_EffectSamplerVariable;

    ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
    ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
    ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
    ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;
};

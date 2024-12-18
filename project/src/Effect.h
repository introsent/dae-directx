#pragma once
#include "pch.h"
#include "Texture.h"
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
	//ID3DX11EffectTechnique* GetTechnique() const;

	ID3DX11EffectTechnique* GetTechniquePoint() const;
	ID3DX11EffectTechnique* GetTechniqueLinear() const;
	ID3DX11EffectTechnique* GetTechniqueAnisotropic() const;

	void SetDiffuseMap(Texture* pDiffuseTexture);

	ID3DX11EffectMatrixVariable* GetWorldViewProjectionMatrix() const;
private:
	ID3DX11Effect* m_pEffect;
	//ID3DX11EffectTechnique* m_pTechnique;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;

	ID3DX11EffectTechnique* m_pTechniquePoint;
	ID3DX11EffectTechnique* m_pTechniqueLinear;
	ID3DX11EffectTechnique* m_pTechniqueAnisotropic;

	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

};
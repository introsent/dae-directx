#pragma once
#include "pch.h"
#include "Texture.h"

class Effect
{
public:
    Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
    virtual ~Effect();

    Effect(const Effect& other) = delete;
    Effect& operator=(const Effect& rhs) = delete;
    Effect(Effect&& other) = delete;
    Effect& operator=(Effect&& rhs) = delete;

    ID3DX11Effect* GetEffect() const;

    ID3DX11EffectVectorVariable* GetCameraPosition() const;
    ID3DX11EffectMatrixVariable* GetWorldViewProjectionMatrix() const;
    ID3DX11EffectMatrixVariable* GetWorldMatrix() const;


    ID3DX11EffectTechnique* GetTechnique() const;

protected:
    ID3DX11Effect* m_pEffect;
    ID3DX11EffectTechnique* m_pTechnique;

    ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
    ID3DX11EffectMatrixVariable* m_pMatWorldVariable;
    ID3DX11EffectVectorVariable* m_pVecCameraVariable;

    static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
};

//--------------------------------------------------------------------------------------
// File: EnvironmenSMapEffect.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "EffectCommon.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;


// Constant buffer layout. Must match the shader!
struct EnvironmenSMapEffectConstants
{
    XMVECTOR environmenSMapSpecular;
    float environmenSMapAmount;
    float fresnelFactor;
    float pad[2];

    XMVECTOR diffuseColor;
    XMVECTOR emissiveColor;
    
    XMVECTOR lightDirection[IEffectLights::MaxDirectionalLights];
    XMVECTOR lightDiffuseColor[IEffectLights::MaxDirectionalLights];

    XMVECTOR eyePosition;

    XMVECTOR fogColor;
    XMVECTOR fogVector;

    XMMATRIX world;
    XMVECTOR worldInverseTranspose[3];
    XMMATRIX worldViewProj;
};

static_assert((sizeof(EnvironmenSMapEffectConstants) % 16) == 0, "CB size not padded correctly");


// Traits type describes our characteristics to the EffectBase template.
struct EnvironmenSMapEffectTraits
{
    using ConstantBufferType = EnvironmenSMapEffectConstants;

    static constexpr int VertexShaderCount = 12;
    static constexpr int PixelShaderCount = 16;
    static constexpr int ShaderPermutationCount = 56;

    static constexpr int MappingCount = 3;
};


// Internal EnvironmenSMapEffect implementation class.
class EnvironmenSMapEffect::Impl : public EffectBase<EnvironmenSMapEffectTraits>
{
public:
    Impl(_In_ ID3D11Device* device);

    bool preferPerPixelLighting;
    bool fresnelEnabled;
    bool specularEnabled;
    bool biasedVertexNormals;
    EnvironmenSMapEffect::Mapping mapping;

    EffectLights lights;

    ComPtr<ID3D11ShaderResourceView> environmenSMap;

    int GetCurrentShaderPermutation() const noexcept;

    void Apply(_In_ ID3D11DeviceContext* deviceContext);
};


// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMap.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapOneLight.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapOneLightFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapPixelLighting.inc"

    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapFresnelBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapOneLightBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapOneLightFresnelBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_VSEnvMapPixelLightingBn.inc"

    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMap.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpecular.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpecularNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapPixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapPixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapPixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpherePixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpherePixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapDualParabolaPixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog.inc"
#else
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMap.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapFresnel.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapOneLight.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapOneLightFresnel.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapPixelLightingSM4.inc"

    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapBn.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapFresnelBn.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapOneLightBn.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapOneLightFresnelBn.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapPixelLightingBn.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_VSEnvMapPixelLightingBnSM4.inc"

    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMap.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapNoFog.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpecular.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpecularNoFog.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapPixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapPixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapPixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpherePixelLighting.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpherePixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog.inc"
#endif
}


template<>
const ShaderBytecode EffectBase<EnvironmenSMapEffectTraits>::VertexShaderBytecode[] =
{
    { EnvironmenSMapEffect_VSEnvMap,                   sizeof(EnvironmenSMapEffect_VSEnvMap)                   },
    { EnvironmenSMapEffect_VSEnvMapFresnel,            sizeof(EnvironmenSMapEffect_VSEnvMapFresnel)            },
    { EnvironmenSMapEffect_VSEnvMapOneLight,           sizeof(EnvironmenSMapEffect_VSEnvMapOneLight)           },
    { EnvironmenSMapEffect_VSEnvMapOneLightFresnel,    sizeof(EnvironmenSMapEffect_VSEnvMapOneLightFresnel)    },
    { EnvironmenSMapEffect_VSEnvMapPixelLighting,      sizeof(EnvironmenSMapEffect_VSEnvMapPixelLighting)      },

    { EnvironmenSMapEffect_VSEnvMapBn,                 sizeof(EnvironmenSMapEffect_VSEnvMapBn)                 },
    { EnvironmenSMapEffect_VSEnvMapFresnelBn,          sizeof(EnvironmenSMapEffect_VSEnvMapFresnelBn)          },
    { EnvironmenSMapEffect_VSEnvMapOneLightBn,         sizeof(EnvironmenSMapEffect_VSEnvMapOneLightBn)         },
    { EnvironmenSMapEffect_VSEnvMapOneLightFresnelBn,  sizeof(EnvironmenSMapEffect_VSEnvMapOneLightFresnelBn)  },
    { EnvironmenSMapEffect_VSEnvMapPixelLightingBn,    sizeof(EnvironmenSMapEffect_VSEnvMapPixelLightingBn)    },

#if defined(_XBOX_ONE) && defined(_TITLE)
    { EnvironmenSMapEffect_VSEnvMapPixelLighting,      sizeof(EnvironmenSMapEffect_VSEnvMapPixelLighting)      },
    { EnvironmenSMapEffect_VSEnvMapPixelLightingBn,    sizeof(EnvironmenSMapEffect_VSEnvMapPixelLightingBn)    },
#else
    { EnvironmenSMapEffect_VSEnvMapPixelLightingSM4,   sizeof(EnvironmenSMapEffect_VSEnvMapPixelLightingSM4)   },
    { EnvironmenSMapEffect_VSEnvMapPixelLightingBnSM4, sizeof(EnvironmenSMapEffect_VSEnvMapPixelLightingBnSM4) },
#endif
};


template<>
const int EffectBase<EnvironmenSMapEffectTraits>::VertexShaderIndices[] =
{
    0,      // basic
    0,      // basic, no fog
    1,      // fresnel
    1,      // fresnel, no fog
    0,      // specular
    0,      // specular, no fog
    1,      // fresnel + specular
    1,      // fresnel + specular, no fog

    2,      // one light
    2,      // one light, no fog
    3,      // one light, fresnel
    3,      // one light, fresnel, no fog
    2,      // one light, specular
    2,      // one light, specular, no fog
    3,      // one light, fresnel + specular
    3,      // one light, fresnel + specular, no fog

    4,      // pixel lighting
    4,      // pixel lighting, no fog
    4,      // pixel lighting, fresnel
    4,      // pixel lighting, fresnel, no fog

    5,      // basic (biased vertex normals)
    5,      // basic (biased vertex normals), no fog
    6,      // fresnel (biased vertex normals)
    6,      // fresnel (biased vertex normals), no fog
    5,      // specular (biased vertex normals)
    5,      // specular (biased vertex normals), no fog
    6,      // fresnel + specular (biased vertex normals)
    6,      // fresnel + specular (biased vertex normals), no fog

    7,      // one light (biased vertex normals)
    7,      // one light (biased vertex normals), no fog
    8,      // one light (biased vertex normals), fresnel
    8,      // one light (biased vertex normals), fresnel, no fog
    7,      // one light (biased vertex normals), specular
    7,      // one light (biased vertex normals), specular, no fog
    8,      // one light (biased vertex normals), fresnel + specular
    8,      // one light (biased vertex normals), fresnel + specular, no fog

    9,      // pixel lighting (biased vertex normals)
    9,      // pixel lighting (biased vertex normals), no fog
    9,      // pixel lighting (biased vertex normals), fresnel
    9,      // pixel lighting (biased vertex normals), fresnel, no fog

    4,      // spheremap pixel lighting
    4,      // spheremap pixel lighting, no fog
    4,      // spheremap pixel lighting, fresnel
    4,      // spheremap pixel lighting, fresnel, no fog

    9,      // spheremap pixel lighting (biased vertex normals)
    9,      // spheremap pixel lighting (biased vertex normals), no fog
    9,      // spheremap pixel lighting (biased vertex normals), fresnel
    9,      // spheremap pixel lighting (biased vertex normals), fresnel, no fog

    10,     // dual-parabola pixel lighting
    10,     // dual-parabola pixel lighting, no fog
    10,     // dual-parabola pixel lighting, fresnel
    10,     // dual-parabola pixel lighting, fresnel, no fog

    11,     // dual-parabola pixel lighting (biased vertex normals)
    11,     // dual-parabola pixel lighting (biased vertex normals), no fog
    11,     // dual-parabola pixel lighting (biased vertex normals), fresnel
    11,     // dual-parabola pixel lighting (biased vertex normals), fresnel, no fog
};


template<>
const ShaderBytecode EffectBase<EnvironmenSMapEffectTraits>::PixelShaderBytecode[] =
{
    { EnvironmenSMapEffect_PSEnvMap,                          sizeof(EnvironmenSMapEffect_PSEnvMap)                          },
    { EnvironmenSMapEffect_PSEnvMapNoFog,                     sizeof(EnvironmenSMapEffect_PSEnvMapNoFog)                     },
    { EnvironmenSMapEffect_PSEnvMapSpecular,                  sizeof(EnvironmenSMapEffect_PSEnvMapSpecular)                  },
    { EnvironmenSMapEffect_PSEnvMapSpecularNoFog,             sizeof(EnvironmenSMapEffect_PSEnvMapSpecularNoFog)             },
    { EnvironmenSMapEffect_PSEnvMapPixelLighting,             sizeof(EnvironmenSMapEffect_PSEnvMapPixelLighting)             },
    { EnvironmenSMapEffect_PSEnvMapPixelLightingNoFog,        sizeof(EnvironmenSMapEffect_PSEnvMapPixelLightingNoFog)        },
    { EnvironmenSMapEffect_PSEnvMapPixelLightingFresnel,      sizeof(EnvironmenSMapEffect_PSEnvMapPixelLightingFresnel)      },
    { EnvironmenSMapEffect_PSEnvMapPixelLightingFresnelNoFog, sizeof(EnvironmenSMapEffect_PSEnvMapPixelLightingFresnelNoFog) },

    { EnvironmenSMapEffect_PSEnvMapSpherePixelLighting,             sizeof(EnvironmenSMapEffect_PSEnvMapSpherePixelLighting) },
    { EnvironmenSMapEffect_PSEnvMapSpherePixelLightingNoFog,        sizeof(EnvironmenSMapEffect_PSEnvMapSpherePixelLightingNoFog) },
    { EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnel,      sizeof(EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnel) },
    { EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog, sizeof(EnvironmenSMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog) },

    { EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLighting,             sizeof(EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLighting) },
    { EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingNoFog,        sizeof(EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingNoFog) },
    { EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnel,      sizeof(EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnel) },
    { EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog, sizeof(EnvironmenSMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog) },
};


template<>
const int EffectBase<EnvironmenSMapEffectTraits>::PixelShaderIndices[] =
{
    0,      // basic
    1,      // basic, no fog
    0,      // fresnel
    1,      // fresnel, no fog
    2,      // specular
    3,      // specular, no fog
    2,      // fresnel + specular
    3,      // fresnel + specular, no fog

    0,      // one light
    1,      // one light, no fog
    0,      // one light, fresnel
    1,      // one light, fresnel, no fog
    2,      // one light, specular
    3,      // one light, specular, no fog
    2,      // one light, fresnel + specular
    3,      // one light, fresnel + specular, no fog

    4,      // per pixel lighting
    5,      // per pixel lighting, no fog
    6,      // per pixel lighting, fresnel
    7,      // per pixel lighting, fresnel, no fog

    0,      // basic (biased vertex normals)
    1,      // basic (biased vertex normals), no fog
    0,      // fresnel (biased vertex normals)
    1,      // fresnel (biased vertex normals), no fog
    2,      // specular (biased vertex normals)
    3,      // specular (biased vertex normals), no fog
    2,      // fresnel + specular (biased vertex normals)
    3,      // fresnel + specular (biased vertex normals), no fog

    0,      // one light (biased vertex normals)
    1,      // one light (biased vertex normals), no fog
    0,      // one light (biased vertex normals), fresnel
    1,      // one light (biased vertex normals), fresnel, no fog
    2,      // one light (biased vertex normals), specular
    3,      // one light (biased vertex normals), specular, no fog
    2,      // one light (biased vertex normals), fresnel + specular
    3,      // one light (biased vertex normals), fresnel + specular, no fog

    4,      // per pixel lighting (biased vertex normals)
    5,      // per pixel lighting (biased vertex normals), no fog
    6,      // per pixel lighting (biased vertex normals), fresnel
    7,      // per pixel lighting (biased vertex normals), fresnel, no fog

    8,      // spheremap pixel lighting
    9,      // spheremap pixel lighting, no fog
    10,     // spheremap pixel lighting, fresnel
    11,     // spheremap pixel lighting, fresnel, no fog

    8,      // spheremap pixel lighting (biased vertex normals)
    9,      // spheremap pixel lighting (biased vertex normals), no fog
    10,     // spheremap pixel lighting (biased vertex normals), fresnel
    11,     // spheremap pixel lighting (biased vertex normals), fresnel, no fog

    12,     // dual-parabola pixel lighting
    13,     // dual-parabola pixel lighting, no fog
    14,     // dual-parabola pixel lighting, fresnel
    15,     // dual-parabola pixel lighting, fresnel, no fog

    12,     // dual-parabola pixel lighting (biased vertex normals)
    13,     // dual-parabola pixel lighting (biased vertex normals), no fog
    14,     // dual-parabola pixel lighting (biased vertex normals), fresnel
    15,     // dual-parabola pixel lighting (biased vertex normals), fresnel, no fog
};


// Global pool of per-device EnvironmenSMapEffect resources.
template<>
SharedResourcePool<ID3D11Device*, EffectBase<EnvironmenSMapEffectTraits>::DeviceResources> EffectBase<EnvironmenSMapEffectTraits>::deviceResourcesPool = {};


// Constructor.
EnvironmenSMapEffect::Impl::Impl(_In_ ID3D11Device* device)
    : EffectBase(device),
    preferPerPixelLighting(false),
    fresnelEnabled(true),
    specularEnabled(false),
    biasedVertexNormals(false),
    mapping(Mapping_Cube)
{
    static_assert(static_cast<int>(std::size(EffectBase<EnvironmenSMapEffectTraits>::VertexShaderIndices)) == EnvironmenSMapEffectTraits::ShaderPermutationCount, "array/max mismatch");
    static_assert(static_cast<int>(std::size(EffectBase<EnvironmenSMapEffectTraits>::VertexShaderBytecode)) == EnvironmenSMapEffectTraits::VertexShaderCount, "array/max mismatch");
    static_assert(static_cast<int>(std::size(EffectBase<EnvironmenSMapEffectTraits>::PixelShaderBytecode)) == EnvironmenSMapEffectTraits::PixelShaderCount, "array/max mismatch");
    static_assert(static_cast<int>(std::size(EffectBase<EnvironmenSMapEffectTraits>::PixelShaderIndices)) == EnvironmenSMapEffectTraits::ShaderPermutationCount, "array/max mismatch");

    constants.environmenSMapAmount = 1;
    constants.fresnelFactor = 1;

    XMVECTOR unwantedOutput[MaxDirectionalLights];

    lights.InitializeConstants(unwantedOutput[0], constants.lightDirection, constants.lightDiffuseColor, unwantedOutput);
}


int EnvironmenSMapEffect::Impl::GetCurrentShaderPermutation() const noexcept
{
    int permutation = 0;

    // Use optimized shaders if fog is disabled.
    if (!fog.enabled)
    {
        permutation += 1;
    }

    // Support fresnel?
    if (fresnelEnabled)
    {
        permutation += 2;
    }

    if (mapping == Mapping_Sphere)
    {
        permutation += 40;

        if (biasedVertexNormals)
        {
            permutation += 4;
        }
    }
    else if (mapping == Mapping_DualParabola)
    {
        permutation += 48;

        if (biasedVertexNormals)
        {
            permutation += 4;
        }
    }
    else // Mapping_Cube
    {
        if (preferPerPixelLighting)
        {
            permutation += 16;
        }
        else
        {
            // Support specular?
            if (specularEnabled)
            {
                permutation += 4;
            }

            // Use the only-bother-with-the-first-light shader optimization?
            if (!lights.lightEnabled[1] && !lights.lightEnabled[2])
            {
                permutation += 8;
            }
        }

        if (biasedVertexNormals)
        {
            // Compressed normals need to be scaled and biased in the vertex shader.
            permutation += 20;
        }
    }

    return permutation;
}


// Sets our state onto the D3D device.
void EnvironmenSMapEffect::Impl::Apply(_In_ ID3D11DeviceContext* deviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);
            
    lights.SetConstants(dirtyFlags, matrices, constants.world, constants.worldInverseTranspose, constants.eyePosition, constants.diffuseColor, constants.emissiveColor, true);

    // Set the textures.
    ID3D11ShaderResourceView* textures[2] =
    {
        texture.Get(),
        environmenSMap.Get(),
    };

    deviceContext->PSSetShaderResources(0, 2, textures);
    
    // Set shaders and constant buffers.
    ApplyShaders(deviceContext, GetCurrentShaderPermutation());
}


// Public constructor.
EnvironmenSMapEffect::EnvironmenSMapEffect(_In_ ID3D11Device* device)
  : pImpl(std::make_unique<Impl>(device))
{
}


// Move constructor.
EnvironmenSMapEffect::EnvironmenSMapEffect(EnvironmenSMapEffect&& moveFrom) noexcept
  : pImpl(std::move(moveFrom.pImpl))
{
}


// Move assignment.
EnvironmenSMapEffect& EnvironmenSMapEffect::operator= (EnvironmenSMapEffect&& moveFrom) noexcept
{
    pImpl = std::move(moveFrom.pImpl);
    return *this;
}


// Public destructor.
EnvironmenSMapEffect::~EnvironmenSMapEffect()
{
}


// IEffect methods.
void EnvironmenSMapEffect::Apply(_In_ ID3D11DeviceContext* deviceContext)
{
    pImpl->Apply(deviceContext);
}


void EnvironmenSMapEffect::GetVertexShaderBytecode(_Out_ void const** pShaderByteCode, _Out_ size_t* pByteCodeLength)
{
    pImpl->GetVertexShaderBytecode(pImpl->GetCurrentShaderPermutation(), pShaderByteCode, pByteCodeLength);
}


// Camera settings.
void XM_CALLCONV EnvironmenSMapEffect::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}


void XM_CALLCONV EnvironmenSMapEffect::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}


void XM_CALLCONV EnvironmenSMapEffect::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}


void XM_CALLCONV EnvironmenSMapEffect::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}


// Material settings.
void XM_CALLCONV EnvironmenSMapEffect::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}


void XM_CALLCONV EnvironmenSMapEffect::SetEmissiveColor(FXMVECTOR value)
{
    pImpl->lights.emissiveColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}


void EnvironmenSMapEffect::SetAlpha(float value)
{
    pImpl->lights.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}


void XM_CALLCONV EnvironmenSMapEffect::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;
    pImpl->lights.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}


// Light settings.
void EnvironmenSMapEffect::SetLightingEnabled(bool value)
{
    if (!value)
    {
        throw std::invalid_argument("EnvironmenSMapEffect does not support turning off lighting");
    }
}


void EnvironmenSMapEffect::SetPerPixelLighting(bool value)
{
    pImpl->preferPerPixelLighting = value;
}


void XM_CALLCONV EnvironmenSMapEffect::SetAmbientLightColor(FXMVECTOR value)
{
    pImpl->lights.ambientLightColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}


void EnvironmenSMapEffect::SetLightEnabled(int whichLight, bool value)
{
    XMVECTOR unwantedOutput[MaxDirectionalLights] = {};

    pImpl->dirtyFlags |= pImpl->lights.SetLightEnabled(whichLight, value, pImpl->constants.lightDiffuseColor, unwantedOutput);
}


void XM_CALLCONV EnvironmenSMapEffect::SetLightDirection(int whichLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(whichLight);

    pImpl->constants.lightDirection[whichLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}


void XM_CALLCONV EnvironmenSMapEffect::SetLightDiffuseColor(int whichLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightDiffuseColor(whichLight, value, pImpl->constants.lightDiffuseColor);
}


void XM_CALLCONV EnvironmenSMapEffect::SetLightSpecularColor(int, FXMVECTOR)
{
    // Unsupported interface method.
}


void EnvironmenSMapEffect::EnableDefaultLighting()
{
    EffectLights::EnableDefaultLighting(this);
}


// Fog settings.
void EnvironmenSMapEffect::SetFogEnabled(bool value)
{
    pImpl->fog.enabled = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}


void EnvironmenSMapEffect::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}


void EnvironmenSMapEffect::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}


void XM_CALLCONV EnvironmenSMapEffect::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}


// Texture settings.
void EnvironmenSMapEffect::SetTexture(_In_opt_ ID3D11ShaderResourceView* value)
{
    pImpl->texture = value;
}


void EnvironmenSMapEffect::SetEnvironmenSMap(_In_opt_ ID3D11ShaderResourceView* value)
{
    pImpl->environmenSMap = value;
}


// Additional settings.
void EnvironmenSMapEffect::SetMode(EnvironmenSMapEffect::Mapping mapping)
{
    if (static_cast<int>(mapping) < 0 || static_cast<int>(mapping) >= EnvironmenSMapEffectTraits::MappingCount)
    {
        throw std::invalid_argument("Unsupported mapping");
    }

    if (mapping == Mapping_DualParabola)
    {
        if (pImpl->GetDeviceFeatureLevel() < D3D_FEATURE_LEVEL_10_0)
        {
            throw std::runtime_error("Dual Parabola requires Feature Level 10.0 or later");
        }
    }

    pImpl->mapping = mapping;
}


void EnvironmenSMapEffect::SetEnvironmenSMapAmount(float value)
{
    pImpl->constants.environmenSMapAmount = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}


void XM_CALLCONV EnvironmenSMapEffect::SetEnvironmenSMapSpecular(FXMVECTOR value)
{
    pImpl->constants.environmenSMapSpecular = value;

    pImpl->specularEnabled = !XMVector3Equal(value, XMVectorZero());

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}


void EnvironmenSMapEffect::SetFresnelFactor(float value)
{
    pImpl->constants.fresnelFactor = value;

    pImpl->fresnelEnabled = (value != 0);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}


// Normal compression settings.
void EnvironmenSMapEffect::SetBiasedVertexNormals(bool value)
{
    pImpl->biasedVertexNormals = value;
}

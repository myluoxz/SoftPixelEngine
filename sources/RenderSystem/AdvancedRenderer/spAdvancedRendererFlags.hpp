/*
 * Advanced renderer flags header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_ADVANCED_RENDERER_FLAGS_H__
#define __SP_ADVANCED_RENDERER_FLAGS_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_ADVANCEDRENDERER)


#include "Base/spMaterialConfigTypes.hpp"


namespace sp
{
namespace video
{


/**
Advanced renderer flags. Use this for the "AdvancedRenderer::generateResources" function.
\see DeferredRenderer
\see ForwardRenderer
*/
enum EAdvancedRenderFlags
{
    /**
    Makes use of the texture transformation matrix. This will be used for the
    diffuse-, specular-, normal- and height map.
    */
    RENDERERFLAG_USE_TEXTURE_MATRIX         = 0x00000001,
    /**
    Enables individual specular map usage. If this option is enabled every model
    must have an additional texture (layer 1) with specular information.
    */
    RENDERERFLAG_HAS_SPECULAR_MAP           = 0x00000002,
    /**
    Enables individual light map usage. If this option is enabled every model must have
    an addition texture (layer 1 if there is no specular map, otherwise 2) with illumination data.
    Those lightmaps can be generated with the "SoftPixel Sandbox" or rather the lightmap-generator from this engine.
    */
    RENDERERFLAG_HAS_LIGHT_MAP              = 0x00000004,
    //! Allows the lighting to over-blend. By default disabled.
    RENDERERFLAG_ALLOW_OVERBLENDING         = 0x00000008,
    
    /**
    Enables normal-mapping. If this option is enabled every model must have an additional
    texture (layer 1 if there is no specular map and no light map, otherwise 2 or 3) with normal vector information.
    */
    RENDERERFLAG_NORMAL_MAPPING             = 0x00000010,
    /**
    Enables parallax-occlusion mapping. If this option is enabled every model must have an additional
    texture (layer 2 if there is no specular map and no light map, otherwise 3 or 4) with height map information.
    This can be a gray-scaled texture. If the RENDERERFLAG_NORMALMAP_XYZ_H option is enabled,
    no height map is needed. In that case the height map information is get from the normal map's alpha channel.
    This requires normal-mapping (RENDERERFLAG_NORMAL_MAPPING).
    */
    RENDERERFLAG_PARALLAX_MAPPING           = 0x00000020,
    /**
    Enables the normal map to also contain the height map data in the alpha channel. This this option is enabled
    no height map texture is used. This requires parallax-mapping (RENDERERFLAG_PARALLAX_MAPPING).
    */
    RENDERERFLAG_NORMALMAP_XYZ_H            = 0x00000040,
    /**
    Enables shadow mapping. For this technique "variance shadow mapping" (VSM)
    is used for performance reasons.
    */
    RENDERERFLAG_SHADOW_MAPPING             = 0x00000080,
    /**
    Enables global illumination. For this technique "reflective shadow maps" are used.
    This requires shadow-mapping (RENDERERFLAG_SHADOW_MAPPING).
    \note This is a very time consuming process. It is recommendable to use only one
    light source with this technique.
    \todo This is still in progress!
    */
    RENDERERFLAG_GLOBAL_ILLUMINATION        = 0x00000100,
    /**
    Enables render optimization path for global-illumination.
    This requires global-illumination (RENDERERFLAG_GLOBAL_ILLUMINATION).
    \note This should only be used if normal mapping is disabled or the scene
    has only a few normal maps. Otherwise this method has a negative effect.
    \todo This is incomplete!
    */
    RENDERERFLAG_USE_VPL_OPTIMIZATION       = 0x00000200,
    /*
    Enables tiled deferred shading. This can optimize the use of
    thousands of light sources in a large scene. If you have many lights
    in a small scene, this method can have a negative effect.
    \note This method requires computer shaders.
    It won't run on a GPU which does not support GPGPU.
    \todo This is still in progress!
    */
    RENDERERFLAG_TILED_SHADING              = 0x00000400,

    //! Enables the bloom effect. All glossy surfaces glow intensely.
    RENDERERFLAG_BLOOM                      = 0x00000800,
    
    #if 0
    /**
    Enables height-field tessellation. This can not be used together
    with parallax-mapping (RENDERERFLAG_PARALLAX_MAPPING).
    */
    RENDERERFLAG_TESSELLATION               = 0x00001000,
    #endif
    
    /**
    This option can be used for debugging purposes. It renders the final image as four viewports
    containing the color buffer output, normal buffer output, depth buffer output and the final image.
    */
    RENDERERFLAG_DEBUG_GBUFFER              = 0x01000000,
    /**
    This option can be used for debugging purposes. It renders the
    world-position for each pixel instead of its distance to the view camera.
    This requires gbuffer debugging (RENDERERFLAG_DEBUG_GBUFFER).
    */
    RENDERERFLAG_DEBUG_GBUFFER_WORLDPOS     = 0x02000000,
    /**
    This option can be used for debugging purposes. It renders the
    texture coordinates as colors for each pixel instead of the diffuse colors.
    This requires gbuffer debugging (RENDERERFLAG_DEBUG_GBUFFER).
    */
    RENDERERFLAG_DEBUG_GBUFFER_TEXCOORDS    = 0x04000000,
    /**
    This option can be used for debugging purposes. It renders all
    virtual points lights as small colored cubes. These virtual points lights
    are generated from the reflective shadow maps.
    This requires global illumination (RENDERERFLAG_GLOBAL_ILLUMINATION).
    */
    RENDERERFLAG_DEBUG_VIRTUALPOINTLIGHTS   = 0x08000000,
};


/**
Texture layer model for deferred-renderer. Use this structure to determine how the texture layers are constructed.
If a texture layer has the value of TEXTURE_IGNORE this layer is not used. By default every layer has the value of TEXTURE_IGNORE.
*/
struct STextureLayerModel
{
    STextureLayerModel() :
        DiffuseMap  (TEXTURE_IGNORE),
        SpecularMap (TEXTURE_IGNORE),
        LightMap    (TEXTURE_IGNORE),
        NormalMap   (TEXTURE_IGNORE),
        HeightMap   (TEXTURE_IGNORE)
    {
    }
    ~STextureLayerModel()
    {
    }
    
    /* Functions */
    void clear()
    {
        DiffuseMap  = TEXTURE_IGNORE;
        SpecularMap = TEXTURE_IGNORE;
        LightMap    = TEXTURE_IGNORE;
        NormalMap   = TEXTURE_IGNORE;
        HeightMap   = TEXTURE_IGNORE;
    }
    
    /* Members */
    u8 DiffuseMap;  //!< Diffuse map layer. Commonly 0.
    u8 SpecularMap; //!< Diffuse map layer. Commonly 1 or TEXTURE_IGNORE.
    u8 LightMap;    //!< Diffuse map layer. Commonly 1, 2 or TEXTURE_IGNORE.
    u8 NormalMap;   //!< Diffuse map layer. Commonly 1, 2, 3 or TEXTURE_IGNORE.
    /**
    Diffuse map layer. Commonly 1, 2, 3, 4 or TEXTURE_IGNORE.
    If the height-map information is stored in the alpha-channel
    of the normal-maps, this layer has the same value as the "NormalMap" layer.
    */
    u8 HeightMap;
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================

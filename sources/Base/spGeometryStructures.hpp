/*
 * Geometry structures header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_GEOMETRY_STRUCTURES_H__
#define __SP_GEOMETRY_STRUCTURES_H__


#include "Base/spStandard.hpp"
#include "Base/spMath.hpp"
#include "Base/spMaterialColor.hpp"


namespace sp
{
namespace scene
{


/*
 * Pre-declerations
 */

class SceneNode;
class Mesh;
class Terrain;
class Collision;


/*
 * Macros
 */

static const f32 DEF_PERSPECTIVE_FOV    = 74.0f;
static const s32 DEF_MESH_SEGMENTS      = 10;
static const s32 DEF_MESH_TESSELLATION  = 8;
static const s32 DEF_GEOMIP_LEVELS      = 5;
static const s32 DEF_SCENE_FLAGS        = ~0;


/*
 * Enumerations
 */

//! Object order types for sorting.
enum ENodeOrderTypes
{
    ORDER_FOREGROUND    = -1,
    ORDER_NORMAL        = 0,
    ORDER_BACKGROUND    = 1,
};

//! Light source models.
enum ELightModels
{
    LIGHT_DIRECTIONAL = 0,  //!< Directional light source. Position is unnecessary.
    LIGHT_POINT,            //!< Point light source. Direction is unnecessary.
    LIGHT_SPOT,             //!< Spot light source. Position and direction is necessary.
};

//! Basic meshes that can be constructed by the MeshGenerator.
enum EBasicMeshes
{
    /* Triangle based primitives */
    MESH_CUBE,          //!< Simple cube.
    MESH_CONE,          //!< Simple cone (with or without cap).
    MESH_CYLINDER,      //!< Simple cylinder (with or without cap).
    MESH_SPHERE,        //!< Simple sphere.
    MESH_ICOSPHERE,     //!< IcoSphere (like in Blender) generated by an Icosahedron with sub-devision.
    MESH_TORUS,         //!< Torus (or rather ring).
    MESH_TORUSKNOT,     //!< Torusknot.
    MESH_SPIRAL,        //!< Spiral with flexible rotation degree (with or without cap).
    MESH_PIPE,          //!< Pipe (with or without cap).
    MESH_PLANE,         //!< Simple plane (or rather quad).
    MESH_DISK,          //!< Simple disk (with or without cap, without cap it looks like a CD-ROM).
    MESH_CUBOCTAHEDRON, //!< Polyhedron Cuboctahedron.
    MESH_TETRAHEDRON,   //!< Platonic body: Tetrahedron.
    MESH_OCTAHEDRON,    //!< Platonic body: Octahedron.
    MESH_DODECAHEDRON,  //!< Platonic body: Dodecahedron.
    MESH_ICOSAHEDRON,   //!< Platonic body: Icosahedron.
    MESH_TEAPOT,        //!< Original "Utah Teapot". The 3D model par excellence ;-).
    
    /* Wire based primitives */
    MESH_WIRE_CUBE,     //!< Simple wire cube.
};

//! Mesh file formats
enum EMeshFileFormats
{
    MESHFORMAT_UNKNOWN, //!< Unknown file format.
    
    /* Supported formats */
    MESHFORMAT_SPM,     //!< SoftPixelMesh.
    MESHFORMAT_3DS,     //!< 3D Studio.
    MESHFORMAT_MS3D,    //!< Milkshape3D.
    MESHFORMAT_X,       //!< DirectX.
    MESHFORMAT_B3D,     //!< Blitz3D.
    MESHFORMAT_MD2,     //!< Quake II model.
    MESHFORMAT_MD3,     //!< Quake III model.
    MESHFORMAT_OBJ,     //!< Wavefront Object.
    
    /* Unsupported formats (just for identification) */
    MESHFORMAT_OGRE,    //!< OGRE Mesh (unsupported yet).
    MESHFORMAT_LWO,     //!< LightWave Object (unsupported yet).
};

//! Scene file formats
enum ESceneFileFormats
{
    SCENEFORMAT_UNKNOWN,    //!< Unknown file format.
    
    /* Supported formats */
    SCENEFORMAT_SPSB,       //!< SoftPixel Sandbox Scene.
    SCENEFORMAT_BSP1,       //!< BinarySpacePartition v.1 (Quake map).
    SCENEFORMAT_BSP3,       //!< BinarySpacePartition v.3 (Quake III Arena map).
};

//! Pre defined super shape models.
enum ESuperShapeModels
{
    SUPERSHAPE_SMALL_CRYSTAL1 = 0,
    SUPERSHAPE_SMALL_CRYSTAL2,
    
    SUPERSHAPE_SMALL_STAR1,
    SUPERSHAPE_SMALL_STAR2,
    SUPERSHAPE_SMALL_STAR3,
    SUPERSHAPE_SMALL_STAR4,
    
    SUPERSHAPE_BIG_STAR1,
    
    SUPERSHAPE_URCHIN1,
    
    SUPERSHAPE_CUSHION1,
    
    SUPERSHAPE_RANDOM
};


/*
 * Structures
 */

/* === Vertex structure (for drawing 2D primitives) */

//#define __DRAW2DXYZ__

//! \deprecated
class SPrimitiveVertex2D
{
    
    public:
        
        SPrimitiveVertex2D() : d3dColor(0xFFFFFFFF), oglColor(255)
        {
        }
        SPrimitiveVertex2D(f32 X, f32 Y, f32 U, f32 V, const video::color &Clr = 255, const f32 RHW = 1.0f)
            #ifndef __DRAW2DXYZ__
            : d3dPosition(X, Y, 0.0f, RHW), d3dColor(Clr.getSingle()), TexCoord(U, V),
            #else
            : d3dPosition(X, Y, 0.0f), d3dColor(Clr.getSingle()), TexCoord(U, V),
            #endif
            oglPosition(X / RHW, Y / RHW, 0.0f, 1.0f / RHW), oglColor(Clr)
        {
        }
        ~SPrimitiveVertex2D()
        {
        }
        
        /* === Functions === */
        
        inline void setPosition(const dim::vector4df &VertexPosition)
        {
            d3dPosition = VertexPosition;
            #ifndef __DRAW2DXYZ__
            oglPosition.W = 1.0f / VertexPosition.W;
            #endif
            oglPosition.X = VertexPosition.X * oglPosition.W;
            oglPosition.Y = VertexPosition.Y * oglPosition.W;
            oglPosition.Z = VertexPosition.Z * oglPosition.W;
        }
        inline void setPosition(const dim::point2di &VertexPosition)
        {
            #ifndef __DRAW2DXYZ__
            setPosition(dim::vector4df(
                static_cast<f32>(VertexPosition.X), static_cast<f32>(VertexPosition.Y), 0.0f
            ));
            #else
            setPosition(dim::vector3df(
                static_cast<f32>(VertexPosition.X), static_cast<f32>(VertexPosition.Y), 0.0f
            ));
            #endif
        }
        inline dim::vector4df getPosition() const
        {
            return d3dPosition;
        }
        
        inline void setColor(const video::color &VertexColor)
        {
            oglColor = VertexColor;
            d3dColor = VertexColor.getSingle();
        }
        inline video::color getColor() const
        {
            return oglColor;
        }
        
        inline void setTexCoord(const dim::point2df &VertexTexCoord)
        {
            TexCoord = VertexTexCoord;
        }
        inline dim::point2df getTexCoord() const
        {
            return TexCoord;
        }
        
    private:
        
        /* === Members === */
        
        //! Direct3D Coordinate (X, Y, Z, W)
        #ifndef __DRAW2DXYZ__
        dim::vector4df d3dPosition;
        #else
        dim::vector3df d3dPosition;
        #endif
        
        //! Direct3D Color (A, R, G, B)
        u32 d3dColor;
        
        //! Direct3D & OpenGL Texture coordinate (U, V)
        dim::point2df TexCoord;
        
        //! OpenGL Coordinate (X, Y, Z, W)
        dim::vector4df oglPosition;
        
        //! OpenGL Color (R, G, B, A)
        video::color oglColor;
        
};


/* === Vertex structure (for drawing triangle lists) === */

//! \deprecated
class SMeshVertex3D
{
    
    public:
        
        SMeshVertex3D()
            : Normal(0.f, 0.f, 1.f), Fog(0.f)
        {
            setColor(0xFFFFFFFF);
        }
        SMeshVertex3D(f32 X, f32 Y, f32 Z, const video::color &Clr)
            : Position(X, Y, Z), Normal(0.f, 0.f, 1.f), Fog(0.f)
        {
            setColor(Clr);
        }
        SMeshVertex3D(f32 X, f32 Y, f32 Z, u32 Clr, f32 U = 0.0f, f32 V = 0.0f)
            : Position(X, Y, Z), Normal(0.f, 0.f, 1.f), Fog(0.f)
        {
            setColor(Clr);
            for (s32 i = 0; i < MAX_COUNT_OF_TEXTURES; ++i)
                TexCoord[i] = dim::vector3df(U, V, 0.f);
        }
        
        /* === Functions === */
        
        inline void setPosition(const dim::vector3df &VertexPosition)
        {
            Position = VertexPosition;
        }
        inline dim::vector3df getPosition() const
        {
            return Position;
        }
        inline dim::vector3df& getPosition()
        {
            return Position;
        }
        
        inline void setNormal(const dim::vector3df &VertexNormal)
        {
            Normal = VertexNormal;
        }
        inline dim::vector3df getNormal() const
        {
            return Normal;
        }
        inline dim::vector3df& getNormal()
        {
            return Normal;
        }
        
        inline void setTexCoord(u8 Layer, const dim::vector3df &VertexTexCoord)
        {
            TexCoord[Layer] = VertexTexCoord;
        }
        inline dim::vector3df getTexCoord(u8 Layer) const
        {
            return TexCoord[Layer];
        }
        inline dim::vector3df& getTexCoord(u8 Layer)
        {
            return TexCoord[Layer];
        }
        
        inline void setColor(const video::color &VertexColor)
        {
            oglColor    = VertexColor;
            d3d9Color   = VertexColor.getSingle();
        }
        inline void setColor(const dim::vector4df &VertexColor)
        {
            oglColor    = video::color(
                (u8)(VertexColor.X * 255), (u8)(VertexColor.Y * 255), (u8)(VertexColor.Z * 255), (u8)(VertexColor.W * 255)
            );
            d3d9Color   = oglColor.getSingle();
        }
        inline void setColor(const u32 VertexColor)
        {
            const video::color Clr(
                video::getRed(VertexColor), video::getGreen(VertexColor), video::getBlue(VertexColor), video::getAlpha(VertexColor)
            );
            
            oglColor    = Clr;
            d3d9Color   = VertexColor;
        }
        inline video::color getColor() const
        {
            return oglColor;
        }
        inline video::color& getColor()
        {
            return oglColor;
        }
        
        inline void setFog(f32 VertexFog)
        {
            Fog = VertexFog;
        }
        inline f32 getFog() const
        {
            return Fog;
        }
        inline f32& getFog()
        {
            return Fog;
        }
        
        inline void setTangent(const dim::vector3df &VertexTangent)
        {
            Tangent = VertexTangent;
        }
        inline dim::vector3df getTangent() const
        {
            return Tangent;
        }
        inline dim::vector3df& getTangent()
        {
            return Tangent;
        }
        
        inline void setBinormal(const dim::vector3df &VertexBinormal)
        {
            Binormal = VertexBinormal;
        }
        inline dim::vector3df getBinormal() const
        {
            return Binormal;
        }
        inline dim::vector3df& getBinormal()
        {
            return Binormal;
        }
        
    private:
        
        /* === Members === */
        
        //! Coordination (X, Y, Z)
        dim::vector3df Position;
        
        //! Normal (NX, NY, NZ)
        dim::vector3df Normal;
        
        //! Direct3D9 Color (A, R, G, B)
        u32 d3d9Color;
        
        //! Texture coordinates [8 for multi-texturing] (U, V, W)
        dim::vector3df TexCoord[MAX_COUNT_OF_TEXTURES];
        
        //! OpenGL & Direct3D11 Color (R, G, B, A)
        video::color oglColor;
        
        //! Fog coordinate (0.0 - 1.0)
        f32 Fog;
        
        //! Tangent (only for Direct3D11)
        dim::vector3df Tangent;
        
        //! Binormal (only for Direct3D11)
        dim::vector3df Binormal;
        
};


/* === SMeshTriangle3D structure (for 3d mesh surfaces) === */

//! \deprecated
struct SMeshTriangle3D
{
    SMeshTriangle3D()
        : a(0), b(0), c(0)
    {
    }
    SMeshTriangle3D(u32 IndexA, u32 IndexB, u32 IndexC, u32 ExCounter = 0)
        : a(IndexA + ExCounter), b(IndexB + ExCounter), c(IndexC + ExCounter)
    {
    }
    ~SMeshTriangle3D()
    {
    }
    
    /* Functions */
    void flip()
    {
        math::Swap(a, c);
    }
    
    u32& operator [] (const s32 i)
    {
        return *(&a + i);
    }
    const u32 operator [] (const s32 i) const
    {
        return i < 3 ? *(&a + i) : 0;
    }
    
    //! Vertex indices
    u32 a, b, c;
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================

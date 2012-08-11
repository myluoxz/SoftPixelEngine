"// #########################################################\n"
"// # Direct3D11 Default Basic Shader 3D (Shader Model 4.0) #\n"
"// #########################################################\n"
"//\n"
"// ========== Copyright (c) 2010 - Lukas Hermanns ==========\n"
"//\n"
"\n"
"\n"
"/*\n"
" * Global members\n"
" */\n"
"\n"
"Texture2D Texture2D0        : register(t0);\n"
"Texture2D Texture2D1        : register(t1);\n"
"Texture2D Texture2D2        : register(t2);\n"
"Texture2D Texture2D3        : register(t3);\n"
"\n"
"SamplerState SamplerLinear0    : register(s0);\n"
"SamplerState SamplerLinear1    : register(s1);\n"
"SamplerState SamplerLinear2    : register(s2);\n"
"SamplerState SamplerLinear3    : register(s3);\n"
"\n"
"\n"
"/*\n"
" * Constant buffer structures\n"
" */\n"
"\n"
"struct SLight\n"
"{\n"
"    int Model;                            // Light model (Directionl, Point, Spot)\n"
"    int Enabled;                        // Enabled/ disabled\n"
"    int2 pad1;\n"
"    float4 Position;                    // Position for Point- and Spot light and Direction for Directional light\n"
"    float4 Diffuse, Ambient, Specular;    // Light colors\n"
"    float4 SpotDir;                        // Spot light direction\n"
"    float Attn0, Attn1, Attn2;            // Attunation values\n"
"    int pad2;\n"
"    float Theta, Phi, Falloff, Range;    // Spot light attributes\n"
"};\n"
"\n"
"struct SMaterial\n"
"{\n"
"    float4 Diffuse, Ambient, Specular, Emission;    // Material colors\n"
"    int Shading;                                    // Shading (flat, gouraud, phong, perpixel)\n"
"    int LightingEnabled;                            // Global lighting enabled/ disabled\n"
"    int FogEnabled;                                    // Global fog enabled/ disabled\n"
"    float Shininess;                                // Specular shininess\n"
"    int AlphaMethod;                                // Alpha test function\n"
"    float AlphaReference;                            // Alpha test reference value\n"
"    int2 pad;\n"
"};\n"
"\n"
"struct STexture\n"
"{\n"
"    int3 MapGenType;    // Texture coordinate generation\n"
"    int EnvType;        // Texture environment\n"
"    float4x4 Matrix;    // Texture coordiante transformation\n"
"};\n"
"\n"
"struct SClipPlane\n"
"{\n"
"    int Enabled;    // Enabled/ disabled\n"
"    int3 pad;\n"
"    float3 Normal;    // Plane normal vector\n"
"    float Distance;    // Plane distance to the origin\n"
"};\n"
"\n"
"struct SFogStates\n"
"{\n"
"    int Mode;            // Fog mode (Plane, Thick etc.)\n"
"    float Density;        // Density/ thickness\n"
"    float Near, Far;    // Near/ far planes\n"
"    float4 Color;        // Fog color\n"
"};\n"
"\n"
"\n"
"/*\n"
" * Constant buffers\n"
" */\n"
"\n"
"cbuffer ConstantBufferLights            : register(b0)\n"
"{\n"
"    SLight Lights[8];    // Light sources\n"
"};\n"
"\n"
"cbuffer ConstantBufferObject            : register(b1)\n"
"{\n"
"    float4x4 WorldMatrix, ViewMatrix, ProjectionMatrix;    // Matrices\n"
"    SMaterial Material;                                    // Material attributes\n"
"};\n"
"\n"
"cbuffer ConstantBufferSurface            : register(b2)\n"
"{\n"
"    int TextureLayers;        // Count of texture layers\n"
"    int3 pad;\n"
"    STexture Textures[8];    // Texture surfaces\n"
"};\n"
"\n"
"cbuffer ConstantBufferDriverSettings    : register(b3)\n"
"{\n"
"    SClipPlane Planes[8];    // Clipping planes\n"
"    SFogStates Fog;            // Fog effect states\n"
"};\n"
"\n"
"\n"
"/*\n"
" * Macros\n"
" */\n"
"\n"
"#define MAPGEN_DISABLE             0\n"
"#define MAPGEN_OBJECT_LINEAR    1\n"
"#define MAPGEN_EYE_LINEAR        2\n"
"#define MAPGEN_SPHERE_MAP        3\n"
"#define MAPGEN_NORMAL_MAP        4\n"
"#define MAPGEN_REFLECTION_MAP    5\n"
"\n"
"#define LIGHT_DIRECTIONAL        0\n"
"#define LIGHT_POINT                1\n"
"#define LIGHT_SPOT                2\n"
"\n"
"#define SHADING_FLAT            0\n"
"#define SHADING_GOURAUD            1\n"
"#define SHADING_PHONG            2\n"
"#define SHADING_PERPIXEL        3\n"
"\n"
"#define TEXENV_MODULATE            0\n"
"#define TEXENV_REPLACE            1\n"
"#define TEXENV_ADD                2\n"
"#define TEXENV_ADDSIGNED        3\n"
"#define TEXENV_SUBTRACT            4\n"
"#define TEXENV_INTERPOLATE        5\n"
"#define TEXENV_DOT3                6\n"
"\n"
"#define CMPSIZE_NEVER            0\n"
"#define CMPSIZE_EQUAL            1\n"
"#define CMPSIZE_NOTEQUAL        2\n"
"#define CMPSIZE_LESS            3\n"
"#define CMPSIZE_LESSEQUAL        4\n"
"#define CMPSIZE_GREATER            5\n"
"#define CMPSIZE_GREATEREQUAL    6\n"
"#define CMPSIZE_ALWAYS            7\n"
"\n"
"#define FOG_STATIC_PALE            0\n"
"#define FOG_STATIC_THICK        1\n"
"#define FOG_VOLUMETRIC            2\n"
"\n"
"\n"
"/*\n"
" * Structures\n"
" */\n"
"\n"
"struct VertexInput\n"
"{\n"
"    float3 Position        : POSITION;\n"
"    float3 Normal        : NORMAL;\n"
"    float4 Color        : COLOR;\n"
"    float2 TexCoord0    : TEXCOORD0;\n"
"    float2 TexCoord1    : TEXCOORD1;\n"
"    float2 TexCoord2    : TEXCOORD2;\n"
"    float2 TexCoord3    : TEXCOORD3;\n"
"};\n"
"\n"
"struct VertexPixelExchange\n"
"{\n"
"    float4 Position        : SV_POSITION;\n"
"    float3 Normal        : NORMAL;\n"
"    float4 Diffuse        : COLOR0;\n"
"    float4 Ambient        : COLOR1;\n"
"    float2 TexCoord0    : TEXCOORD0;\n"
"    float2 TexCoord1    : TEXCOORD1;\n"
"    float2 TexCoord2    : TEXCOORD2;\n"
"    float2 TexCoord3    : TEXCOORD3;\n"
"    \n"
"    float4 WorldPos        : POSITION1;\n"
"    float4 WorldViewPos    : POSITION2;\n"
"};\n"
"\n"
"struct PixelOutput\n"
"{\n"
"    float4 Color : SV_TARGET;\n"
"};\n"
"\n"
"\n"
"/*\n"
" * Inline functions\n"
" */\n"
"\n"
"inline void DiscardPixel()\n"
"{\n"
"    clip(-1.0f);\n"
"}\n"
"\n"
"\n"
"/*\n"
" * Functions\n"
" */\n"
"\n"
"void LightCalculation(int i, float3 Normal, float3 Position, inout float4 ColorOut)\n"
"{\n"
"    float Intensity = 1.0f;\n"
"    \n"
"    // Compute intensity\n"
"    switch (Lights[i].Model)\n"
"    {\n"
"        case LIGHT_DIRECTIONAL:\n"
"            Intensity = saturate(dot(Normal, Lights[i].Position.xyz));\n"
"            break;\n"
"        case LIGHT_POINT:\n"
"            Intensity = saturate(dot(Normal, normalize(Lights[i].Position.xyz - Position)));\n"
"            break;\n"
"        case LIGHT_SPOT:\n"
"            break;\n"
"    }\n"
"    \n"
"    // Compute attenuation\n"
"    if (Lights[i].Model != LIGHT_DIRECTIONAL)\n"
"    {\n"
"        const float Distance = distance(Lights[i].Position.xyz, Position);\n"
"        float Attenuation = 1.0f / ( Lights[i].Attn0 + Lights[i].Attn1*Distance + Lights[i].Attn2*Distance*Distance );\n"
"        Intensity *= Attenuation;\n"
"    }\n"
"    \n"
"    // Apply light color\n"
"    ColorOut += Lights[i].Diffuse * Intensity;\n"
"    //ColorOut += Lights[i].Specular * pow(Intensity, Material.Shininess);\n"
"}\n"
"\n"
"void FogCalculation(float Depth, inout float4 Color)\n"
"{\n"
"    float Factor = 0.0f;\n"
"    \n"
"    switch (Fog.Mode)\n"
"    {\n"
"        case FOG_STATIC_PALE:\n"
"            Factor = exp(-Fog.Density * Depth); break;\n"
"        case FOG_STATIC_THICK:\n"
"        {\n"
"            float TempFactor = Fog.Density * Depth;\n"
"            Factor = exp(-TempFactor * TempFactor);\n"
"        }\n"
"        break;\n"
"    }\n"
"    \n"
"    clamp(Factor, 0.0f, 1.0f);\n"
"    \n"
"    Color.xyz = Fog.Color.xyz * (1.0f - Factor) + Color.xyz * Factor;\n"
"}\n"
"\n"
"bool ClippingPlane(int i, float4 Position)\n"
"{\n"
"    return dot(Position.xyz, normalize(Planes[i].Normal)) + Planes[i].Distance < 0.0f;\n"
"}\n"
"\n"
"void TexCoordGeneration(int MapGenType, float Pos, float WorldViewPos, float TransNormal, float TexCoordIn, inout float TexCoordOut)\n"
"{\n"
"    // Texture coordinate generation\n"
"    switch (MapGenType)\n"
"    {\n"
"        case MAPGEN_DISABLE:\n"
"            TexCoordOut = TexCoordIn; break;\n"
"        case MAPGEN_OBJECT_LINEAR:\n"
"            TexCoordOut = Pos; break;\n"
"        case MAPGEN_EYE_LINEAR:\n"
"            TexCoordOut = WorldViewPos; break;\n"
"        case MAPGEN_SPHERE_MAP:\n"
"            TexCoordOut = TransNormal*0.5f + 0.5f; break;\n"
"        case MAPGEN_NORMAL_MAP:\n"
"            break; //!TODO!\n"
"        case MAPGEN_REFLECTION_MAP:\n"
"            break; //!TODO!\n"
"    }\n"
"}\n"
"\n"
"void TextureMapping(int i, Texture2D Tex, SamplerState Sampler, float2 TexCoord, inout float4 ColorOut)\n"
"{\n"
"    const float4 TexColor = Tex.Sample(Sampler, TexCoord);\n"
"    \n"
"    switch (Textures[i].EnvType)\n"
"    {\n"
"        case TEXENV_MODULATE:\n"
"            ColorOut *= TexColor; break;\n"
"        case TEXENV_REPLACE:\n"
"            ColorOut = TexColor; break;\n"
"        case TEXENV_ADD:\n"
"            ColorOut += TexColor; break;\n"
"        case TEXENV_ADDSIGNED:\n"
"            ColorOut += TexColor - float4(0.5f, 0.5f, 0.5f, 1.0f); break;\n"
"        case TEXENV_SUBTRACT:\n"
"            ColorOut -= TexColor; break;\n"
"        case TEXENV_INTERPOLATE:\n"
"            break; //!TODO!\n"
"        case TEXENV_DOT3:\n"
"            ColorOut.r = (ColorOut.r - 0.5f)*(TexColor.r - 0.5f);\n"
"            ColorOut.g = (ColorOut.g - 0.5f)*(TexColor.g - 0.5f);\n"
"            ColorOut.b = (ColorOut.b - 0.5f)*(TexColor.b - 0.5f);\n"
"            break;\n"
"    }\n"
"}\n"
"\n"
"\n"
"/*\n"
" * ======= Vertex Shader =======\n"
" */\n"
"\n"
"VertexPixelExchange VertexMain(VertexInput Input)\n"
"{\n"
"    #define mcrTexCoordGeneration(t, c)                                                                                                    \\\n"
"        TexCoordGeneration(Textures[t].MapGenType.x, Input.Position.x, Output.WorldViewPos.x, TransNormal.x, Input.c.x, Output.c.x);    \\\n"
"        TexCoordGeneration(Textures[t].MapGenType.y, Input.Position.y, Output.WorldViewPos.y, TransNormal.y, Input.c.y, Output.c.y);\n"
"    #define mcrTexCoordTransform(i, t)                                                                \\\n"
"        Output.t = mul(float4(Output.t.x, Output.t.y, 0.0f, 1.0f), Textures[i].Matrix).xy;\n"
"    \n"
"    // Temporary variables\n"
"    VertexPixelExchange Output = (VertexPixelExchange)0;\n"
"    \n"
"    // Compute vertex positions (local, gloabl, projected)\n"
"    Output.WorldPos        = mul(float4(Input.Position, 1.0), WorldMatrix);\n"
"    Output.WorldViewPos    = mul(Output.WorldPos, ViewMatrix);\n"
"    Output.Position        = mul(Output.WorldViewPos, ProjectionMatrix);\n"
"    \n"
"    // Compute normals\n"
"    float3 TransNormal    = mul(Input.Normal, WorldMatrix);\n"
"    TransNormal            = mul(TransNormal, ViewMatrix);\n"
"    TransNormal            = normalize(TransNormal);\n"
"    Output.Normal        = TransNormal;\n"
"    \n"
"    // Compute final vertex color\n"
"    Output.Diffuse = Input.Color * Material.Diffuse;\n"
"    Output.Ambient = Material.Ambient;\n"
"    \n"
"    // Light computations\n"
"    if (Material.LightingEnabled && Material.Shading < SHADING_PHONG)\n"
"    {\n"
"        float4 LightColor = Output.Ambient;\n"
"        \n"
"        for (int i = 0; i < 8; ++i)\n"
"        {\n"
"            if (Lights[i].Enabled)\n"
"                LightCalculation(i, Output.Normal, Output.WorldViewPos.xyz, LightColor);\n"
"        }\n"
"        \n"
"        Output.Diffuse.rgb *= LightColor.rgb;\n"
"        Output.Diffuse = saturate(Output.Diffuse);\n"
"    }\n"
"    \n"
"    // Compute texture coordinates\n"
"    if (TextureLayers > 0)\n"
"    {\n"
"        mcrTexCoordGeneration(0, TexCoord0);\n"
"        mcrTexCoordTransform(0, TexCoord0);\n"
"        \n"
"        if (TextureLayers > 1)\n"
"        {\n"
"            mcrTexCoordGeneration(1, TexCoord1);\n"
"            mcrTexCoordTransform(1, TexCoord1);\n"
"            \n"
"            if (TextureLayers > 2)\n"
"            {\n"
"                mcrTexCoordGeneration(2, TexCoord2);\n"
"                mcrTexCoordTransform(2, TexCoord2);\n"
"                \n"
"                if (TextureLayers > 3)\n"
"                {\n"
"                    mcrTexCoordGeneration(3, TexCoord3);\n"
"                    mcrTexCoordTransform(3, TexCoord3);\n"
"                } // fi tex-layer 3\n"
"            } // fi tex-layer 2\n"
"        } // fi tex-layer 1\n"
"    } // fi tex-layer 0\n"
"    \n"
"    return Output;\n"
"    \n"
"    #undef mcrTexCoordGeneration\n"
"    #undef mcrTexCoordTransform\n"
"}\n"
"\n"
"\n"
"/*\n"
" * ======= Pixel Shader =======\n"
" */\n"
"\n"
"PixelOutput PixelMain(VertexPixelExchange Input)\n"
"{\n"
"    // Temporary variables\n"
"    PixelOutput Output = (PixelOutput)0;\n"
"    \n"
"    float4 TexColor        = 1.0f;\n"
"    float4 LightColor    = 1.0f;\n"
"    \n"
"    // Light computations\n"
"    if (Material.LightingEnabled && Material.Shading >= SHADING_PHONG)\n"
"    {\n"
"        LightColor = Input.Ambient;\n"
"        const float3 Normal = normalize(Input.Normal);\n"
"        \n"
"        for (int i = 0; i < 8; ++i)\n"
"        {\n"
"            if (Lights[i].Enabled)\n"
"                LightCalculation(i, Normal, Input.WorldViewPos.xyz, LightColor);\n"
"        }\n"
"    }\n"
"    \n"
"    // Texture mapping\n"
"    if (TextureLayers > 0)\n"
"    {\n"
"        TextureMapping(0, Texture2D0, SamplerLinear0, Input.TexCoord0, TexColor);\n"
"        if (TextureLayers > 1)\n"
"        {\n"
"            TextureMapping(1, Texture2D1, SamplerLinear1, Input.TexCoord1, TexColor);\n"
"            if (TextureLayers > 2)\n"
"            {\n"
"                TextureMapping(2, Texture2D2, SamplerLinear2, Input.TexCoord2, TexColor);\n"
"                if (TextureLayers > 3)\n"
"                {\n"
"                    TextureMapping(3, Texture2D3, SamplerLinear3, Input.TexCoord3, TexColor);\n"
"                } // fi tex-layer 3\n"
"            } // fi tex-layer 2\n"
"        } // fi tex-layer 1\n"
"    } // fi tex-layer 0\n"
"    \n"
"    // Final color output\n"
"    Output.Color = Input.Diffuse * TexColor * float4(LightColor.rgb, 1.0);\n"
"    \n"
"    // Fog computations\n"
"    if (Material.FogEnabled)\n"
"        FogCalculation(Input.WorldViewPos.z, Output.Color);\n"
"    \n"
"    // Alpha reference method\n"
"    switch (Material.AlphaMethod)\n"
"    {\n"
"        case CMPSIZE_ALWAYS:\n"
"            break;\n"
"        case CMPSIZE_GREATEREQUAL:\n"
"            if (!(Output.Color.a >= Material.AlphaReference))\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_GREATER:\n"
"            if (!(Output.Color.a > Material.AlphaReference))\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_LESSEQUAL:\n"
"            if (!(Output.Color.a <= Material.AlphaReference))\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_LESS:\n"
"            if (!(Output.Color.a < Material.AlphaReference))\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_NOTEQUAL:\n"
"            if (Output.Color.a == Material.AlphaReference)\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_EQUAL:\n"
"            if (Output.Color.a != Material.AlphaReference)\n"
"                DiscardPixel();\n"
"            break;\n"
"        case CMPSIZE_NEVER:\n"
"            DiscardPixel(); break;\n"
"    }\n"
"    \n"
"    // Process clipping planes\n"
"    for (int i = 0; i < 8; ++i)\n"
"    {\n"
"        if (Planes[i].Enabled && ClippingPlane(i, Input.WorldPos))\n"
"        {\n"
"            DiscardPixel();\n"
"            return Output;\n"
"        }\n"
"    }\n"
"    \n"
"    // Return the output color\n"
"    return Output;\n"
"}\n"
"\n"

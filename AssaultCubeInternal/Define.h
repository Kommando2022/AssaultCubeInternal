#include "Struct.h"

typedef UINT32 GlEnum;
typedef UINT32 GlInt;

typedef void (__cdecl* TypeSetCursor)(UINT32* Cursor);
typedef UINT32*(__cdecl* TypeCreateCursor)(UINT8* Data, UINT8* Mask, int Width, int Height, int HotX, int HotY);
typedef int(__cdecl* TypeShowCursor)(int Toggle);
typedef BOOL(__stdcall* TypeGlSwapBuffers)(HDC HardwareDevice);
typedef void(__stdcall* TypeGlDepthFunc)(GlEnum GlFunc);
typedef void(__stdcall* TypeGlDisableClientState)(GlEnum Cap);
typedef void(__stdcall* TypeGlBindTexture)(GlEnum Target, GlInt Texture);
typedef void(__stdcall* TypeGlBlendFunc)(GlEnum SFactor, GlEnum DFactor);
typedef void(__stdcall* TypeGlEnable)(GlEnum GlFunc);
typedef void(__stdcall* TypeGlDisable)(GlEnum GlFunc);
typedef void(__stdcall* TypeGlColor4f)(float R , float G ,float B ,float A);

TypeSetCursor SDLSetCursor = NULL;
TypeCreateCursor SDLCreateCursor = NULL;
TypeShowCursor SDLShowCursor = NULL;
TypeGlSwapBuffers GlSwapBuffers = NULL;
TypeGlDisableClientState GlDisableClientState = NULL;
TypeGlDepthFunc GlDepthFunc = NULL;
TypeGlBindTexture GlBindTexture = NULL;
TypeGlEnable GlEnable = NULL;
TypeGlDisable GlDisable = NULL;
TypeGlColor4f GlColor4f = NULL;
TypeGlBlendFunc GlBlendFunc = NULL;

bool ShowMenu = false;
bool Test = NULL;
const char* CurrentMeshName = NULL;
Vector3 RainbowColor;
bool Once = false;

HMODULE SDLHandle = NULL;
HMODULE OpenGlHandle = NULL;
UINT64 ReturnAddress = 0x41456F;
UINT32 IndicieCount = NULL;
UINT32 This = NULL;

/* ClearBufferMask */
#define Gl_DEPTH_BUFFER_BIT               0x00000100
#define Gl_STENCIL_BUFFER_BIT             0x00000400
#define Gl_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define Gl_FALSE                          0
#define Gl_TRUE                           1

/* BeginMode */
#define Gl_POINTS                         0x0000
#define Gl_LINES                          0x0001
#define Gl_LINE_LOOP                      0x0002
#define Gl_LINE_STRIP                     0x0003
#define Gl_TRIANGlES                      0x0004
#define Gl_TRIANGlE_STRIP                 0x0005
#define Gl_TRIANGlE_FAN                   0x0006

/* AlphaFunction */
#define Gl_NEVER                          0x0200
#define Gl_LESS                           0x0201
#define Gl_EQUAL                          0x0202
#define Gl_LEQUAL                         0x0203
#define Gl_GREATER                        0x0204
#define Gl_NOTEQUAL                       0x0205
#define Gl_GEQUAL                         0x0206
#define Gl_ALWAYS                         0x0207

/* BlendingFactorDest */
#define Gl_ZERO                           0
#define Gl_ONE                            1
#define Gl_SRC_COLOR                      0x0300
#define Gl_ONE_MINUS_SRC_COLOR            0x0301
#define Gl_SRC_ALPHA                      0x0302
#define Gl_ONE_MINUS_SRC_ALPHA            0x0303
#define Gl_DST_ALPHA                      0x0304
#define Gl_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      Gl_ZERO */
/*      Gl_ONE */
#define Gl_DST_COLOR                      0x0306
#define Gl_ONE_MINUS_DST_COLOR            0x0307
#define Gl_SRC_ALPHA_SATURATE             0x0308
/*      Gl_SRC_ALPHA */
/*      Gl_ONE_MINUS_SRC_ALPHA */
/*      Gl_DST_ALPHA */
/*      Gl_ONE_MINUS_DST_ALPHA */

/* ColorMaterialFace */
/*      Gl_FRONT_AND_BACK */

/* ColorMaterialParameter */
/*      Gl_AMBIENT_AND_DIFFUSE */

/* ColorPointerType */
/*      Gl_UNSIGNED_BYTE */
/*      Gl_FLOAT */
/*      Gl_FIXED */

/* CullFaceMode */
#define Gl_FRONT                          0x0404
#define Gl_BACK                           0x0405
#define Gl_FRONT_AND_BACK                 0x0408

/* DepthFunction */
/*      Gl_NEVER */
/*      Gl_LESS */
/*      Gl_EQUAL */
/*      Gl_LEQUAL */
/*      Gl_GREATER */
/*      Gl_NOTEQUAL */
/*      Gl_GEQUAL */
/*      Gl_ALWAYS */

/* EnableCap */
#define Gl_FOG                            0x0B60
#define Gl_LIGHTING                       0x0B50
#define Gl_TEXTURE_2D                     0x0DE1
#define Gl_CULL_FACE                      0x0B44
#define Gl_ALPHA_TEST                     0x0BC0
#define Gl_BLEND                          0x0BE2
#define Gl_COLOR_LOGIC_OP                 0x0BF2
#define Gl_DITHER                         0x0BD0
#define Gl_STENCIL_TEST                   0x0B90
#define Gl_DEPTH_TEST                     0x0B71
/*      Gl_LIGHT0 */
/*      Gl_LIGHT1 */
/*      Gl_LIGHT2 */
/*      Gl_LIGHT3 */
/*      Gl_LIGHT4 */
/*      Gl_LIGHT5 */
/*      Gl_LIGHT6 */
/*      Gl_LIGHT7 */
#define Gl_POINT_SMOOTH                   0x0B10
#define Gl_LINE_SMOOTH                    0x0B20
#define Gl_SCISSOR_TEST                   0x0C11
#define Gl_COLOR_MATERIAL                 0x0B57
#define Gl_NORMALIZE                      0x0BA1
#define Gl_RESCALE_NORMAL                 0x803A
#define Gl_POLYGON_OFFSET_FILL            0x8037
#define Gl_VERTEX_ARRAY                   0x8074
#define Gl_NORMAL_ARRAY                   0x8075
#define Gl_COLOR_ARRAY                    0x8076
#define Gl_TEXTURE_COORD_ARRAY            0x8078
#define Gl_MULTISAMPLE                    0x809D
#define Gl_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define Gl_SAMPLE_ALPHA_TO_ONE            0x809F
#define Gl_SAMPLE_COVERAGE                0x80A0

/* ErrorCode */
#define Gl_NO_ERROR                       0
#define Gl_INVALID_ENUM                   0x0500
#define Gl_INVALID_VALUE                  0x0501
#define Gl_INVALID_OPERATION              0x0502
#define Gl_STACK_OVERFLOW                 0x0503
#define Gl_STACK_UNDERFLOW                0x0504
#define Gl_OUT_OF_MEMORY                  0x0505

/* FogMode */
/*      Gl_LINEAR */
#define Gl_EXP                            0x0800
#define Gl_EXP2                           0x0801

/* FogParameter */
#define Gl_FOG_DENSITY                    0x0B62
#define Gl_FOG_START                      0x0B63
#define Gl_FOG_END                        0x0B64
#define Gl_FOG_MODE                       0x0B65
#define Gl_FOG_COLOR                      0x0B66

/* FrontFaceDirection */
#define Gl_CW                             0x0900
#define Gl_CCW                            0x0901

/* GetPName */
#define Gl_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define Gl_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define Gl_ALIASED_POINT_SIZE_RANGE       0x846D
#define Gl_ALIASED_LINE_WIDTH_RANGE       0x846E
#define Gl_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#define Gl_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B
#define Gl_MAX_LIGHTS                     0x0D31
#define Gl_MAX_TEXTURE_SIZE               0x0D33
#define Gl_MAX_MODELVIEW_STACK_DEPTH      0x0D36
#define Gl_MAX_PROJECTION_STACK_DEPTH     0x0D38
#define Gl_MAX_TEXTURE_STACK_DEPTH        0x0D39
#define Gl_MAX_VIEWPORT_DIMS              0x0D3A
#define Gl_MAX_ELEMENTS_VERTICES          0x80E8
#define Gl_MAX_ELEMENTS_INDICES           0x80E9
#define Gl_MAX_TEXTURE_UNITS              0x84E2
#define Gl_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define Gl_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define Gl_SUBPIXEL_BITS                  0x0D50
#define Gl_RED_BITS                       0x0D52
#define Gl_GREEN_BITS                     0x0D53
#define Gl_BLUE_BITS                      0x0D54
#define Gl_ALPHA_BITS                     0x0D55
#define Gl_DEPTH_BITS                     0x0D56
#define Gl_STENCIL_BITS                   0x0D57

/* HintMode */
#define Gl_DONT_CARE                      0x1100
#define Gl_FASTEST                        0x1101
#define Gl_NICEST                         0x1102

/* HintTarget */
#define Gl_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define Gl_POINT_SMOOTH_HINT              0x0C51
#define Gl_LINE_SMOOTH_HINT               0x0C52
#define Gl_POLYGON_SMOOTH_HINT            0x0C53
#define Gl_FOG_HINT                       0x0C54

/* LightModelParameter */
#define Gl_LIGHT_MODEL_AMBIENT            0x0B53
#define Gl_LIGHT_MODEL_TWO_SIDE           0x0B52

/* LightParameter */
#define Gl_AMBIENT                        0x1200
#define Gl_DIFFUSE                        0x1201
#define Gl_SPECULAR                       0x1202
#define Gl_POSITION                       0x1203
#define Gl_SPOT_DIRECTION                 0x1204
#define Gl_SPOT_EXPONENT                  0x1205
#define Gl_SPOT_CUTOFF                    0x1206
#define Gl_CONSTANT_ATTENUATION           0x1207
#define Gl_LINEAR_ATTENUATION             0x1208
#define Gl_QUADRATIC_ATTENUATION          0x1209

/* DataType */
#define Gl_BYTE                           0x1400
#define Gl_UNSIGNED_BYTE                  0x1401
#define Gl_SHORT                          0x1402
#define Gl_UNSIGNED_SHORT                 0x1403
#define Gl_FLOAT                          0x1406
#define Gl_FIXED                          0x140C

/* LogicOp */
#define Gl_CLEAR                          0x1500
#define Gl_AND                            0x1501
#define Gl_AND_REVERSE                    0x1502
#define Gl_COPY                           0x1503
#define Gl_AND_INVERTED                   0x1504
#define Gl_NOOP                           0x1505
#define Gl_XOR                            0x1506
#define Gl_OR                             0x1507
#define Gl_NOR                            0x1508
#define Gl_EQUIV                          0x1509
#define Gl_INVERT                         0x150A
#define Gl_OR_REVERSE                     0x150B
#define Gl_COPY_INVERTED                  0x150C
#define Gl_OR_INVERTED                    0x150D
#define Gl_NAND                           0x150E
#define Gl_SET                            0x150F

/* MaterialFace */
/*      Gl_FRONT_AND_BACK */

/* MaterialParameter */
#define Gl_EMISSION                       0x1600
#define Gl_SHININESS                      0x1601
#define Gl_AMBIENT_AND_DIFFUSE            0x1602
/*      Gl_AMBIENT */
/*      Gl_DIFFUSE */
/*      Gl_SPECULAR */

/* MatrixMode */
#define Gl_MODELVIEW                      0x1700
#define Gl_PROJECTION                     0x1701
#define Gl_TEXTURE                        0x1702

/* NormalPointerType */
/*      Gl_BYTE */
/*      Gl_SHORT */
/*      Gl_FLOAT */
/*      Gl_FIXED */

/* PixelFormat */
#define Gl_ALPHA                          0x1906
#define Gl_RGB                            0x1907
#define Gl_RGBA                           0x1908
#define Gl_LUMINANCE                      0x1909
#define Gl_LUMINANCE_ALPHA                0x190A

/* PixelStoreParameter */
#define Gl_UNPACK_ALIGNMENT               0x0CF5
#define Gl_PACK_ALIGNMENT                 0x0D05

/* PixelType */
/*      Gl_UNSIGNED_BYTE */
#define Gl_UNSIGNED_SHORT_4_4_4_4         0x8033
#define Gl_UNSIGNED_SHORT_5_5_5_1         0x8034
#define Gl_UNSIGNED_SHORT_5_6_5           0x8363

/* ShadingModel */
#define Gl_FLAT                           0x1D00
#define Gl_SMOOTH                         0x1D01

/* StencilFunction */
/*      Gl_NEVER */
/*      Gl_LESS */
/*      Gl_EQUAL */
/*      Gl_LEQUAL */
/*      Gl_GREATER */
/*      Gl_NOTEQUAL */
/*      Gl_GEQUAL */
/*      Gl_ALWAYS */

/* StencilOp */
/*      Gl_ZERO */
#define Gl_KEEP                           0x1E00
#define Gl_REPLACE                        0x1E01
#define Gl_INCR                           0x1E02
#define Gl_DECR                           0x1E03
/*      Gl_INVERT */

/* StringName */
#define Gl_VENDOR                         0x1F00
#define Gl_RENDERER                       0x1F01
#define Gl_VERSION                        0x1F02
#define Gl_EXTENSIONS                     0x1F03

/* TexCoordPointerType */
/*      Gl_SHORT */
/*      Gl_FLOAT */
/*      Gl_FIXED */
/*      Gl_BYTE */

/* TextureEnvMode */
#define Gl_MODULATE                       0x2100
#define Gl_DECAL                          0x2101
/*      Gl_BLEND */
#define Gl_ADD                            0x0104
/*      Gl_REPLACE */

/* TextureEnvParameter */
#define Gl_TEXTURE_ENV_MODE               0x2200
#define Gl_TEXTURE_ENV_COLOR              0x2201

/* TextureEnvTarget */
#define Gl_TEXTURE_ENV                    0x2300

/* TextureMagFilter */
#define Gl_NEAREST                        0x2600
#define Gl_LINEAR                         0x2601

/* TextureMinFilter */
/*      Gl_NEAREST */
/*      Gl_LINEAR */
#define Gl_NEAREST_MIPMAP_NEAREST         0x2700
#define Gl_LINEAR_MIPMAP_NEAREST          0x2701
#define Gl_NEAREST_MIPMAP_LINEAR          0x2702
#define Gl_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define Gl_TEXTURE_MAG_FILTER             0x2800
#define Gl_TEXTURE_MIN_FILTER             0x2801
#define Gl_TEXTURE_WRAP_S                 0x2802
#define Gl_TEXTURE_WRAP_T                 0x2803

/* TextureTarget */
/*      Gl_TEXTURE_2D */

/* TextureUnit */
#define Gl_TEXTURE0                       0x84C0
#define Gl_TEXTURE1                       0x84C1
#define Gl_TEXTURE2                       0x84C2
#define Gl_TEXTURE3                       0x84C3
#define Gl_TEXTURE4                       0x84C4
#define Gl_TEXTURE5                       0x84C5
#define Gl_TEXTURE6                       0x84C6
#define Gl_TEXTURE7                       0x84C7
#define Gl_TEXTURE8                       0x84C8
#define Gl_TEXTURE9                       0x84C9
#define Gl_TEXTURE10                      0x84CA
#define Gl_TEXTURE11                      0x84CB
#define Gl_TEXTURE12                      0x84CC
#define Gl_TEXTURE13                      0x84CD
#define Gl_TEXTURE14                      0x84CE
#define Gl_TEXTURE15                      0x84CF
#define Gl_TEXTURE16                      0x84D0
#define Gl_TEXTURE17                      0x84D1
#define Gl_TEXTURE18                      0x84D2
#define Gl_TEXTURE19                      0x84D3
#define Gl_TEXTURE20                      0x84D4
#define Gl_TEXTURE21                      0x84D5
#define Gl_TEXTURE22                      0x84D6
#define Gl_TEXTURE23                      0x84D7
#define Gl_TEXTURE24                      0x84D8
#define Gl_TEXTURE25                      0x84D9
#define Gl_TEXTURE26                      0x84DA
#define Gl_TEXTURE27                      0x84DB
#define Gl_TEXTURE28                      0x84DC
#define Gl_TEXTURE29                      0x84DD
#define Gl_TEXTURE30                      0x84DE
#define Gl_TEXTURE31                      0x84DF

/* TextureWrapMode */
#define Gl_REPEAT                         0x2901
#define Gl_CLAMP_TO_EDGE                  0x812F

/* PixelInternalFormat */
#define Gl_PALETTE4_RGB8_OES              0x8B90
#define Gl_PALETTE4_RGBA8_OES             0x8B91
#define Gl_PALETTE4_R5_G6_B5_OES          0x8B92
#define Gl_PALETTE4_RGBA4_OES             0x8B93
#define Gl_PALETTE4_RGB5_A1_OES           0x8B94
#define Gl_PALETTE8_RGB8_OES              0x8B95
#define Gl_PALETTE8_RGBA8_OES             0x8B96
#define Gl_PALETTE8_R5_G6_B5_OES          0x8B97
#define Gl_PALETTE8_RGBA4_OES             0x8B98
#define Gl_PALETTE8_RGB5_A1_OES           0x8B99

/* VertexPointerType */
/*      Gl_SHORT */
/*      Gl_FLOAT */
/*      Gl_FIXED */
/*      Gl_BYTE */

/* LightName */
#define Gl_LIGHT0                         0x4000
#define Gl_LIGHT1                         0x4001
#define Gl_LIGHT2                         0x4002
#define Gl_LIGHT3                         0x4003
#define Gl_LIGHT4                         0x4004
#define Gl_LIGHT5                         0x4005
#define Gl_LIGHT6                         0x4006
#define Gl_LIGHT7                         0x4007
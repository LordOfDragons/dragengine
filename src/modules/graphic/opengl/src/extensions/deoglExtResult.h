/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLEXTRESULT_H_
#define _DEOGLEXTRESULT_H_

/**
 * OpenGL Extensions Result.
 *
 * Header file containing the result of the opengl extensions. This
 * file has to be included in any source file requiring opengl
 * related functions or tokens. Once initialized all functions which
 * are valid contain the driver provided function while all others
 * are set to NULL.
 *
 * The definitions used here are taken from the header files provided
 * on http://www.opengl.org/registry/ .
 */

// it is often the case that the glext header is already included by the gl header above.
// this is bad for us since these gl headers usually do not have all the extensions that
// the module can support. for this we need our own glext header which is the most
// recent containing all the definitions we might need. due to the automatic inclusion
// of a lesser glext header though we need to use a trick. the include-once guard has to
// be disabled first before we can include our own glext header. this way the glext
// header thinks nothing has been run beforehand and adds all the missing definitions.
// this works since all definitions in the glext header are guarded by #ifndef clauses.
// the glext header provided by the host system only defines and locks all the
// definitions it knows about. all those that the glext header does not know about are
// free to be added by our own glext header. already defined ones are not required to
// be redefined which is the correct behavior we are looking for.

#ifdef OS_ANDROID
	#undef __glext_h_
	#undef __gl2ext_h_
	#undef __gl3ext_h_
	#undef EGL_EGLEXT_VERSION
	#include "egl32.h"
	#include "egl2ext.h"
	#include "androidfix.h"
	#define GLAPI GL_API
	
#else
	#undef __glext_h_
	#undef GL_GLEXT_VERSION
	#include "glext.h"
#endif

// Regular expression to convert opengl function definition to local pointer function.
// Search: GLAPI.+(gl[A-Za-z0-9_]+).+;
// Replace: extern GLAPI PFN\U\1\EPROC p\1;

#ifdef OS_W32
	#ifdef APIENTRYP
		#undef APIENTRYP
	#endif
	#define APIENTRYP *
	
	#ifdef GLAPI
		#undef GLAPI
	#endif
	#define GLAPI
#endif

#ifdef OS_MACOS
	#ifdef GLAPI
		#undef GLAPI
	#endif
	#define GLAPI
#endif



// Define tokens without prefix where there exists no version without prefix yet
//////////////////////////////////////////////////////////////////////////////////

#define GL_INTENSITY16F                   0x881D
#define GL_LUMINANCE16F                   0x881E
#define GL_LUMINANCE_ALPHA16F             0x881F
#define GL_INTENSITY32F                   0x8817
#define GL_LUMINANCE32F                   0x8818
#define GL_LUMINANCE_ALPHA32F             0x8819

#define GL_COMPRESSED_RGB_S3TC_DXT1       0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1      0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3      0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5      0x83F3
#define GL_COMPRESSED_SRGB_S3TC_DXT1      0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5 0x8C4F
#define GL_COMPRESSED_LUMINANCE_LATC1     0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 0x8C73
#define GL_COMPRESSED_RED_GREEN_RGTC2     0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 0x8DBE

#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS 0x8CDA

// #define GL_OBJECT_COMPILE_STATUS          0x8B81
// #define GL_OBJECT_LINK_STATUS             0x8B82
// #define GL_OBJECT_INFO_LOG_LENGTH         0x8B84

#define GL_MAX_GEOMETRY_VARYING_COMPONENTS 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS  0x8DDE



// Required Extensions
////////////////////////

// GL_EXT_polygon_offset
//////////////////////////

extern GLAPI PFNGLPOLYGONOFFSETEXTPROC pglPolygonOffset;



// GL_EXT_texture3D : opengl version 1.2
//////////////////////////////////////////

extern GLAPI PFNGLTEXIMAGE3DPROC pglTexImage3D;
extern GLAPI PFNGLTEXSUBIMAGE3DPROC pglTexSubImage3D;



// GL_EXT_blend_color : opengl version 1.2
////////////////////////////////////////////

extern GLAPI PFNGLBLENDCOLOREXTPROC pglBlendColor;



// GL_EXT_blend_minmax : opengl version 1.2
/////////////////////////////////////////////

extern GLAPI PFNGLBLENDEQUATIONEXTPROC pglBlendEquation;



// GL_ARB_multitexture . opengl version 1.3
/////////////////////////////////////////////

extern GLAPI PFNGLACTIVETEXTUREPROC pglActiveTexture;
// extern GLAPI PFNGLCLIENTACTIVETEXTUREPROC pglClientActiveTexture;
// extern GLAPI PFNGLMULTITEXCOORD1DPROC pglMultiTexCoord1d;
// extern GLAPI PFNGLMULTITEXCOORD1DVPROC pglMultiTexCoord1dv;
// extern GLAPI PFNGLMULTITEXCOORD1FPROC pglMultiTexCoord1f;
// extern GLAPI PFNGLMULTITEXCOORD1FVPROC pglMultiTexCoord1fv;
// extern GLAPI PFNGLMULTITEXCOORD1IPROC pglMultiTexCoord1i;
// extern GLAPI PFNGLMULTITEXCOORD1IVPROC pglMultiTexCoord1iv;
// extern GLAPI PFNGLMULTITEXCOORD1SPROC pglMultiTexCoord1s;
// extern GLAPI PFNGLMULTITEXCOORD1SVPROC pglMultiTexCoord1sv;
// extern GLAPI PFNGLMULTITEXCOORD2DPROC pglMultiTexCoord2d;
// extern GLAPI PFNGLMULTITEXCOORD2DVPROC pglMultiTexCoord2dv;
// extern GLAPI PFNGLMULTITEXCOORD2FPROC pglMultiTexCoord2f;
// extern GLAPI PFNGLMULTITEXCOORD2FVPROC pglMultiTexCoord2fv;
// extern GLAPI PFNGLMULTITEXCOORD2IPROC pglMultiTexCoord2i;
// extern GLAPI PFNGLMULTITEXCOORD2IVPROC pglMultiTexCoord2iv;
// extern GLAPI PFNGLMULTITEXCOORD2SPROC pglMultiTexCoord2s;
// extern GLAPI PFNGLMULTITEXCOORD2SVPROC pglMultiTexCoord2sv;
// extern GLAPI PFNGLMULTITEXCOORD3DPROC pglMultiTexCoord3d;
// extern GLAPI PFNGLMULTITEXCOORD3DVPROC pglMultiTexCoord3dv;
// extern GLAPI PFNGLMULTITEXCOORD3FPROC pglMultiTexCoord3f;
// extern GLAPI PFNGLMULTITEXCOORD3FVPROC pglMultiTexCoord3fv;
// extern GLAPI PFNGLMULTITEXCOORD3IPROC pglMultiTexCoord3i;
// extern GLAPI PFNGLMULTITEXCOORD3IVPROC pglMultiTexCoord3iv;
// extern GLAPI PFNGLMULTITEXCOORD3SPROC pglMultiTexCoord3s;
// extern GLAPI PFNGLMULTITEXCOORD3SVPROC pglMultiTexCoord3sv;
// extern GLAPI PFNGLMULTITEXCOORD4DPROC pglMultiTexCoord4d;
// extern GLAPI PFNGLMULTITEXCOORD4DVPROC pglMultiTexCoord4dv;
// extern GLAPI PFNGLMULTITEXCOORD4FPROC pglMultiTexCoord4f;
// extern GLAPI PFNGLMULTITEXCOORD4FVPROC pglMultiTexCoord4fv;
// extern GLAPI PFNGLMULTITEXCOORD4IPROC pglMultiTexCoord4i;
// extern GLAPI PFNGLMULTITEXCOORD4IVPROC pglMultiTexCoord4iv;
// extern GLAPI PFNGLMULTITEXCOORD4SPROC pglMultiTexCoord4s;
// extern GLAPI PFNGLMULTITEXCOORD4SVPROC pglMultiTexCoord4sv;



// GL_ARB_texture_compression : opengl version 1.3
////////////////////////////////////////////////////

extern GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC pglCompressedTexImage3D;
extern GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC pglCompressedTexImage2D;
// extern GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC pglCompressedTexImage1D;
// extern GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC pglCompressedTexSubImage3D;
// extern GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC pglCompressedTexSubImage2D;
// extern GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC pglCompressedTexSubImage1D;
// extern GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC pglGetCompressedTexImage;



// GL_ARB_vertex_buffer_object . opengl version 1.5
/////////////////////////////////////////////////////

extern GLAPI PFNGLBINDBUFFERPROC pglBindBuffer;
extern GLAPI PFNGLDELETEBUFFERSPROC pglDeleteBuffers;
extern GLAPI PFNGLGENBUFFERSPROC pglGenBuffers;
// extern GLAPI PFNGLISBUFFERPROC pglIsBuffer;
extern GLAPI PFNGLBUFFERDATAPROC pglBufferData;
extern GLAPI PFNGLBUFFERSUBDATAPROC pglBufferSubData;
// extern GLAPI PFNGLGETBUFFERSUBDATAPROC pglGetBufferSubData;
extern GLAPI PFNGLMAPBUFFERPROC pglMapBuffer;
extern GLAPI PFNGLMAPBUFFERRANGEPROC pglMapBufferRange;
extern GLAPI PFNGLUNMAPBUFFERPROC pglUnmapBuffer;
// extern GLAPI PFNGLGETBUFFERPARAMETERIVPROC pglGetBufferParameteriv;
// extern GLAPI PFNGLGETBUFFERPOINTERVPROC pglGetBufferPointerv;



// GL_ARB_occlusion_query . opengl version 1.5
////////////////////////////////////////////////

extern GLAPI PFNGLGENQUERIESPROC pglGenQueries;
extern GLAPI PFNGLDELETEQUERIESPROC pglDeleteQueries;
// extern GLAPI PFNGLISQUERYPROC pglIsQuery;
extern GLAPI PFNGLBEGINQUERYPROC pglBeginQuery;
extern GLAPI PFNGLENDQUERYPROC pglEndQuery;
// extern GLAPI PFNGLGETQUERYIVPROC pglGetQueryiv;
// extern GLAPI PFNGLGETQUERYOBJECTIVPROC pglGetQueryObjectiv;
extern GLAPI PFNGLGETQUERYOBJECTUIVPROC pglGetQueryObjectuiv;



// opengl version 2.0
///////////////////////

extern PFNGLSTENCILOPSEPARATEPROC pglStencilOpSeparate;
// extern PFNGLSTENCILFUNCSEPARATEPROC pglStencilFuncSeparate;
// extern PFNGLSTENCILMASKSEPARATEPROC pglStencilMaskSeparate;



// GL_ARB_vertex_program . opengl version 2.0
///////////////////////////////////////////////

// extern GLAPI PFNGLVERTEXATTRIB1DPROC pglVertexAttrib1d;
// extern GLAPI PFNGLVERTEXATTRIB1DVPROC pglVertexAttrib1dv;
// extern GLAPI PFNGLVERTEXATTRIB1FPROC pglVertexAttrib1f;
// extern GLAPI PFNGLVERTEXATTRIB1FVPROC pglVertexAttrib1fv;
// extern GLAPI PFNGLVERTEXATTRIB1SPROC pglVertexAttrib1s;
// extern GLAPI PFNGLVERTEXATTRIB1SVPROC pglVertexAttrib1sv;
// extern GLAPI PFNGLVERTEXATTRIB2DPROC pglVertexAttrib2d;
// extern GLAPI PFNGLVERTEXATTRIB2DVPROC pglVertexAttrib2dv;
// extern GLAPI PFNGLVERTEXATTRIB2FPROC pglVertexAttrib2f;
// extern GLAPI PFNGLVERTEXATTRIB2FVPROC pglVertexAttrib2fv;
// extern GLAPI PFNGLVERTEXATTRIB2SPROC pglVertexAttrib2s;
// extern GLAPI PFNGLVERTEXATTRIB2SVPROC pglVertexAttrib2sv;
// extern GLAPI PFNGLVERTEXATTRIB3DPROC pglVertexAttrib3d;
// extern GLAPI PFNGLVERTEXATTRIB3DVPROC pglVertexAttrib3dv;
// extern GLAPI PFNGLVERTEXATTRIB3FPROC pglVertexAttrib3f;
// extern GLAPI PFNGLVERTEXATTRIB3FVPROC pglVertexAttrib3fv;
// extern GLAPI PFNGLVERTEXATTRIB3SPROC pglVertexAttrib3s;
// extern GLAPI PFNGLVERTEXATTRIB3SVPROC pglVertexAttrib3sv;
// extern GLAPI PFNGLVERTEXATTRIB4NBVPROC pglVertexAttrib4Nbv;
// extern GLAPI PFNGLVERTEXATTRIB4NIVPROC pglVertexAttrib4Niv;
// extern GLAPI PFNGLVERTEXATTRIB4NSVPROC pglVertexAttrib4Nsv;
// extern GLAPI PFNGLVERTEXATTRIB4NUBPROC pglVertexAttrib4Nub;
// extern GLAPI PFNGLVERTEXATTRIB4NUBVPROC pglVertexAttrib4Nubv;
// extern GLAPI PFNGLVERTEXATTRIB4NUIVPROC pglVertexAttrib4Nuiv;
// extern GLAPI PFNGLVERTEXATTRIB4NUSVPROC pglVertexAttrib4Nusv;
// extern GLAPI PFNGLVERTEXATTRIB4BVPROC pglVertexAttrib4bv;
// extern GLAPI PFNGLVERTEXATTRIB4DPROC pglVertexAttrib4d;
// extern GLAPI PFNGLVERTEXATTRIB4DVPROC pglVertexAttrib4dv;
// extern GLAPI PFNGLVERTEXATTRIB4FPROC pglVertexAttrib4f;
// extern GLAPI PFNGLVERTEXATTRIB4FVPROC pglVertexAttrib4fv;
// extern GLAPI PFNGLVERTEXATTRIB4IVPROC pglVertexAttrib4iv;
// extern GLAPI PFNGLVERTEXATTRIB4SPROC pglVertexAttrib4s;
// extern GLAPI PFNGLVERTEXATTRIB4SVPROC pglVertexAttrib4sv;
// extern GLAPI PFNGLVERTEXATTRIB4UBVPROC pglVertexAttrib4ubv;
// extern GLAPI PFNGLVERTEXATTRIB4UIVPROC pglVertexAttrib4uiv;
// extern GLAPI PFNGLVERTEXATTRIB4USVPROC pglVertexAttrib4usv;
extern GLAPI PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer;
extern GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray;
// extern GLAPI PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray;
// extern GLAPI PFNGLPROGRAMSTRINGPROC pglProgramString;
// extern GLAPI PFNGLBINDPROGRAMPROC pglBindProgram;
// extern GLAPI PFNGLDELETEPROGRAMSPROC pglDeletePrograms;
// extern GLAPI PFNGLGENPROGRAMSPROC pglGenPrograms;
// extern GLAPI PFNGLPROGRAMENVPARAMETER4DPROC pglProgramEnvParameter4d;
// extern GLAPI PFNGLPROGRAMENVPARAMETER4DVPROC pglProgramEnvParameter4dv;
// extern GLAPI PFNGLPROGRAMENVPARAMETER4FPROC pglProgramEnvParameter4f;
// extern GLAPI PFNGLPROGRAMENVPARAMETER4FVPROC pglProgramEnvParameter4fv;
// extern GLAPI PFNGLPROGRAMLOCALPARAMETER4DPROC pglProgramLocalParameter4d;
// extern GLAPI PFNGLPROGRAMLOCALPARAMETER4DVPROC pglProgramLocalParameter4dv;
// extern GLAPI PFNGLPROGRAMLOCALPARAMETER4FPROC pglProgramLocalParameter4f;
// extern GLAPI PFNGLPROGRAMLOCALPARAMETER4FVPROC pglProgramLocalParameter4fv;
// extern GLAPI PFNGLGETPROGRAMENVPARAMETERDVPROC pglGetProgramEnvParameterdv;
// extern GLAPI PFNGLGETPROGRAMENVPARAMETERFVPROC pglGetProgramEnvParameterfv;
// extern GLAPI PFNGLGETPROGRAMLOCALPARAMETERDVPROC pglGetProgramLocalParameterdv;
// extern GLAPI PFNGLGETPROGRAMLOCALPARAMETERFVPROC pglGetProgramLocalParameterfv;
// extern GLAPI PFNGLGETPROGRAMIVPROC pglGetProgramiv;
// extern GLAPI PFNGLGETPROGRAMSTRINGPROC pglGetProgramString;
// extern GLAPI PFNGLGETVERTEXATTRIBDVPROC pglGetVertexAttribdv;
// extern GLAPI PFNGLGETVERTEXATTRIBFVPROC pglGetVertexAttribfv;
// extern GLAPI PFNGLGETVERTEXATTRIBIVPROC pglGetVertexAttribiv;
// extern GLAPI PFNGLGETVERTEXATTRIBPOINTERVPROC pglGetVertexAttribPointerv;
// extern GLAPI PFNGLISPROGRAMPROC pglIsProgram;



// GL_ARB_vertex_shader : opengl version 2.0
//////////////////////////////////////////////

extern GLAPI PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation;
// extern GLAPI PFNGLGETACTIVEATTRIBPROC pglGetActiveAttrib;
// extern GLAPI PFNGLGETATTRIBLOCATIONPROC pglGetAttribLocation;



// GL_ARB_draw_buffers : opengl version 2.0
/////////////////////////////////////////////

extern GLAPI PFNGLDRAWBUFFERSPROC pglDrawBuffers;



// GL_ARB_shader_objects . no opengl version, most probably 2.1
/////////////////////////////////////////////////////////////////

extern GLAPI PFNGLDELETESHADERPROC pglDeleteShader; // core only
extern GLAPI PFNGLDELETEPROGRAMPROC pglDeleteProgram; // core only
extern GLAPI PFNGLCREATESHADERPROC pglCreateShader; // core only
extern GLAPI PFNGLCREATEPROGRAMPROC pglCreateProgram; // core only
extern GLAPI PFNGLATTACHSHADERPROC pglAttachShader; // core only
extern GLAPI PFNGLUSEPROGRAMPROC pglUseProgram; // core only
extern GLAPI PFNGLGETSHADERIVPROC pglGetShaderiv; // core only
extern GLAPI PFNGLGETPROGRAMIVPROC pglGetProgramiv; // core only
extern GLAPI PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog; // core only
extern GLAPI PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog; // core only

// extern GLAPI PFNGLDELETEOBJECTPROC pglDeleteObject;
// extern GLAPI PFNGLGETHANDLEPROC pglGetHandle;
// extern GLAPI PFNGLDETACHOBJECTPROC pglDetachObject;
// extern GLAPI PFNGLCREATESHADEROBJECTPROC pglCreateShaderObject;
extern GLAPI PFNGLSHADERSOURCEPROC pglShaderSource;
extern GLAPI PFNGLCOMPILESHADERPROC pglCompileShader;
// extern GLAPI PFNGLCREATEPROGRAMOBJECTPROC pglCreateProgramObject;
// extern GLAPI PFNGLATTACHOBJECTPROC pglAttachObject;
extern GLAPI PFNGLLINKPROGRAMPROC pglLinkProgram;
// extern GLAPI PFNGLUSEPROGRAMOBJECTPROC pglUseProgramObject;
// extern GLAPI PFNGLVALIDATEPROGRAMPROC pglValidateProgram;
extern GLAPI PFNGLUNIFORM1FPROC pglUniform1f;
extern GLAPI PFNGLUNIFORM2FPROC pglUniform2f;
extern GLAPI PFNGLUNIFORM3FPROC pglUniform3f;
extern GLAPI PFNGLUNIFORM4FPROC pglUniform4f;
extern GLAPI PFNGLUNIFORM1IPROC pglUniform1i;
extern GLAPI PFNGLUNIFORM2IPROC pglUniform2i;
extern GLAPI PFNGLUNIFORM3IPROC pglUniform3i;
extern GLAPI PFNGLUNIFORM4IPROC pglUniform4i;
// extern GLAPI PFNGLUNIFORM1FVPROC pglUniform1fv;
// extern GLAPI PFNGLUNIFORM2FVPROC pglUniform2fv;
// extern GLAPI PFNGLUNIFORM3FVPROC pglUniform3fv;
// extern GLAPI PFNGLUNIFORM4FVPROC pglUniform4fv;
// extern GLAPI PFNGLUNIFORM1IVPROC pglUniform1iv;
// extern GLAPI PFNGLUNIFORM2IVPROC pglUniform2iv;
// extern GLAPI PFNGLUNIFORM3IVPROC pglUniform3iv;
// extern GLAPI PFNGLUNIFORM4IVPROC pglUniform4iv;
// extern GLAPI PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv;
extern GLAPI PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv;
extern GLAPI PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
// extern GLAPI PFNGLGETOBJECTPARAMETERFVPROC pglGetObjectParameterfv;
// extern GLAPI PFNGLGETOBJECTPARAMETERIVPROC pglGetObjectParameteriv;
// extern GLAPI PFNGLGETINFOLOGPROC pglGetInfoLog;
// extern GLAPI PFNGLGETATTACHEDOBJECTSPROC pglGetAttachedObjects;
extern GLAPI PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
// extern GLAPI PFNGLGETACTIVEUNIFORMPROC pglGetActiveUniform;
// extern GLAPI PFNGLGETUNIFORMFVPROC pglGetUniformfv;
// extern GLAPI PFNGLGETUNIFORMIVPROC pglGetUniformiv;
// extern GLAPI PFNGLGETSHADERSOURCEPROC pglGetShaderSource;



// these are listed in opengl version 2.1 but do not look like belonging directly to an extension
///////////////////////////////////////////////////////////////////////////////////////////////////

// extern GLAPI PFNGLUNIFORMMATRIX2X3FVPROC pglUniformMatrix2x3fv;
extern GLAPI PFNGLUNIFORMMATRIX3X2FVPROC pglUniformMatrix3x2fv;
// extern GLAPI PFNGLUNIFORMMATRIX2X4FVPROC pglUniformMatrix2x4fv;
// extern GLAPI PFNGLUNIFORMMATRIX4X2FVPROC pglUniformMatrix4x2fv;
// extern GLAPI PFNGLUNIFORMMATRIX3X4FVPROC pglUniformMatrix3x4fv;
extern GLAPI PFNGLUNIFORMMATRIX4X3FVPROC pglUniformMatrix4x3fv;



// these are listed in opengl version 3.0 but do not have an extension assigned
/////////////////////////////////////////////////////////////////////////////////

extern GLAPI PFNGLCLEARBUFFERIVPROC pglClearBufferiv;
// extern GLAPI PFNGLCLEARBUFFERUIVPROC pglClearBufferuiv;
extern GLAPI PFNGLCLEARBUFFERFVPROC pglClearBufferfv;
extern GLAPI PFNGLCLEARBUFFERFIPROC pglClearBufferfi;



// OpenGL 3.2 core stuff
//////////////////////////

extern GLAPI PFNGLFENCESYNCPROC pglFenceSync;
extern GLAPI PFNGLDELETESYNCPROC pglDeleteSync;
extern GLAPI PFNGLCLIENTWAITSYNCPROC pglClientWaitSync;
// extern GLAPI PFNGLWAITSYNCPROC pglWaitSync;



// GL_ARB_draw_elements_base_vertex : opengl version 3.2
//////////////////////////////////////////////////////////

extern GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC pglDrawElementsBaseVertex;
// extern GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC pglDrawRangeElementsBaseVertex;
extern GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC pglDrawElementsInstancedBaseVertex;
extern GLAPI PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC pglMultiDrawElementsBaseVertex;
extern GLAPI PFNGLVERTEXATTRIBIPOINTERPROC pglVertexAttribIPointer;



// GL_ARB_sampler_objects : opengl version 3.3
////////////////////////////////////////////////

extern GLAPI PFNGLGENSAMPLERSPROC pglGenSamplers;
extern GLAPI PFNGLDELETESAMPLERSPROC pglDeleteSamplers;
// extern GLAPI PFNGLISSAMPLERPROC pglIsSampler;
extern GLAPI PFNGLBINDSAMPLERPROC pglBindSampler;
extern GLAPI PFNGLSAMPLERPARAMETERIPROC pglSamplerParameteri;
// extern GLAPI PFNGLSAMPLERPARAMETERIVPROC pglSamplerParameteriv;
// extern GLAPI PFNGLSAMPLERPARAMETERFPROC pglSamplerParameterf;
// extern GLAPI PFNGLSAMPLERPARAMETERFVPROC pglSamplerParameterfv;
// extern GLAPI PFNGLSAMPLERPARAMETERIIVPROC pglSamplerParameterIiv;
// extern GLAPI PFNGLSAMPLERPARAMETERIUIVPROC pglSamplerParameterIuiv;
// extern GLAPI PFNGLGETSAMPLERPARAMETERIVPROC pglGetSamplerParameteriv;
// extern GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC pglGetSamplerParameterIiv;
// extern GLAPI PFNGLGETSAMPLERPARAMETERFVPROC pglGetSamplerParameterfv;
// extern GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC pglGetSamplerParameterIuiv;



// GL_ARB_texture_array : opengl 3.0
//////////////////////////////////////

extern GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC pglFramebufferTextureLayer;



// GL_ARB_framebuffer_object : opengl 3.0
///////////////////////////////////////////

// extern GLAPI PFNGLISRENDERBUFFERPROC pglIsRenderbuffer;
// extern GLAPI PFNGLBINDRENDERBUFFERPROC pglBindRenderbuffer;
// extern GLAPI PFNGLDELETERENDERBUFFERSPROC pglDeleteRenderbuffers;
// extern GLAPI PFNGLGENRENDERBUFFERSPROC pglGenRenderbuffers;
// extern GLAPI PFNGLRENDERBUFFERSTORAGEPROC pglRenderbufferStorage;
// extern GLAPI PFNGLGETRENDERBUFFERPARAMETERIVPROC pglGetRenderbufferParameteriv;
// extern GLAPI PFNGLISFRAMEBUFFERPROC pglIsFramebuffer;
extern GLAPI PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer;
extern GLAPI PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers;
extern GLAPI PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers;
extern GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus;
extern GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D;
// extern GLAPI PFNGLFRAMEBUFFERTEXTURE3DPROC pglFramebufferTexture3D;
// extern GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC pglFramebufferRenderbuffer;
// extern GLAPI PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC pglGetFramebufferAttachmentParameteriv;
extern GLAPI PFNGLGENERATEMIPMAPPROC pglGenerateMipmap;
extern GLAPI PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer;
// extern GLAPI PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC pglRenderbufferStorageMultisample;
extern GLAPI PFNGLFRAMEBUFFERTEXTUREARBPROC pglFramebufferTexture;



// GL_ARB_geometry_shader4 : no opengl version
////////////////////////////////////////////////

// extern GLAPI PFNGLPROGRAMPARAMETERIARBPROC pglProgramParameteri;
extern GLAPI PFNGLFRAMEBUFFERTEXTUREFACEARBPROC pglFramebufferTextureFace;



// GL_ARB_vertex_array_object : no opengl version
///////////////////////////////////////////////////

extern GLAPI PFNGLBINDVERTEXARRAYPROC pglBindVertexArray;
extern GLAPI PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays;
extern GLAPI PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays;
// extern GLAPI PFNGLISVERTEXARRAYPROC pglIsVertexArray;



// GL_EXT_subtexture : no opengl version
//////////////////////////////////////////

// extern GLAPI PFNGLTEXSUBIMAGE1DEXTPROC pglTexSubImage1D;
extern GLAPI PFNGLTEXSUBIMAGE2DEXTPROC pglTexSubImage2D;



// GL_EXT_copy_texture : no opengl version
////////////////////////////////////////////

// extern GLAPI PFNGLCOPYTEXIMAGE1DEXTPROC pglCopyTexImage1D;
// extern GLAPI PFNGLCOPYTEXIMAGE2DEXTPROC pglCopyTexImage2D;
// extern GLAPI PFNGLCOPYTEXSUBIMAGE1DEXTPROC pglCopyTexSubImage1D;
// extern GLAPI PFNGLCOPYTEXSUBIMAGE2DEXTPROC pglCopyTexSubImage2D;
// extern GLAPI PFNGLCOPYTEXSUBIMAGE3DEXTPROC pglCopyTexSubImage3D;



// GL_EXT_texture_object : no opengl version
//////////////////////////////////////////////

// extern GLAPI PFNGLARETEXTURESRESIDENTEXTPROC pglAreTexturesResident;
// extern GLAPI PFNGLBINDTEXTUREEXTPROC pglBindTexture;
// extern GLAPI PFNGLDELETETEXTURESEXTPROC pglDeleteTextures;
// extern GLAPI PFNGLGENTEXTURESEXTPROC pglGenTextures;
// extern GLAPI PFNGLISTEXTUREEXTPROC pglIsTexture;
// extern GLAPI PFNGLPRIORITIZETEXTURESEXTPROC pglPrioritizeTextures;



// Optional Extensions
////////////////////////

// GL_ARB_multisample : opengl version 1.3
////////////////////////////////////////////

// extern GLAPI PFNGLSAMPLECOVERAGEARBPROC pglSampleCoverage;



// GL_ARB_draw_instanced : opengl version 3.1
///////////////////////////////////////////////

extern GLAPI PFNGLDRAWARRAYSINSTANCEDARBPROC pglDrawArraysInstanced;
extern GLAPI PFNGLDRAWELEMENTSINSTANCEDARBPROC pglDrawElementsInstanced;



// GL_EXT_transform_feedback : opengl version 3.0
///////////////////////////////////////////////////

// extern GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC pglBeginTransformFeedback;
// extern GLAPI PFNGLENDTRANSFORMFEEDBACKPROC pglEndTransformFeedback;
extern GLAPI PFNGLBINDBUFFERRANGEPROC pglBindBufferRange;
// extern GLAPI PFNGLBINDBUFFEROFFSETEXTPROC pglBindBufferOffset; // EXT-only
extern GLAPI PFNGLBINDBUFFERBASEPROC pglBindBufferBase;
extern GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC pglTransformFeedbackVaryings;
// extern GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC pglGetTransformFeedbackVarying;




// GL_EXT_gpu_shader4 : opengl version 3.0
////////////////////////////////////////////

// extern GLAPI PFNGLGETUNIFORMUIVEXTPROC pglGetUniformuiv;
extern GLAPI PFNGLBINDFRAGDATALOCATIONEXTPROC pglBindFragDataLocation;
// extern GLAPI PFNGLGETFRAGDATALOCATIONEXTPROC pglGetFragDataLocation;
extern GLAPI PFNGLUNIFORM1UIEXTPROC pglUniform1ui;
extern GLAPI PFNGLUNIFORM2UIEXTPROC pglUniform2ui;
extern GLAPI PFNGLUNIFORM3UIEXTPROC pglUniform3ui;
extern GLAPI PFNGLUNIFORM4UIEXTPROC pglUniform4ui;
// extern GLAPI PFNGLUNIFORM1UIVEXTPROC pglUniform1uiv;
// extern GLAPI PFNGLUNIFORM2UIVEXTPROC pglUniform2uiv;
// extern GLAPI PFNGLUNIFORM3UIVEXTPROC pglUniform3uiv;
// extern GLAPI PFNGLUNIFORM4UIVEXTPROC pglUniform4uiv;



// GL_ARB_texture_buffer_object : opengl version 3.1
//////////////////////////////////////////////////////

extern GLAPI PFNGLTEXBUFFERARBPROC pglTexBuffer;



// GL_ARB_timer_query : opengl version 3.3
////////////////////////////////////////////

// extern GLAPI PFNGLQUERYCOUNTERPROC pglQueryCounter;
// extern GLAPI PFNGLGETQUERYOBJECTI64VPROC pglGetQueryObjecti64v;
// extern GLAPI PFNGLGETQUERYOBJECTUI64VPROC pglGetQueryObjectui64v;



// GL_ARB_draw_buffers_blend : opengl version 4.0
///////////////////////////////////////////////////

// extern GLAPI PFNGLBLENDEQUATIONIARBPROC pglBlendEquationi;
// extern GLAPI PFNGLBLENDEQUATIONSEPARATEIARBPROC pglBlendEquationSeparatei;
// extern GLAPI PFNGLBLENDFUNCIARBPROC pglBlendFunci;
// extern GLAPI PFNGLBLENDFUNCSEPARATEIARBPROC pglBlendFuncSeparatei;

// OpenGL version 1.4
///////////////////////

// extern GLAPI PFNGLBLENDFUNCSEPARATEPROC pglBlendFuncSeparate;



// GL_ARB_tessellation_shader : opengl version 4.0
////////////////////////////////////////////////////

extern GLAPI PFNGLPATCHPARAMETERIPROC pglPatchParameteri;
// extern GLAPI PFNGLPATCHPARAMETERFVPROC pglPatchParameterfv;



// GL_ARB_transform_feedback2 : opengl version 4.0
////////////////////////////////////////////////////

/*
extern GLAPI PFNGLBINDTRANSFORMFEEDBACKPROC pglBindTransformFeedback;
extern GLAPI PFNGLDELETETRANSFORMFEEDBACKSPROC pglDeleteTransformFeedbacks;
extern GLAPI PFNGLGENTRANSFORMFEEDBACKSPROC pglGenTransformFeedbacks;
extern GLAPI PFNGLISTRANSFORMFEEDBACKPROC pglIsTransformFeedback;
extern GLAPI PFNGLPAUSETRANSFORMFEEDBACKPROC pglPauseTransformFeedback;
extern GLAPI PFNGLRESUMETRANSFORMFEEDBACKPROC pglResumeTransformFeedback;
extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKPROC pglDrawTransformFeedback;
*/


// GL_ARB_transform_feedback3 : opengl version 4.0
////////////////////////////////////////////////////

/*
extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC pglDrawTransformFeedbackStream;
extern GLAPI PFNGLBEGINQUERYINDEXEDPROC pglBeginQueryIndexed;
extern GLAPI PFNGLENDQUERYINDEXEDPROC pglEndQueryIndexed;
extern GLAPI PFNGLGETQUERYINDEXEDIVPROC pglGetQueryIndexediv;
*/


// GL_ARB_viewport_array : opengl version 4.1
///////////////////////////////////////////////

/*
extern GLAPI PFNGLVIEWPORTARRAYVPROC pglViewportArrayv;
extern GLAPI PFNGLVIEWPORTINDEXEDFPROC pglViewportIndexedf;
extern GLAPI PFNGLVIEWPORTINDEXEDFVPROC pglViewportIndexedfv;
extern GLAPI PFNGLSCISSORARRAYVPROC pglScissorArrayv;
extern GLAPI PFNGLSCISSORINDEXEDPROC pglScissorIndexed;
extern GLAPI PFNGLSCISSORINDEXEDVPROC pglScissorIndexedv;
extern GLAPI PFNGLDEPTHRANGEARRAYVPROC pglDepthRangeArrayv;
extern GLAPI PFNGLDEPTHRANGEINDEXEDPROC pglDepthRangeIndexed;
extern GLAPI PFNGLGETFLOATI_VPROC pglGetFloati_v;
extern GLAPI PFNGLGETDOUBLEI_VPROC pglGetDoublei_v;

extern GLAPI PFNGLENABLEINDEXEDEXTPROC pglEnableIndexed;
extern GLAPI PFNGLDISABLEINDEXEDEXTPROC pglDisableIndexed;
extern GLAPI PFNGLISENABLEDINDEXEDEXTPROC pglIsEnabledIndexed;
extern GLAPI PFNGLGETINTEGERINDEXEDVEXTPROC pglGetIntegerIndexedv;
*/


// GL_ARB_transform_feedback_instanced : opengl version 4.2
/////////////////////////////////////////////////////////////

/*
extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC pglDrawTransformFeedbackInstanced;
extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC pglDrawTransformFeedbackStreamInstanced;
*/


// GL_ARB_copy_image : opengl version 4.3
///////////////////////////////////////////

extern GLAPI PFNGLCOPYIMAGESUBDATAPROC pglCopyImageSubData;



// GL_NV_copy_image : opengl version 4.3
//////////////////////////////////////////

extern GLAPI PFNGLCOPYIMAGESUBDATANVPROC pglCopyImageSubDataNV;



// GL_ARB_shader_storage_buffer_object : opengl version 4.3
/////////////////////////////////////////////////////////////

extern GLAPI PFNGLSHADERSTORAGEBLOCKBINDINGPROC pglShaderStorageBlockBinding;



// GL_ARB_program_interface_query : opengl version 4.3
/////////////////////////////////////////////////////////////

extern GLAPI PFNGLGETPROGRAMINTERFACEIVPROC pglGetProgramInterfaceiv;
extern GLAPI PFNGLGETPROGRAMRESOURCEINDEXPROC pglGetProgramResourceIndex;



// GL_ARB_compute_shader : opengl version 4.3
///////////////////////////////////////////////

extern GLAPI PFNGLDISPATCHCOMPUTEPROC pglDispatchCompute;
extern GLAPI PFNGLDISPATCHCOMPUTEINDIRECTPROC pglDispatchComputeIndirect;



// GL_ARB_draw_indirect : opengl version 4.3
//////////////////////////////////////////////

struct oglDrawArraysIndirectCommand{
	GLuint count;
	GLuint instanceCount;
	GLuint first;
	GLuint baseInstance;
	
	oglDrawArraysIndirectCommand();
};

struct oglDrawElementsIndirectCommand{
	GLuint count;
	GLuint instanceCount;
	GLuint firstIndex;
	GLint baseVertex;
	GLuint baseInstance;
	
	oglDrawElementsIndirectCommand();
};

union oglDrawIndirectCommand{
	struct Element{
		GLuint count;
		GLuint instanceCount;
		GLuint firstIndex;
		GLint baseVertex;
		GLuint baseInstance;
	} element;
	
	struct Array{
		GLuint count;
		GLuint instanceCount;
		GLuint first;
		GLuint baseInstance;
	} array;
};

extern GLAPI PFNGLDRAWARRAYSINDIRECTPROC pglDrawArraysIndirect;
extern GLAPI PFNGLDRAWELEMENTSINDIRECTPROC pglDrawElementsIndirect;



// GL_ARB_multi_draw_indirect : opengl version 4.3
////////////////////////////////////////////////////

/**
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * 
 * Sourcing parameters from client memory is NOT allowed although the documentatin claims so!
 * If you try doing this you get GL_INVALID_OPERATION and weeks of big headaches!
 * This ONLY works with GL_DRAW_INDIRECT_BUFFER and nothing else!
 * 
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */
extern GLAPI PFNGLMULTIDRAWARRAYSINDIRECTPROC pglMultiDrawArraysIndirect;

/**
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 * 
 * Sourcing parameters from client memory is NOT allowed although the documentatin claims so!
 * If you try doing this you get GL_INVALID_OPERATION and weeks of big headaches!
 * This ONLY works with GL_DRAW_INDIRECT_BUFFER and nothing else!
 * 
 * WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
 */
extern GLAPI PFNGLMULTIDRAWELEMENTSINDIRECTPROC pglMultiDrawElementsIndirect;



// GL_ARB_clear_buffer_data : opengl version 4.3
//////////////////////////////////////////////////

extern GLAPI PFNGLCLEARBUFFERSUBDATAPROC pglClearBufferSubData;



// GL_ARB_buffer_storage : opengl version 4.3
///////////////////////////////////////////////

// extern GLAPI PFNGLBUFFERSTORAGEPROC pglBufferStorage;



// GL_ARB_direct_state_access : opengl version 4.4
////////////////////////////////////////////////////

extern GLAPI PFNGLCREATEBUFFERSPROC pglCreateBuffers;
extern GLAPI PFNGLNAMEDBUFFERSTORAGEPROC pglNamedBufferStorage;
extern GLAPI PFNGLNAMEDBUFFERDATAPROC pglNamedBufferData;
extern GLAPI PFNGLNAMEDBUFFERSUBDATAPROC pglNamedBufferSubData;
extern GLAPI PFNGLCOPYNAMEDBUFFERSUBDATAPROC pglCopyNamedBufferSubData;
extern GLAPI PFNGLCLEARNAMEDBUFFERDATAPROC pglClearNamedBufferData;
extern GLAPI PFNGLCLEARNAMEDBUFFERSUBDATAPROC pglClearNamedBufferSubData;
extern GLAPI PFNGLMAPNAMEDBUFFERPROC pglMapNamedBuffer;
extern GLAPI PFNGLMAPNAMEDBUFFERRANGEPROC pglMapNamedBufferRange;
extern GLAPI PFNGLUNMAPNAMEDBUFFERPROC pglUnmapNamedBuffer;
extern GLAPI PFNGLCREATEFRAMEBUFFERSPROC pglCreateFramebuffers;
extern GLAPI PFNGLNAMEDFRAMEBUFFERTEXTUREPROC pglNamedFramebufferTexture;
extern GLAPI PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC pglNamedFramebufferTextureLayer;
extern GLAPI PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC pglNamedFramebufferDrawBuffers;
extern GLAPI PFNGLCLEARNAMEDFRAMEBUFFERFVPROC pglClearNamedFramebufferfv;
extern GLAPI PFNGLCLEARNAMEDFRAMEBUFFERFIPROC pglClearNamedFramebufferfi;
extern GLAPI PFNGLBLITNAMEDFRAMEBUFFERPROC pglBlitNamedFramebuffer;
extern GLAPI PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC pglCheckNamedFramebufferStatus;
extern GLAPI PFNGLCREATETEXTURESPROC pglCreateTextures;
extern GLAPI PFNGLTEXTUREBUFFERPROC pglTextureBuffer;
extern GLAPI PFNGLTEXTUREBUFFERRANGEPROC pglTextureBufferRange;
extern GLAPI PFNGLTEXTURESTORAGE2DPROC pglTextureStorage2D;
extern GLAPI PFNGLTEXTURESTORAGE3DPROC pglTextureStorage3D;
extern GLAPI PFNGLTEXTURESUBIMAGE2DPROC pglTextureSubImage2D;
extern GLAPI PFNGLTEXTURESUBIMAGE3DPROC pglTextureSubImage3D;
extern GLAPI PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC pglCompressedTextureSubImage2D;
extern GLAPI PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC pglCompressedTextureSubImage3D;
extern GLAPI PFNGLCOPYTEXTURESUBIMAGE2DPROC pglCopyTextureSubImage2D;
extern GLAPI PFNGLCOPYTEXTURESUBIMAGE3DPROC pglCopyTextureSubImage3D;
extern GLAPI PFNGLTEXTUREPARAMETERIPROC pglTextureParameteri;
extern GLAPI PFNGLGENERATETEXTUREMIPMAPPROC pglGenerateTextureMipmap;
extern GLAPI PFNGLBINDTEXTUREUNITPROC pglBindTextureUnit;
extern GLAPI PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC pglGetCompressedTextureImage;
extern GLAPI PFNGLGETTEXTURELEVELPARAMETERIVPROC pglGetTextureLevelParameteriv;
extern GLAPI PFNGLCREATEVERTEXARRAYSPROC pglCreateVertexArrays;
extern GLAPI PFNGLENABLEVERTEXARRAYATTRIBPROC pglEnableVertexArrayAttrib;
extern GLAPI PFNGLVERTEXARRAYELEMENTBUFFERPROC pglVertexArrayElementBuffer;
extern GLAPI PFNGLVERTEXARRAYVERTEXBUFFERSPROC pglVertexArrayVertexBuffers;
extern GLAPI PFNGLVERTEXARRAYATTRIBBINDINGPROC pglVertexArrayAttribBinding;
extern GLAPI PFNGLVERTEXARRAYBINDINGDIVISORPROC pglVertexArrayBindingDivisor;



// GL_ARB_clip_control : opengl version 4.5
/////////////////////////////////////////////

extern GLAPI PFNGLCLIPCONTROLPROC pglClipControl;



// GL_ARB_indirect_parameters : opengl version 4.6
////////////////////////////////////////////////////

extern GLAPI PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC pglMultiDrawArraysIndirectCountARB;
extern GLAPI PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC pglMultiDrawElementsIndirectCountARB;



// GL_ARB_uniform_buffer_object : no opengl version
/////////////////////////////////////////////////////

// extern GLAPI PFNGLGETUNIFORMINDICESPROC pglGetUniformIndices;
// extern GLAPI PFNGLGETACTIVEUNIFORMSIVPROC pglGetActiveUniformsiv;
// extern GLAPI PFNGLGETACTIVEUNIFORMNAMEPROC pglGetActiveUniformName;
extern GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC pglGetUniformBlockIndex;
// extern GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC pglGetActiveUniformBlockiv;
// extern GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC pglGetActiveUniformBlockName;
extern GLAPI PFNGLUNIFORMBLOCKBINDINGPROC pglUniformBlockBinding;



// GL_ARB_copy_buffer : no opengl version
///////////////////////////////////////////

extern GLAPI PFNGLCOPYBUFFERSUBDATAPROC pglCopyBufferSubData;



// GL_ARB_texture_multisample : no opengl version
///////////////////////////////////////////////////

// extern GLAPI PFNGLTEXIMAGE2DMULTISAMPLEPROC pglTexImage2DMultisample;
// extern GLAPI PFNGLTEXIMAGE3DMULTISAMPLEPROC pglTexImage3DMultisample;
// extern GLAPI PFNGLGETMULTISAMPLEFVPROC pglGetMultisamplefv;
// extern GLAPI PFNGLSAMPLEMASKIPROC pglSampleMaski;



// GL_ARB_get_program_binary : no opengl version
//////////////////////////////////////////////////

extern GLAPI PFNGLGETPROGRAMBINARYPROC pglGetProgramBinary;
extern GLAPI PFNGLPROGRAMBINARYPROC pglProgramBinary;
// extern GLAPI PFNGLPROGRAMPARAMETERIPROC pglProgramParameteri;



// GL_ARB_separate_shader_objects : no opengl version
///////////////////////////////////////////////////////

// extern GLAPI PFNGLUSEPROGRAMSTAGESPROC pglUseProgramStages;
// extern GLAPI PFNGLACTIVESHADERPROGRAMPROC pglActiveShaderProgram;
// extern GLAPI PFNGLCREATESHADERPROGRAMVPROC pglCreateShaderProgramv;
// extern GLAPI PFNGLBINDPROGRAMPIPELINEPROC pglBindProgramPipeline;
// extern GLAPI PFNGLDELETEPROGRAMPIPELINESPROC pglDeleteProgramPipelines;
// extern GLAPI PFNGLGENPROGRAMPIPELINESPROC pglGenProgramPipelines;
// extern GLAPI PFNGLISPROGRAMPIPELINEPROC pglIsProgramPipeline;
// extern GLAPI PFNGLGETPROGRAMPIPELINEIVPROC pglGetProgramPipelineiv;
// extern GLAPI PFNGLPROGRAMUNIFORM1IPROC pglProgramUniform1i;
// extern GLAPI PFNGLPROGRAMUNIFORM1IVPROC pglProgramUniform1iv;
// extern GLAPI PFNGLPROGRAMUNIFORM1FPROC pglProgramUniform1f;
// extern GLAPI PFNGLPROGRAMUNIFORM1FVPROC pglProgramUniform1fv;
// extern GLAPI PFNGLPROGRAMUNIFORM1DPROC pglProgramUniform1d;
// extern GLAPI PFNGLPROGRAMUNIFORM1DVPROC pglProgramUniform1dv;
// extern GLAPI PFNGLPROGRAMUNIFORM1UIPROC pglProgramUniform1ui;
// extern GLAPI PFNGLPROGRAMUNIFORM1UIVPROC pglProgramUniform1uiv;
// extern GLAPI PFNGLPROGRAMUNIFORM2IPROC pglProgramUniform2i;
// extern GLAPI PFNGLPROGRAMUNIFORM2IVPROC pglProgramUniform2iv;
// extern GLAPI PFNGLPROGRAMUNIFORM2FPROC pglProgramUniform2f;
// extern GLAPI PFNGLPROGRAMUNIFORM2FVPROC pglProgramUniform2fv;
// extern GLAPI PFNGLPROGRAMUNIFORM2DPROC pglProgramUniform2d;
// extern GLAPI PFNGLPROGRAMUNIFORM2DVPROC pglProgramUniform2dv;
// extern GLAPI PFNGLPROGRAMUNIFORM2UIPROC pglProgramUniform2ui;
// extern GLAPI PFNGLPROGRAMUNIFORM2UIVPROC pglProgramUniform2uiv;
// extern GLAPI PFNGLPROGRAMUNIFORM3IPROC pglProgramUniform3i;
// extern GLAPI PFNGLPROGRAMUNIFORM3IVPROC pglProgramUniform3iv;
// extern GLAPI PFNGLPROGRAMUNIFORM3FPROC pglProgramUniform3f;
// extern GLAPI PFNGLPROGRAMUNIFORM3FVPROC pglProgramUniform3fv;
// extern GLAPI PFNGLPROGRAMUNIFORM3DPROC pglProgramUniform3d;
// extern GLAPI PFNGLPROGRAMUNIFORM3DVPROC pglProgramUniform3dv;
// extern GLAPI PFNGLPROGRAMUNIFORM3UIPROC pglProgramUniform3ui;
// extern GLAPI PFNGLPROGRAMUNIFORM3UIVPROC pglProgramUniform3uiv;
// extern GLAPI PFNGLPROGRAMUNIFORM4IPROC pglProgramUniform4i;
// extern GLAPI PFNGLPROGRAMUNIFORM4IVPROC pglProgramUniform4iv;
// extern GLAPI PFNGLPROGRAMUNIFORM4FPROC pglProgramUniform4f;
// extern GLAPI PFNGLPROGRAMUNIFORM4FVPROC pglProgramUniform4fv;
// extern GLAPI PFNGLPROGRAMUNIFORM4DPROC pglProgramUniform4d;
// extern GLAPI PFNGLPROGRAMUNIFORM4DVPROC pglProgramUniform4dv;
// extern GLAPI PFNGLPROGRAMUNIFORM4UIPROC pglProgramUniform4ui;
// extern GLAPI PFNGLPROGRAMUNIFORM4UIVPROC pglProgramUniform4uiv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2FVPROC pglProgramUniformMatrix2fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3FVPROC pglProgramUniformMatrix3fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4FVPROC pglProgramUniformMatrix4fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2DVPROC pglProgramUniformMatrix2dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3DVPROC pglProgramUniformMatrix3dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4DVPROC pglProgramUniformMatrix4dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC pglProgramUniformMatrix2x3fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC pglProgramUniformMatrix3x2fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC pglProgramUniformMatrix2x4fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC pglProgramUniformMatrix4x2fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC pglProgramUniformMatrix3x4fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC pglProgramUniformMatrix4x3fv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC pglProgramUniformMatrix2x3dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC pglProgramUniformMatrix3x2dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC pglProgramUniformMatrix2x4dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC pglProgramUniformMatrix4x2dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC pglProgramUniformMatrix3x4dv;
// extern GLAPI PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC pglProgramUniformMatrix4x3dv;
// extern GLAPI PFNGLVALIDATEPROGRAMPIPELINEPROC pglValidateProgramPipeline;
// extern GLAPI PFNGLGETPROGRAMPIPELINEINFOLOGPROC pglGetProgramPipelineInfoLog;



// GL_ARB_debug_output : no opengl version
////////////////////////////////////////////

extern GLAPI PFNGLDEBUGMESSAGECONTROLARBPROC pglDebugMessageControl;
// extern GLAPI PFNGLDEBUGMESSAGEINSERTARBPROC pglDebugMessageInsert;
extern GLAPI PFNGLDEBUGMESSAGECALLBACKARBPROC pglDebugMessageCallback;
extern GLAPI PFNGLGETDEBUGMESSAGELOGARBPROC pglGetDebugMessageLog;



// GL_AMD_debug_output : no opengl version
////////////////////////////////////////////

extern GLAPI PFNGLDEBUGMESSAGEENABLEAMDPROC pglDebugMessageEnableAMD;
// extern GLAPI PFNGLDEBUGMESSAGEINSERTAMDPROC pglDebugMessageInsertAMD;
extern GLAPI PFNGLDEBUGMESSAGECALLBACKAMDPROC pglDebugMessageCallbackAMD;
extern GLAPI PFNGLGETDEBUGMESSAGELOGAMDPROC pglGetDebugMessageLogAMD;



// GL_AMD_performance_monitor : no opengl version
///////////////////////////////////////////////////

extern GLAPI PFNGLGETPERFMONITORGROUPSAMDPROC pglGetPerfMonitorGroupsAMD;
extern GLAPI PFNGLGETPERFMONITORCOUNTERSAMDPROC pglGetPerfMonitorCountersAMD;
extern GLAPI PFNGLGETPERFMONITORGROUPSTRINGAMDPROC pglGetPerfMonitorGroupStringAMD;
extern GLAPI PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC pglGetPerfMonitorCounterStringAMD;
extern GLAPI PFNGLGETPERFMONITORCOUNTERINFOAMDPROC pglGetPerfMonitorCounterInfoAMD;
extern GLAPI PFNGLGENPERFMONITORSAMDPROC pglGenPerfMonitorsAMD;
extern GLAPI PFNGLDELETEPERFMONITORSAMDPROC pglDeletePerfMonitorsAMD;
extern GLAPI PFNGLSELECTPERFMONITORCOUNTERSAMDPROC pglSelectPerfMonitorCountersAMD;
extern GLAPI PFNGLBEGINPERFMONITORAMDPROC pglBeginPerfMonitorAMD;
extern GLAPI PFNGLENDPERFMONITORAMDPROC pglEndPerfMonitorAMD;
extern GLAPI PFNGLGETPERFMONITORCOUNTERDATAAMDPROC pglGetPerfMonitorCounterDataAMD;



// GL_NV_transform_feedback : no opengl version
/////////////////////////////////////////////////

// extern GLAPI PFNGLBEGINTRANSFORMFEEDBACKNVPROC pglBeginTransformFeedbackNV;
// extern GLAPI PFNGLENDTRANSFORMFEEDBACKNVPROC pglEndTransformFeedbackNV;
// extern GLAPI PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC pglTransformFeedbackAttribsNV;
// extern GLAPI PFNGLBINDBUFFERRANGENVPROC pglBindBufferRangeNV;
// extern GLAPI PFNGLBINDBUFFEROFFSETNVPROC pglBindBufferOffsetNV;
// extern GLAPI PFNGLBINDBUFFERBASENVPROC pglBindBufferBaseNV;
extern GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC pglTransformFeedbackVaryingsNV;
extern GLAPI PFNGLACTIVEVARYINGNVPROC pglActiveVaryingNV;
extern GLAPI PFNGLGETVARYINGLOCATIONNVPROC pglGetVaryingLocationNV;
// extern GLAPI PFNGLGETACTIVEVARYINGNVPROC pglGetActiveVaryingNV;
// extern GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC pglGetTransformFeedbackVaryingNV;
// extern GLAPI PFNGLTRANSFORMFEEDBACKSTREAMATTRIBSNVPROC pglTransformFeedbackStreamAttribsNV;



// GL_NV_transform_feedback2 : no opengl version
//////////////////////////////////////////////////

// extern GLAPI PFNGLBINDTRANSFORMFEEDBACKNVPROC pglBindTransformFeedbackNV;
// extern GLAPI PFNGLDELETETRANSFORMFEEDBACKSNVPROC pglDeleteTransformFeedbacksNV;
// extern GLAPI PFNGLGENTRANSFORMFEEDBACKSNVPROC pglGenTransformFeedbacksNV;
// extern GLAPI PFNGLISTRANSFORMFEEDBACKNVPROC pglIsTransformFeedbackNV;
// extern GLAPI PFNGLPAUSETRANSFORMFEEDBACKNVPROC pglPauseTransformFeedbackNV;
// extern GLAPI PFNGLRESUMETRANSFORMFEEDBACKNVPROC pglResumeTransformFeedbackNV;
// extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKNVPROC pglDrawTransformFeedbackNV;



// GL_ARB_transform_feedback3 : no opengl version
///////////////////////////////////////////////////

// extern GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC pglDrawTransformFeedbackStreamNV;
// extern GLAPI PFNGLBEGINQUERYINDEXEDPROC pglBeginQueryIndexedNV;
// extern GLAPI PFNGLENDQUERYINDEXEDPROC pglEndQueryIndexedNV;
// extern GLAPI PFNGLGETQUERYINDEXEDIVPROC pglGetQueryIndexedivNV;



// ARB_shader_image_load_store : 4.6
//////////////////////////////////////

extern GLAPI PFNGLBINDIMAGETEXTUREPROC pglBindImageTexture;


// OpenGL 4.2 : no extension
//////////////////////////////

extern GLAPI PFNGLMEMORYBARRIERPROC pglMemoryBarrier;


// OpenGL 4.3 : no extension
//////////////////////////////

extern GLAPI PFNGLINVALIDATEFRAMEBUFFERPROC pglInvalidateFramebuffer;

// OpenGL 3.2 : no extension
//////////////////////////////

extern GLAPI PFNGLGETINTEGER64VPROC pglGetInteger64v;

// OpenGL 2.0 : no extension
//////////////////////////////

extern GLAPI PFNGLMULTIDRAWARRAYSPROC pglMultiDrawArrays;
extern GLAPI PFNGLMULTIDRAWELEMENTSPROC pglMultiDrawElements;



// GL_NV_texture_barrier : no opengl version
//////////////////////////////////////////////

extern GLAPI PFNGLTEXTUREBARRIERNVPROC pglTextureBarrier;



// GL_ARB_bindless_texture : no opengl version
////////////////////////////////////////////////

extern GLAPI PFNGLGETTEXTUREHANDLEARBPROC pglGetTextureHandleARB;
extern GLAPI PFNGLGETTEXTURESAMPLERHANDLEARBPROC pglGetTextureSamplerHandleARB;
extern GLAPI PFNGLMAKETEXTUREHANDLERESIDENTARBPROC pglMakeTextureHandleResidentARB;
extern GLAPI PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC pglMakeTextureHandleNonResidentARB;
extern GLAPI PFNGLGETIMAGEHANDLEARBPROC pglGetImageHandleARB;
extern GLAPI PFNGLMAKEIMAGEHANDLERESIDENTARBPROC pglMakeImageHandleResidentARB;
extern GLAPI PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC pglMakeImageHandleNonResidentARB;
extern GLAPI PFNGLUNIFORMHANDLEUI64ARBPROC pglUniformHandleui64ARB;
extern GLAPI PFNGLUNIFORMHANDLEUI64VARBPROC pglUniformHandleui64vARB;
extern GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC pglProgramUniformHandleui64ARB;
extern GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC pglProgramUniformHandleui64vARB;
extern GLAPI PFNGLVERTEXATTRIBL1UI64ARBPROC pglVertexAttribL1ui64ARB;
extern GLAPI PFNGLVERTEXATTRIBL1UI64VARBPROC pglVertexAttribL1ui64vARB;
extern GLAPI PFNGLGETVERTEXATTRIBLUI64VARBPROC pglGetVertexAttribLui64vARB;



// GL_KHR_debug : no opengl version
/////////////////////////////////////

extern GLAPI PFNGLPUSHDEBUGGROUPPROC pglPushDebugGroup;
extern GLAPI PFNGLPOPDEBUGGROUPPROC pglPopDebugGroup;
extern GLAPI PFNGLOBJECTLABELPROC pglObjectLabel;

#endif

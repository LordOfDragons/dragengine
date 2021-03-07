/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>

#include "../deoglBasics.h"

// Regular expressions to convert opengl function declarations into our declarations
// Search: GLAPI.+(gl[A-Za-z0-9_]+).+;
// Replace: GLAPI PFN\U\1\EPROC p\1 = NULL;



// Required Extensions
////////////////////////

// GL_EXT_polygon_offset
//////////////////////////

GLAPI PFNGLPOLYGONOFFSETEXTPROC pglPolygonOffset = NULL;



// GL_EXT_texture3D : opengl version 1.2
//////////////////////////////////////////

GLAPI PFNGLTEXIMAGE3DPROC pglTexImage3D = NULL;
GLAPI PFNGLTEXSUBIMAGE3DPROC pglTexSubImage3D = NULL;



// GL_EXT_blend_color : opengl version 1.2
////////////////////////////////////////////

GLAPI PFNGLBLENDCOLOREXTPROC pglBlendColor = NULL;



// GL_EXT_blend_minmax : opengl version 1.2
/////////////////////////////////////////////

GLAPI PFNGLBLENDEQUATIONEXTPROC pglBlendEquation = NULL;



// GL_ARB_multitexture . opengl version 1.3
/////////////////////////////////////////////

GLAPI PFNGLACTIVETEXTUREPROC pglActiveTexture = NULL;



// GL_ARB_texture_compression : opengl version 1.3
////////////////////////////////////////////////////

GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC pglCompressedTexImage3D = NULL;
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC pglCompressedTexImage2D = NULL;
GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC pglCompressedTexImage1D = NULL;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC pglCompressedTexSubImage3D = NULL;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC pglCompressedTexSubImage2D = NULL;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC pglCompressedTexSubImage1D = NULL;
GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC pglGetCompressedTexImage = NULL;



// GL_ARB_vertex_buffer_object . opengl version 1.5
/////////////////////////////////////////////////////

GLAPI PFNGLBINDBUFFERPROC pglBindBuffer = NULL;
GLAPI PFNGLDELETEBUFFERSPROC pglDeleteBuffers = NULL;
GLAPI PFNGLGENBUFFERSPROC pglGenBuffers = NULL;
GLAPI PFNGLBUFFERDATAPROC pglBufferData = NULL;
GLAPI PFNGLBUFFERSUBDATAPROC pglBufferSubData = NULL;
GLAPI PFNGLGETBUFFERSUBDATAPROC pglGetBufferSubData = NULL;
GLAPI PFNGLMAPBUFFERPROC pglMapBuffer = NULL;
GLAPI PFNGLMAPBUFFERRANGEPROC pglMapBufferRange = NULL;
GLAPI PFNGLUNMAPBUFFERPROC pglUnmapBuffer = NULL;



// GL_ARB_occlusion_query . opengl version 1.5
////////////////////////////////////////////////

GLAPI PFNGLGENQUERIESPROC pglGenQueries = NULL;
GLAPI PFNGLDELETEQUERIESPROC pglDeleteQueries = NULL;
GLAPI PFNGLBEGINQUERYPROC pglBeginQuery = NULL;
GLAPI PFNGLENDQUERYPROC pglEndQuery = NULL;
GLAPI PFNGLGETQUERYOBJECTUIVPROC pglGetQueryObjectuiv = NULL;



// opengl version 2.0
///////////////////////

GLAPI PFNGLSTENCILOPSEPARATEPROC pglStencilOpSeparate = NULL;
GLAPI PFNGLSTENCILFUNCSEPARATEPROC pglStencilFuncSeparate = NULL;
GLAPI PFNGLSTENCILMASKSEPARATEPROC pglStencilMaskSeparate = NULL;



// GL_ARB_vertex_program . opengl version 2.0
///////////////////////////////////////////////

GLAPI PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer = NULL;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray = NULL;



// GL_ARB_vertex_shader : opengl version 2.0
//////////////////////////////////////////////

GLAPI PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation = NULL;



// GL_ARB_draw_buffers : opengl version 2.0
/////////////////////////////////////////////

GLAPI PFNGLDRAWBUFFERSPROC pglDrawBuffers = NULL;



// GL_ARB_shader_objects . no opengl version, most probably 2.1
/////////////////////////////////////////////////////////////////

GLAPI PFNGLDELETESHADERPROC pglDeleteShader = NULL; // core only
GLAPI PFNGLDELETEPROGRAMPROC pglDeleteProgram = NULL; // core only
GLAPI PFNGLCREATESHADERPROC pglCreateShader = NULL; // core only
GLAPI PFNGLCREATEPROGRAMPROC pglCreateProgram = NULL; // core only
GLAPI PFNGLATTACHSHADERPROC pglAttachShader = NULL; // core only
GLAPI PFNGLUSEPROGRAMPROC pglUseProgram = NULL; // core only
GLAPI PFNGLGETSHADERIVPROC pglGetShaderiv = NULL; // core only
GLAPI PFNGLGETPROGRAMIVPROC pglGetProgramiv = NULL; // core only
GLAPI PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = NULL; // core only
GLAPI PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = NULL; // core only

// GLAPI PFNGLDELETEOBJECTPROC pglDeleteObject = NULL;
// GLAPI PFNGLCREATESHADEROBJECTPROC pglCreateShaderObject = NULL;
GLAPI PFNGLSHADERSOURCEPROC pglShaderSource = NULL;
GLAPI PFNGLCOMPILESHADERPROC pglCompileShader = NULL;
// GLAPI PFNGLCREATEPROGRAMOBJECTPROC pglCreateProgramObject = NULL;
// GLAPI PFNGLATTACHOBJECTPROC pglAttachObject = NULL;
GLAPI PFNGLLINKPROGRAMPROC pglLinkProgram = NULL;
// GLAPI PFNGLUSEPROGRAMOBJECTPROC pglUseProgramObject = NULL;
GLAPI PFNGLUNIFORM1FPROC pglUniform1f = NULL;
GLAPI PFNGLUNIFORM2FPROC pglUniform2f = NULL;
GLAPI PFNGLUNIFORM3FPROC pglUniform3f = NULL;
GLAPI PFNGLUNIFORM4FPROC pglUniform4f = NULL;
GLAPI PFNGLUNIFORM1IPROC pglUniform1i = NULL;
GLAPI PFNGLUNIFORM2IPROC pglUniform2i = NULL;
GLAPI PFNGLUNIFORM3IPROC pglUniform3i = NULL;
GLAPI PFNGLUNIFORM4IPROC pglUniform4i = NULL;
GLAPI PFNGLUNIFORM1FVPROC pglUniform1fv = NULL;
GLAPI PFNGLUNIFORM2FVPROC pglUniform2fv = NULL;
GLAPI PFNGLUNIFORM3FVPROC pglUniform3fv = NULL;
GLAPI PFNGLUNIFORM4FVPROC pglUniform4fv = NULL;
GLAPI PFNGLUNIFORM1IVPROC pglUniform1iv = NULL;
GLAPI PFNGLUNIFORM2IVPROC pglUniform2iv = NULL;
GLAPI PFNGLUNIFORM3IVPROC pglUniform3iv = NULL;
GLAPI PFNGLUNIFORM4IVPROC pglUniform4iv = NULL;
GLAPI PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv = NULL;
GLAPI PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv = NULL;
GLAPI PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = NULL;
// GLAPI PFNGLGETOBJECTPARAMETERIVPROC pglGetObjectParameteriv = NULL;
// GLAPI PFNGLGETINFOLOGPROC pglGetInfoLog = NULL;
GLAPI PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = NULL;



// these are listed in opengl version 2.1 but do not look like belonging directly to an extension
///////////////////////////////////////////////////////////////////////////////////////////////////

GLAPI PFNGLUNIFORMMATRIX2X3FVPROC pglUniformMatrix2x3fv = NULL;
GLAPI PFNGLUNIFORMMATRIX3X2FVPROC pglUniformMatrix3x2fv = NULL;
GLAPI PFNGLUNIFORMMATRIX2X4FVPROC pglUniformMatrix2x4fv = NULL;
GLAPI PFNGLUNIFORMMATRIX4X2FVPROC pglUniformMatrix4x2fv = NULL;
GLAPI PFNGLUNIFORMMATRIX3X4FVPROC pglUniformMatrix3x4fv = NULL;
GLAPI PFNGLUNIFORMMATRIX4X3FVPROC pglUniformMatrix4x3fv = NULL;



// these are listed in opengl version 3.0 but do not have an extension assigned
/////////////////////////////////////////////////////////////////////////////////

GLAPI PFNGLCLEARBUFFERIVPROC pglClearBufferiv = NULL;
GLAPI PFNGLCLEARBUFFERUIVPROC pglClearBufferuiv = NULL;
GLAPI PFNGLCLEARBUFFERFVPROC pglClearBufferfv = NULL;
GLAPI PFNGLCLEARBUFFERFIPROC pglClearBufferfi = NULL;
GLAPI PFNGLVERTEXATTRIBIPOINTERPROC pglVertexAttribIPointer = NULL;



// GL_ARB_draw_elements_base_vertex : opengl version 3.2
//////////////////////////////////////////////////////////

GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC pglDrawElementsBaseVertex = NULL;
GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC pglDrawRangeElementsBaseVertex = NULL;
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC pglDrawElementsInstancedBaseVertex = NULL;



// GL_ARB_sampler_objects : opengl version 3.3
////////////////////////////////////////////////

GLAPI PFNGLGENSAMPLERSPROC pglGenSamplers = NULL;
GLAPI PFNGLDELETESAMPLERSPROC pglDeleteSamplers = NULL;
// GLAPI PFNGLISSAMPLERPROC pglIsSampler = NULL;
GLAPI PFNGLBINDSAMPLERPROC pglBindSampler = NULL;
GLAPI PFNGLSAMPLERPARAMETERIPROC pglSamplerParameteri = NULL;
// GLAPI PFNGLSAMPLERPARAMETERIVPROC pglSamplerParameteriv = NULL;
GLAPI PFNGLSAMPLERPARAMETERFPROC pglSamplerParameterf = NULL;
// GLAPI PFNGLSAMPLERPARAMETERFVPROC pglSamplerParameterfv = NULL;
// GLAPI PFNGLSAMPLERPARAMETERIIVPROC pglSamplerParameterIiv = NULL;
// GLAPI PFNGLSAMPLERPARAMETERIUIVPROC pglSamplerParameterIuiv = NULL;
// GLAPI PFNGLGETSAMPLERPARAMETERIVPROC pglGetSamplerParameteriv = NULL;
// GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC pglGetSamplerParameterIiv = NULL;
// GLAPI PFNGLGETSAMPLERPARAMETERFVPROC pglGetSamplerParameterfv = NULL;
// GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC pglGetSamplerParameterIuiv = NULL;



// GL_ARB_array_texture : opengl version 3.0
//////////////////////////////////////////////

GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC pglFramebufferTextureLayer = NULL;



// GL_ARB_framebuffer_object : no opengl version
//////////////////////////////////////////////////

GLAPI PFNGLBINDRENDERBUFFERPROC pglBindRenderbuffer = NULL;
GLAPI PFNGLDELETERENDERBUFFERSPROC pglDeleteRenderbuffers = NULL;
GLAPI PFNGLGENRENDERBUFFERSPROC pglGenRenderbuffers = NULL;
GLAPI PFNGLRENDERBUFFERSTORAGEPROC pglRenderbufferStorage = NULL;
GLAPI PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer = NULL;
GLAPI PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers = NULL;
GLAPI PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers = NULL;
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus = NULL;
GLAPI PFNGLFRAMEBUFFERTEXTURE1DPROC pglFramebufferTexture1D = NULL;
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D = NULL;
GLAPI PFNGLFRAMEBUFFERTEXTURE3DPROC pglFramebufferTexture3D = NULL;
GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC pglFramebufferRenderbuffer = NULL;
GLAPI PFNGLGENERATEMIPMAPPROC pglGenerateMipmap = NULL;
GLAPI PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer = NULL;
GLAPI PFNGLFRAMEBUFFERTEXTUREARBPROC pglFramebufferTexture = NULL;



// GL_ARB_geometry_shader4 : no opengl version
////////////////////////////////////////////////

GLAPI PFNGLFRAMEBUFFERTEXTUREFACEARBPROC pglFramebufferTextureFace = NULL;



// GL_ARB_vertex_array_object : no opengl version
///////////////////////////////////////////////////

GLAPI PFNGLBINDVERTEXARRAYPROC pglBindVertexArray = NULL;
GLAPI PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays = NULL;
GLAPI PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays = NULL;



// GL_EXT_subtexture : no opengl version
//////////////////////////////////////////

GLAPI PFNGLTEXSUBIMAGE1DEXTPROC pglTexSubImage1D = NULL;
GLAPI PFNGLTEXSUBIMAGE2DEXTPROC pglTexSubImage2D = NULL;



// GL_EXT_copy_texture : no opengl version
////////////////////////////////////////////



// GL_EXT_texture_object : no opengl version
//////////////////////////////////////////////



// Optional Extensions
////////////////////////

// GL_ARB_multisample : opengl version 1.3
////////////////////////////////////////////



// GL_EXT_transform_feedback : opengl version 3.0
///////////////////////////////////////////////////

GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC pglBeginTransformFeedback = NULL;
GLAPI PFNGLENDTRANSFORMFEEDBACKPROC pglEndTransformFeedback = NULL;
GLAPI PFNGLBINDBUFFERRANGEPROC pglBindBufferRange = NULL;
GLAPI PFNGLBINDBUFFERBASEPROC pglBindBufferBase = NULL;
GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC pglTransformFeedbackVaryings = NULL;
GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC pglGetTransformFeedbackVarying = NULL;



// GL_EXT_gpu_shader4 : opengl version 3.0
////////////////////////////////////////////

GLAPI PFNGLBINDFRAGDATALOCATIONEXTPROC pglBindFragDataLocation = NULL;



// GL_ARB_draw_instanced : opengl version 3.1
///////////////////////////////////////////////

GLAPI PFNGLDRAWARRAYSINSTANCEDARBPROC pglDrawArraysInstanced = NULL;
GLAPI PFNGLDRAWELEMENTSINSTANCEDARBPROC pglDrawElementsInstanced = NULL;



// GL_ARB_texture_buffer_object : opengl version 3.1
//////////////////////////////////////////////////////

GLAPI PFNGLTEXBUFFERARBPROC pglTexBuffer = NULL;



// GL_ARB_timer_query : opengl version 3.3
////////////////////////////////////////////

GLAPI PFNGLQUERYCOUNTERPROC pglQueryCounter = NULL;
//GLAPI PFNGLGETQUERYOBJECTI64VPROC pglGetQueryObjecti64v = NULL;
GLAPI PFNGLGETQUERYOBJECTUI64VPROC pglGetQueryObjectui64v = NULL;



// GL_ARB_draw_buffers_blend : opengl version 4.0
///////////////////////////////////////////////////



// GL_ARB_tessellation_shader : opengl version 4.0
////////////////////////////////////////////////////

GLAPI PFNGLPATCHPARAMETERIPROC pglPatchParameteri = NULL;
GLAPI PFNGLPATCHPARAMETERFVPROC pglPatchParameterfv = NULL;



// GL_ARB_transform_feedback2 : opengl version 4.0
////////////////////////////////////////////////////

GLAPI PFNGLBINDTRANSFORMFEEDBACKPROC pglBindTransformFeedback = NULL;
GLAPI PFNGLDELETETRANSFORMFEEDBACKSPROC pglDeleteTransformFeedbacks = NULL;
GLAPI PFNGLGENTRANSFORMFEEDBACKSPROC pglGenTransformFeedbacks = NULL;
GLAPI PFNGLISTRANSFORMFEEDBACKPROC pglIsTransformFeedback = NULL;
GLAPI PFNGLPAUSETRANSFORMFEEDBACKPROC pglPauseTransformFeedback = NULL;
GLAPI PFNGLRESUMETRANSFORMFEEDBACKPROC pglResumeTransformFeedback = NULL;
GLAPI PFNGLDRAWTRANSFORMFEEDBACKPROC pglDrawTransformFeedback = NULL;



// GL_ARB_transform_feedback3 : opengl version 4.0
////////////////////////////////////////////////////

GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC pglDrawTransformFeedbackStream = NULL;
GLAPI PFNGLBEGINQUERYINDEXEDPROC pglBeginQueryIndexed = NULL;
GLAPI PFNGLENDQUERYINDEXEDPROC pglEndQueryIndexed = NULL;
GLAPI PFNGLGETQUERYINDEXEDIVPROC pglGetQueryIndexediv = NULL;



// GL_ARB_viewport_array : opengl version 4.1
///////////////////////////////////////////////

GLAPI PFNGLVIEWPORTARRAYVPROC pglViewportArrayv = NULL;
GLAPI PFNGLVIEWPORTINDEXEDFPROC pglViewportIndexedf = NULL;
GLAPI PFNGLVIEWPORTINDEXEDFVPROC pglViewportIndexedfv = NULL;
GLAPI PFNGLSCISSORARRAYVPROC pglScissorArrayv = NULL;
GLAPI PFNGLSCISSORINDEXEDPROC pglScissorIndexed = NULL;
GLAPI PFNGLSCISSORINDEXEDVPROC pglScissorIndexedv = NULL;
GLAPI PFNGLDEPTHRANGEARRAYVPROC pglDepthRangeArrayv = NULL;
GLAPI PFNGLDEPTHRANGEINDEXEDPROC pglDepthRangeIndexed = NULL;
GLAPI PFNGLGETFLOATI_VPROC pglGetFloati_v = NULL;
GLAPI PFNGLGETDOUBLEI_VPROC pglGetDoublei_v = NULL;

GLAPI PFNGLENABLEINDEXEDEXTPROC pglEnableIndexed = NULL;
GLAPI PFNGLDISABLEINDEXEDEXTPROC pglDisableIndexed = NULL;
GLAPI PFNGLISENABLEDINDEXEDEXTPROC pglIsEnabledIndexed = NULL;
GLAPI PFNGLGETINTEGERINDEXEDVEXTPROC pglGetIntegerIndexedv = NULL;



// GL_ARB_transform_feedback_instanced : opengl version 4.2
/////////////////////////////////////////////////////////////

GLAPI PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC pglDrawTransformFeedbackInstanced = NULL;
GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC pglDrawTransformFeedbackStreamInstanced = NULL;



// GL_ARB_copy_image : opengl version 4.3
///////////////////////////////////////////

GLAPI PFNGLCOPYIMAGESUBDATAPROC pglCopyImageSubData = NULL;



// GL_NV_copy_image : opengl version 4.3
//////////////////////////////////////////

GLAPI PFNGLCOPYIMAGESUBDATANVPROC pglCopyImageSubDataNV = NULL;



// GL_ARB_shader_storage_buffer_object : opengl version 4.3
/////////////////////////////////////////////////////////////

GLAPI PFNGLSHADERSTORAGEBLOCKBINDINGPROC pglShaderStorageBlockBinding = NULL;



// GL_ARB_clip_control : opengl version 4.5
/////////////////////////////////////////////

GLAPI PFNGLCLIPCONTROLPROC pglClipControl = NULL;



// GL_ARB_uniform_buffer_object : no opengl version
/////////////////////////////////////////////////////

GLAPI PFNGLGETUNIFORMINDICESPROC pglGetUniformIndices = NULL;
GLAPI PFNGLGETACTIVEUNIFORMSIVPROC pglGetActiveUniformsiv = NULL;
GLAPI PFNGLGETACTIVEUNIFORMNAMEPROC pglGetActiveUniformName = NULL;
GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC pglGetUniformBlockIndex = NULL;
GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC pglGetActiveUniformBlockiv = NULL;
GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC pglGetActiveUniformBlockName = NULL;
GLAPI PFNGLUNIFORMBLOCKBINDINGPROC pglUniformBlockBinding = NULL;



// GL_ARB_copy_buffer : no opengl version
///////////////////////////////////////////



// GL_ARB_texture_multisample : no opengl version
///////////////////////////////////////////////////



// GL_ARB_get_program_binary : no opengl version
//////////////////////////////////////////////////



// GL_ARB_separate_shader_objects : no opengl version
///////////////////////////////////////////////////////



// GL_ARB_debug_output : no opengl version
////////////////////////////////////////////

GLAPI PFNGLDEBUGMESSAGECONTROLARBPROC pglDebugMessageControl = NULL;
GLAPI PFNGLDEBUGMESSAGECALLBACKARBPROC pglDebugMessageCallback = NULL;
GLAPI PFNGLGETDEBUGMESSAGELOGARBPROC pglGetDebugMessageLog = NULL;



// GL_AMD_debug_output : no opengl version
////////////////////////////////////////////

GLAPI PFNGLDEBUGMESSAGEENABLEAMDPROC pglDebugMessageEnableAMD = NULL;
GLAPI PFNGLDEBUGMESSAGECALLBACKAMDPROC pglDebugMessageCallbackAMD = NULL;
GLAPI PFNGLGETDEBUGMESSAGELOGAMDPROC pglGetDebugMessageLogAMD= NULL;



// GL_AMD_performance_monitor : no opengl version
///////////////////////////////////////////////////

GLAPI PFNGLGETPERFMONITORGROUPSAMDPROC pglGetPerfMonitorGroupsAMD = NULL;
GLAPI PFNGLGETPERFMONITORCOUNTERSAMDPROC pglGetPerfMonitorCountersAMD = NULL;
GLAPI PFNGLGETPERFMONITORGROUPSTRINGAMDPROC pglGetPerfMonitorGroupStringAMD = NULL;
GLAPI PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC pglGetPerfMonitorCounterStringAMD = NULL;
GLAPI PFNGLGETPERFMONITORCOUNTERINFOAMDPROC pglGetPerfMonitorCounterInfoAMD = NULL;
GLAPI PFNGLGENPERFMONITORSAMDPROC pglGenPerfMonitorsAMD = NULL;
GLAPI PFNGLDELETEPERFMONITORSAMDPROC pglDeletePerfMonitorsAMD = NULL;
GLAPI PFNGLSELECTPERFMONITORCOUNTERSAMDPROC pglSelectPerfMonitorCountersAMD = NULL;
GLAPI PFNGLBEGINPERFMONITORAMDPROC pglBeginPerfMonitorAMD = NULL;
GLAPI PFNGLENDPERFMONITORAMDPROC pglEndPerfMonitorAMD = NULL;
GLAPI PFNGLGETPERFMONITORCOUNTERDATAAMDPROC pglGetPerfMonitorCounterDataAMD = NULL;



// GL_NV_transform_feedback : no opengl version
/////////////////////////////////////////////////

GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC pglTransformFeedbackVaryingsNV = NULL;
GLAPI PFNGLACTIVEVARYINGNVPROC pglActiveVaryingNV = NULL;
GLAPI PFNGLGETVARYINGLOCATIONNVPROC pglGetVaryingLocationNV = NULL;



// GL_NV_transform_feedback2 : no opengl version
//////////////////////////////////////////////////



// ARB_shader_image_load_store : 4.6
//////////////////////////////////////

GLAPI PFNGLBINDIMAGETEXTUREPROC pglBindImageTexture = NULL;



// OpenGL 4.3 : no extension
//////////////////////////////

GLAPI PFNGLINVALIDATEFRAMEBUFFERPROC pglInvalidateFramebuffer = NULL;

// OpenGL 3.2 : no extension
//////////////////////////////

GLAPI PFNGLGETINTEGER64VPROC pglGetInteger64v = NULL;



// GL_NV_texture_barrier : no opengl version
//////////////////////////////////////////////

GLAPI PFNGLTEXTUREBARRIERNVPROC pglTextureBarrier = NULL;

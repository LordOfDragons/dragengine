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

#include <stdlib.h>

#include "../deoglBasics.h"

// Regular expressions to convert opengl function declarations into our declarations
// Search: GLAPI.+(gl[A-Za-z0-9_]+).+;
// Replace: GLAPI PFN\U\1\EPROC p\1 = nullptr;



// Required Extensions
////////////////////////

// GL_EXT_polygon_offset
//////////////////////////

GLAPI PFNGLPOLYGONOFFSETEXTPROC pglPolygonOffset = nullptr;



// GL_EXT_texture3D : opengl version 1.2
//////////////////////////////////////////

GLAPI PFNGLTEXIMAGE3DPROC pglTexImage3D = nullptr;
GLAPI PFNGLTEXSUBIMAGE3DPROC pglTexSubImage3D = nullptr;



// GL_EXT_blend_color : opengl version 1.2
////////////////////////////////////////////

GLAPI PFNGLBLENDCOLOREXTPROC pglBlendColor = nullptr;



// GL_EXT_blend_minmax : opengl version 1.2
/////////////////////////////////////////////

GLAPI PFNGLBLENDEQUATIONEXTPROC pglBlendEquation = nullptr;



// GL_ARB_multitexture . opengl version 1.3
/////////////////////////////////////////////

GLAPI PFNGLACTIVETEXTUREPROC pglActiveTexture = nullptr;



// GL_ARB_texture_compression : opengl version 1.3
////////////////////////////////////////////////////

GLAPI PFNGLCOMPRESSEDTEXIMAGE3DPROC pglCompressedTexImage3D = nullptr;
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC pglCompressedTexImage2D = nullptr;
// GLAPI PFNGLCOMPRESSEDTEXIMAGE1DPROC pglCompressedTexImage1D = nullptr;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC pglCompressedTexSubImage3D = nullptr;
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC pglCompressedTexSubImage2D = nullptr;
// GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC pglCompressedTexSubImage1D = nullptr;
// GLAPI PFNGLGETCOMPRESSEDTEXIMAGEPROC pglGetCompressedTexImage = nullptr;



// GL_ARB_vertex_buffer_object . opengl version 1.5
/////////////////////////////////////////////////////

GLAPI PFNGLBINDBUFFERPROC pglBindBuffer = nullptr;
GLAPI PFNGLDELETEBUFFERSPROC pglDeleteBuffers = nullptr;
GLAPI PFNGLGENBUFFERSPROC pglGenBuffers = nullptr;
GLAPI PFNGLBUFFERDATAPROC pglBufferData = nullptr;
GLAPI PFNGLBUFFERSUBDATAPROC pglBufferSubData = nullptr;
// GLAPI PFNGLGETBUFFERSUBDATAPROC pglGetBufferSubData = nullptr;
GLAPI PFNGLMAPBUFFERPROC pglMapBuffer = nullptr;
GLAPI PFNGLMAPBUFFERRANGEPROC pglMapBufferRange = nullptr;
GLAPI PFNGLUNMAPBUFFERPROC pglUnmapBuffer = nullptr;



// GL_ARB_occlusion_query . opengl version 1.5
////////////////////////////////////////////////

GLAPI PFNGLGENQUERIESPROC pglGenQueries = nullptr;
GLAPI PFNGLDELETEQUERIESPROC pglDeleteQueries = nullptr;
GLAPI PFNGLBEGINQUERYPROC pglBeginQuery = nullptr;
GLAPI PFNGLENDQUERYPROC pglEndQuery = nullptr;
GLAPI PFNGLGETQUERYOBJECTUIVPROC pglGetQueryObjectuiv = nullptr;



// opengl version 2.0
///////////////////////

GLAPI PFNGLSTENCILOPSEPARATEPROC pglStencilOpSeparate = nullptr;
// GLAPI PFNGLSTENCILFUNCSEPARATEPROC pglStencilFuncSeparate = nullptr;
// GLAPI PFNGLSTENCILMASKSEPARATEPROC pglStencilMaskSeparate = nullptr;



// GL_ARB_vertex_program . opengl version 2.0
///////////////////////////////////////////////

GLAPI PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer = nullptr;
GLAPI PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray = nullptr;



// GL_ARB_vertex_shader : opengl version 2.0
//////////////////////////////////////////////

GLAPI PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation = nullptr;



// GL_ARB_draw_buffers : opengl version 2.0
/////////////////////////////////////////////

GLAPI PFNGLDRAWBUFFERSPROC pglDrawBuffers = nullptr;



// GL_ARB_shader_objects . no opengl version, most probably 2.1
/////////////////////////////////////////////////////////////////

GLAPI PFNGLDELETESHADERPROC pglDeleteShader = nullptr; // core only
GLAPI PFNGLDELETEPROGRAMPROC pglDeleteProgram = nullptr; // core only
GLAPI PFNGLCREATESHADERPROC pglCreateShader = nullptr; // core only
GLAPI PFNGLCREATEPROGRAMPROC pglCreateProgram = nullptr; // core only
GLAPI PFNGLATTACHSHADERPROC pglAttachShader = nullptr; // core only
GLAPI PFNGLUSEPROGRAMPROC pglUseProgram = nullptr; // core only
GLAPI PFNGLGETSHADERIVPROC pglGetShaderiv = nullptr; // core only
GLAPI PFNGLGETPROGRAMIVPROC pglGetProgramiv = nullptr; // core only
GLAPI PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = nullptr; // core only
GLAPI PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = nullptr; // core only

// GLAPI PFNGLDELETEOBJECTPROC pglDeleteObject = nullptr;
// GLAPI PFNGLCREATESHADEROBJECTPROC pglCreateShaderObject = nullptr;
GLAPI PFNGLSHADERSOURCEPROC pglShaderSource = nullptr;
GLAPI PFNGLCOMPILESHADERPROC pglCompileShader = nullptr;
// GLAPI PFNGLCREATEPROGRAMOBJECTPROC pglCreateProgramObject = nullptr;
// GLAPI PFNGLATTACHOBJECTPROC pglAttachObject = nullptr;
GLAPI PFNGLLINKPROGRAMPROC pglLinkProgram = nullptr;
// GLAPI PFNGLUSEPROGRAMOBJECTPROC pglUseProgramObject = nullptr;
GLAPI PFNGLUNIFORM1FPROC pglUniform1f = nullptr;
GLAPI PFNGLUNIFORM2FPROC pglUniform2f = nullptr;
GLAPI PFNGLUNIFORM3FPROC pglUniform3f = nullptr;
GLAPI PFNGLUNIFORM4FPROC pglUniform4f = nullptr;
GLAPI PFNGLUNIFORM1IPROC pglUniform1i = nullptr;
GLAPI PFNGLUNIFORM2IPROC pglUniform2i = nullptr;
GLAPI PFNGLUNIFORM3IPROC pglUniform3i = nullptr;
GLAPI PFNGLUNIFORM4IPROC pglUniform4i = nullptr;
// GLAPI PFNGLUNIFORM1FVPROC pglUniform1fv = nullptr;
// GLAPI PFNGLUNIFORM2FVPROC pglUniform2fv = nullptr;
// GLAPI PFNGLUNIFORM3FVPROC pglUniform3fv = nullptr;
// GLAPI PFNGLUNIFORM4FVPROC pglUniform4fv = nullptr;
// GLAPI PFNGLUNIFORM1IVPROC pglUniform1iv = nullptr;
// GLAPI PFNGLUNIFORM2IVPROC pglUniform2iv = nullptr;
// GLAPI PFNGLUNIFORM3IVPROC pglUniform3iv = nullptr;
// GLAPI PFNGLUNIFORM4IVPROC pglUniform4iv = nullptr;
// GLAPI PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv = nullptr;
GLAPI PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv = nullptr;
GLAPI PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = nullptr;
// GLAPI PFNGLGETOBJECTPARAMETERIVPROC pglGetObjectParameteriv = nullptr;
// GLAPI PFNGLGETINFOLOGPROC pglGetInfoLog = nullptr;
GLAPI PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = nullptr;



// these are listed in opengl version 2.1 but do not look like belonging directly to an extension
///////////////////////////////////////////////////////////////////////////////////////////////////

// GLAPI PFNGLUNIFORMMATRIX2X3FVPROC pglUniformMatrix2x3fv = nullptr;
GLAPI PFNGLUNIFORMMATRIX3X2FVPROC pglUniformMatrix3x2fv = nullptr;
// GLAPI PFNGLUNIFORMMATRIX2X4FVPROC pglUniformMatrix2x4fv = nullptr;
// GLAPI PFNGLUNIFORMMATRIX4X2FVPROC pglUniformMatrix4x2fv = nullptr;
// GLAPI PFNGLUNIFORMMATRIX3X4FVPROC pglUniformMatrix3x4fv = nullptr;
GLAPI PFNGLUNIFORMMATRIX4X3FVPROC pglUniformMatrix4x3fv = nullptr;



// these are listed in opengl version 3.0 but do not have an extension assigned
/////////////////////////////////////////////////////////////////////////////////

GLAPI PFNGLCLEARBUFFERIVPROC pglClearBufferiv = nullptr;
// GLAPI PFNGLCLEARBUFFERUIVPROC pglClearBufferuiv = nullptr;
GLAPI PFNGLCLEARBUFFERFVPROC pglClearBufferfv = nullptr;
GLAPI PFNGLCLEARBUFFERFIPROC pglClearBufferfi = nullptr;
GLAPI PFNGLVERTEXATTRIBIPOINTERPROC pglVertexAttribIPointer = nullptr;



// OpenGL 3.2 core stuff
//////////////////////////

GLAPI PFNGLFENCESYNCPROC pglFenceSync = nullptr;
GLAPI PFNGLDELETESYNCPROC pglDeleteSync = nullptr;
GLAPI PFNGLCLIENTWAITSYNCPROC pglClientWaitSync = nullptr;
// GLAPI PFNGLWAITSYNCPROC pglWaitSync = nullptr;



// GL_ARB_draw_elements_base_vertex : opengl version 3.2
//////////////////////////////////////////////////////////

GLAPI PFNGLDRAWELEMENTSBASEVERTEXPROC pglDrawElementsBaseVertex = nullptr;
// GLAPI PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC pglDrawRangeElementsBaseVertex = nullptr;
GLAPI PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC pglMultiDrawElementsBaseVertex = nullptr;
GLAPI PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC pglDrawElementsInstancedBaseVertex = nullptr;



// GL_ARB_sampler_objects : opengl version 3.3
////////////////////////////////////////////////

GLAPI PFNGLGENSAMPLERSPROC pglGenSamplers = nullptr;
GLAPI PFNGLDELETESAMPLERSPROC pglDeleteSamplers = nullptr;
// GLAPI PFNGLISSAMPLERPROC pglIsSampler = nullptr;
GLAPI PFNGLBINDSAMPLERPROC pglBindSampler = nullptr;
GLAPI PFNGLSAMPLERPARAMETERIPROC pglSamplerParameteri = nullptr;
// GLAPI PFNGLSAMPLERPARAMETERIVPROC pglSamplerParameteriv = nullptr;
// GLAPI PFNGLSAMPLERPARAMETERFPROC pglSamplerParameterf = nullptr;
// GLAPI PFNGLSAMPLERPARAMETERFVPROC pglSamplerParameterfv = nullptr;
// GLAPI PFNGLSAMPLERPARAMETERIIVPROC pglSamplerParameterIiv = nullptr;
// GLAPI PFNGLSAMPLERPARAMETERIUIVPROC pglSamplerParameterIuiv = nullptr;
// GLAPI PFNGLGETSAMPLERPARAMETERIVPROC pglGetSamplerParameteriv = nullptr;
// GLAPI PFNGLGETSAMPLERPARAMETERIIVPROC pglGetSamplerParameterIiv = nullptr;
// GLAPI PFNGLGETSAMPLERPARAMETERFVPROC pglGetSamplerParameterfv = nullptr;
// GLAPI PFNGLGETSAMPLERPARAMETERIUIVPROC pglGetSamplerParameterIuiv = nullptr;



// OpenGL 3.0
///////////////

GLAPI PFNGLGETINTEGERI_VPROC pglGetIntegeri_v = nullptr;



// GL_ARB_array_texture : opengl version 3.0
//////////////////////////////////////////////

GLAPI PFNGLFRAMEBUFFERTEXTURELAYERPROC pglFramebufferTextureLayer = nullptr;



// GL_ARB_framebuffer_object : no opengl version
//////////////////////////////////////////////////

// GLAPI PFNGLBINDRENDERBUFFERPROC pglBindRenderbuffer = nullptr;
// GLAPI PFNGLDELETERENDERBUFFERSPROC pglDeleteRenderbuffers = nullptr;
// GLAPI PFNGLGENRENDERBUFFERSPROC pglGenRenderbuffers = nullptr;
// GLAPI PFNGLRENDERBUFFERSTORAGEPROC pglRenderbufferStorage = nullptr;
GLAPI PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer = nullptr;
GLAPI PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers = nullptr;
GLAPI PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers = nullptr;
GLAPI PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus = nullptr;
GLAPI PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D = nullptr;
// GLAPI PFNGLFRAMEBUFFERTEXTURE3DPROC pglFramebufferTexture3D = nullptr;
// GLAPI PFNGLFRAMEBUFFERRENDERBUFFERPROC pglFramebufferRenderbuffer = nullptr;
GLAPI PFNGLGENERATEMIPMAPPROC pglGenerateMipmap = nullptr;
GLAPI PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer = nullptr;
GLAPI PFNGLFRAMEBUFFERTEXTUREARBPROC pglFramebufferTexture = nullptr;



// GL_ARB_geometry_shader4 : no opengl version
////////////////////////////////////////////////

GLAPI PFNGLFRAMEBUFFERTEXTUREFACEARBPROC pglFramebufferTextureFace = nullptr;



// GL_ARB_vertex_array_object : no opengl version
///////////////////////////////////////////////////

GLAPI PFNGLBINDVERTEXARRAYPROC pglBindVertexArray = nullptr;
GLAPI PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays = nullptr;
GLAPI PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays = nullptr;



// GL_EXT_subtexture : no opengl version
//////////////////////////////////////////

// GLAPI PFNGLTEXSUBIMAGE1DEXTPROC pglTexSubImage1D = nullptr;
GLAPI PFNGLTEXSUBIMAGE2DEXTPROC pglTexSubImage2D = nullptr;



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

// GLAPI PFNGLBEGINTRANSFORMFEEDBACKPROC pglBeginTransformFeedback = nullptr;
// GLAPI PFNGLENDTRANSFORMFEEDBACKPROC pglEndTransformFeedback = nullptr;
GLAPI PFNGLBINDBUFFERRANGEPROC pglBindBufferRange = nullptr;
GLAPI PFNGLBINDBUFFERBASEPROC pglBindBufferBase = nullptr;
GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSPROC pglTransformFeedbackVaryings = nullptr;
// GLAPI PFNGLGETTRANSFORMFEEDBACKVARYINGPROC pglGetTransformFeedbackVarying = nullptr;



// GL_EXT_gpu_shader4 : opengl version 3.0
////////////////////////////////////////////

GLAPI PFNGLBINDFRAGDATALOCATIONEXTPROC pglBindFragDataLocation = nullptr;
GLAPI PFNGLUNIFORM1UIEXTPROC pglUniform1ui = nullptr;
GLAPI PFNGLUNIFORM2UIEXTPROC pglUniform2ui = nullptr;
GLAPI PFNGLUNIFORM3UIEXTPROC pglUniform3ui = nullptr;
GLAPI PFNGLUNIFORM4UIEXTPROC pglUniform4ui = nullptr;
// GLAPI PFNGLUNIFORM1UIVEXTPROC pglUniform1uiv = nullptr;
// GLAPI PFNGLUNIFORM2UIVEXTPROC pglUniform2uiv = nullptr;
// GLAPI PFNGLUNIFORM3UIVEXTPROC pglUniform3uiv = nullptr;
// GLAPI PFNGLUNIFORM4UIVEXTPROC pglUniform4uiv = nullptr;



// GL_ARB_draw_instanced : opengl version 3.1
///////////////////////////////////////////////

GLAPI PFNGLDRAWARRAYSINSTANCEDARBPROC pglDrawArraysInstanced = nullptr;
GLAPI PFNGLDRAWELEMENTSINSTANCEDARBPROC pglDrawElementsInstanced = nullptr;



// GL_ARB_texture_buffer_object : opengl version 3.1
//////////////////////////////////////////////////////

GLAPI PFNGLTEXBUFFERARBPROC pglTexBuffer = nullptr;



// GL_ARB_timer_query : opengl version 3.3
////////////////////////////////////////////

// GLAPI PFNGLQUERYCOUNTERPROC pglQueryCounter = nullptr;
// GLAPI PFNGLGETQUERYOBJECTI64VPROC pglGetQueryObjecti64v = nullptr;
// GLAPI PFNGLGETQUERYOBJECTUI64VPROC pglGetQueryObjectui64v = nullptr;



// GL_ARB_draw_buffers_blend : opengl version 4.0
///////////////////////////////////////////////////

// OpenGL version 1.4
///////////////////////

// GLAPI PFNGLBLENDFUNCSEPARATEPROC pglBlendFuncSeparate = nullptr;



// GL_ARB_tessellation_shader : opengl version 4.0
////////////////////////////////////////////////////

GLAPI PFNGLPATCHPARAMETERIPROC pglPatchParameteri = nullptr;
// GLAPI PFNGLPATCHPARAMETERFVPROC pglPatchParameterfv = nullptr;



// GL_ARB_transform_feedback2 : opengl version 4.0
////////////////////////////////////////////////////

/*
GLAPI PFNGLBINDTRANSFORMFEEDBACKPROC pglBindTransformFeedback = nullptr;
GLAPI PFNGLDELETETRANSFORMFEEDBACKSPROC pglDeleteTransformFeedbacks = nullptr;
GLAPI PFNGLGENTRANSFORMFEEDBACKSPROC pglGenTransformFeedbacks = nullptr;
GLAPI PFNGLISTRANSFORMFEEDBACKPROC pglIsTransformFeedback = nullptr;
GLAPI PFNGLPAUSETRANSFORMFEEDBACKPROC pglPauseTransformFeedback = nullptr;
GLAPI PFNGLRESUMETRANSFORMFEEDBACKPROC pglResumeTransformFeedback = nullptr;
GLAPI PFNGLDRAWTRANSFORMFEEDBACKPROC pglDrawTransformFeedback = nullptr;
*/


// GL_ARB_transform_feedback3 : opengl version 4.0
////////////////////////////////////////////////////

/*
GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC pglDrawTransformFeedbackStream = nullptr;
GLAPI PFNGLBEGINQUERYINDEXEDPROC pglBeginQueryIndexed = nullptr;
GLAPI PFNGLENDQUERYINDEXEDPROC pglEndQueryIndexed = nullptr;
GLAPI PFNGLGETQUERYINDEXEDIVPROC pglGetQueryIndexediv = nullptr;
*/


// GL_ARB_viewport_array : opengl version 4.1
///////////////////////////////////////////////

/*
GLAPI PFNGLVIEWPORTARRAYVPROC pglViewportArrayv = nullptr;
GLAPI PFNGLVIEWPORTINDEXEDFPROC pglViewportIndexedf = nullptr;
GLAPI PFNGLVIEWPORTINDEXEDFVPROC pglViewportIndexedfv = nullptr;
GLAPI PFNGLSCISSORARRAYVPROC pglScissorArrayv = nullptr;
GLAPI PFNGLSCISSORINDEXEDPROC pglScissorIndexed = nullptr;
GLAPI PFNGLSCISSORINDEXEDVPROC pglScissorIndexedv = nullptr;
GLAPI PFNGLDEPTHRANGEARRAYVPROC pglDepthRangeArrayv = nullptr;
GLAPI PFNGLDEPTHRANGEINDEXEDPROC pglDepthRangeIndexed = nullptr;
GLAPI PFNGLGETFLOATI_VPROC pglGetFloati_v = nullptr;
GLAPI PFNGLGETDOUBLEI_VPROC pglGetDoublei_v = nullptr;

GLAPI PFNGLENABLEINDEXEDEXTPROC pglEnableIndexed = nullptr;
GLAPI PFNGLDISABLEINDEXEDEXTPROC pglDisableIndexed = nullptr;
GLAPI PFNGLISENABLEDINDEXEDEXTPROC pglIsEnabledIndexed = nullptr;
GLAPI PFNGLGETINTEGERINDEXEDVEXTPROC pglGetIntegerIndexedv = nullptr;
*/


// GL_ARB_transform_feedback_instanced : opengl version 4.2
/////////////////////////////////////////////////////////////

/*
GLAPI PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC pglDrawTransformFeedbackInstanced = nullptr;
GLAPI PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC pglDrawTransformFeedbackStreamInstanced = nullptr;
*/


// GL_ARB_copy_image : opengl version 4.3
///////////////////////////////////////////

GLAPI PFNGLCOPYIMAGESUBDATAPROC pglCopyImageSubData = nullptr;



// GL_NV_copy_image : opengl version 4.3
//////////////////////////////////////////

GLAPI PFNGLCOPYIMAGESUBDATANVPROC pglCopyImageSubDataNV = nullptr;



// GL_ARB_shader_storage_buffer_object : opengl version 4.3
/////////////////////////////////////////////////////////////

GLAPI PFNGLSHADERSTORAGEBLOCKBINDINGPROC pglShaderStorageBlockBinding = nullptr;



// GL_ARB_program_interface_query : opengl version 4.3
/////////////////////////////////////////////////////////////

GLAPI PFNGLGETPROGRAMINTERFACEIVPROC pglGetProgramInterfaceiv = nullptr;
GLAPI PFNGLGETPROGRAMRESOURCEINDEXPROC pglGetProgramResourceIndex = nullptr;



// GL_ARB_compute_shader : opengl version 4.3
///////////////////////////////////////////////

GLAPI PFNGLDISPATCHCOMPUTEPROC pglDispatchCompute = nullptr;
GLAPI PFNGLDISPATCHCOMPUTEINDIRECTPROC pglDispatchComputeIndirect = nullptr;



// GL_ARB_draw_indirect : opengl version 4.3
//////////////////////////////////////////////

oglDrawArraysIndirectCommand::oglDrawArraysIndirectCommand() :
count( 0 ), instanceCount( 0 ), first( 0 ), baseInstance( 0 ){
}

oglDrawElementsIndirectCommand::oglDrawElementsIndirectCommand() :
count( 0 ), instanceCount( 0 ), firstIndex( 0 ), baseVertex( 0 ), baseInstance( 0 ){
}

GLAPI PFNGLDRAWARRAYSINDIRECTPROC pglDrawArraysIndirect = nullptr;
GLAPI PFNGLDRAWELEMENTSINDIRECTPROC pglDrawElementsIndirect = nullptr;



// GL_ARB_multi_draw_indirect : opengl version 4.3
////////////////////////////////////////////////////

GLAPI PFNGLMULTIDRAWARRAYSINDIRECTPROC pglMultiDrawArraysIndirect = nullptr;
GLAPI PFNGLMULTIDRAWELEMENTSINDIRECTPROC pglMultiDrawElementsIndirect = nullptr;



// GL_ARB_clear_buffer_data : opengl version 4.3
//////////////////////////////////////////////////

GLAPI PFNGLCLEARBUFFERSUBDATAPROC pglClearBufferSubData = nullptr;



// GL_ARB_buffer_storage : opengl version 4.3
///////////////////////////////////////////////

// GLAPI PFNGLBUFFERSTORAGEPROC pglBufferStorage = nullptr;



// GL_ARB_direct_state_access : opengl version 4.4
////////////////////////////////////////////////////

GLAPI PFNGLCREATEBUFFERSPROC pglCreateBuffers = nullptr;
GLAPI PFNGLNAMEDBUFFERSTORAGEPROC pglNamedBufferStorage = nullptr;
GLAPI PFNGLNAMEDBUFFERDATAPROC pglNamedBufferData = nullptr;
GLAPI PFNGLNAMEDBUFFERSUBDATAPROC pglNamedBufferSubData = nullptr;
GLAPI PFNGLCOPYNAMEDBUFFERSUBDATAPROC pglCopyNamedBufferSubData = nullptr;
GLAPI PFNGLCLEARNAMEDBUFFERDATAPROC pglClearNamedBufferData = nullptr;
GLAPI PFNGLCLEARNAMEDBUFFERSUBDATAPROC pglClearNamedBufferSubData = nullptr;
GLAPI PFNGLMAPNAMEDBUFFERPROC pglMapNamedBuffer = nullptr;
GLAPI PFNGLMAPNAMEDBUFFERRANGEPROC pglMapNamedBufferRange = nullptr;
GLAPI PFNGLUNMAPNAMEDBUFFERPROC pglUnmapNamedBuffer = nullptr;
GLAPI PFNGLCREATEFRAMEBUFFERSPROC pglCreateFramebuffers = nullptr;
GLAPI PFNGLNAMEDFRAMEBUFFERTEXTUREPROC pglNamedFramebufferTexture = nullptr;
GLAPI PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC pglNamedFramebufferTextureLayer = nullptr;
GLAPI PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC pglNamedFramebufferDrawBuffers = nullptr;
GLAPI PFNGLCLEARNAMEDFRAMEBUFFERFVPROC pglClearNamedFramebufferfv = nullptr;
GLAPI PFNGLCLEARNAMEDFRAMEBUFFERFIPROC pglClearNamedFramebufferfi = nullptr;
GLAPI PFNGLBLITNAMEDFRAMEBUFFERPROC pglBlitNamedFramebuffer = nullptr;
GLAPI PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC pglCheckNamedFramebufferStatus = nullptr;
GLAPI PFNGLCREATETEXTURESPROC pglCreateTextures = nullptr;
GLAPI PFNGLTEXTUREBUFFERPROC pglTextureBuffer = nullptr;
GLAPI PFNGLTEXTUREBUFFERRANGEPROC pglTextureBufferRange = nullptr;
GLAPI PFNGLTEXTURESTORAGE2DPROC pglTextureStorage2D = nullptr;
GLAPI PFNGLTEXTURESTORAGE3DPROC pglTextureStorage3D = nullptr;
GLAPI PFNGLTEXTURESUBIMAGE2DPROC pglTextureSubImage2D = nullptr;
GLAPI PFNGLTEXTURESUBIMAGE3DPROC pglTextureSubImage3D = nullptr;
GLAPI PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC pglCompressedTextureSubImage2D = nullptr;
GLAPI PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC pglCompressedTextureSubImage3D = nullptr;
GLAPI PFNGLCOPYTEXTURESUBIMAGE2DPROC pglCopyTextureSubImage2D = nullptr;
GLAPI PFNGLCOPYTEXTURESUBIMAGE3DPROC pglCopyTextureSubImage3D = nullptr;
GLAPI PFNGLTEXTUREPARAMETERIPROC pglTextureParameteri = nullptr;
GLAPI PFNGLGENERATETEXTUREMIPMAPPROC pglGenerateTextureMipmap = nullptr;
GLAPI PFNGLBINDTEXTUREUNITPROC pglBindTextureUnit = nullptr;
GLAPI PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC pglGetCompressedTextureImage = nullptr;
GLAPI PFNGLGETTEXTURELEVELPARAMETERIVPROC pglGetTextureLevelParameteriv = nullptr;
GLAPI PFNGLCREATEVERTEXARRAYSPROC pglCreateVertexArrays = nullptr;
GLAPI PFNGLENABLEVERTEXARRAYATTRIBPROC pglEnableVertexArrayAttrib = nullptr;
GLAPI PFNGLVERTEXARRAYELEMENTBUFFERPROC pglVertexArrayElementBuffer = nullptr;
GLAPI PFNGLVERTEXARRAYVERTEXBUFFERSPROC pglVertexArrayVertexBuffers = nullptr;
GLAPI PFNGLVERTEXARRAYATTRIBBINDINGPROC pglVertexArrayAttribBinding = nullptr;
GLAPI PFNGLVERTEXARRAYBINDINGDIVISORPROC pglVertexArrayBindingDivisor = nullptr;



// GL_ARB_clip_control : opengl version 4.5
/////////////////////////////////////////////

GLAPI PFNGLCLIPCONTROLPROC pglClipControl = nullptr;



// GL_ARB_indirect_parameters : opengl version 4.6
////////////////////////////////////////////////////

GLAPI PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC pglMultiDrawArraysIndirectCountARB = nullptr;
GLAPI PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC pglMultiDrawElementsIndirectCountARB = nullptr;



// GL_ARB_uniform_buffer_object : no opengl version
/////////////////////////////////////////////////////

// GLAPI PFNGLGETUNIFORMINDICESPROC pglGetUniformIndices = nullptr;
// GLAPI PFNGLGETACTIVEUNIFORMSIVPROC pglGetActiveUniformsiv = nullptr;
// GLAPI PFNGLGETACTIVEUNIFORMNAMEPROC pglGetActiveUniformName = nullptr;
GLAPI PFNGLGETUNIFORMBLOCKINDEXPROC pglGetUniformBlockIndex = nullptr;
// GLAPI PFNGLGETACTIVEUNIFORMBLOCKIVPROC pglGetActiveUniformBlockiv = nullptr;
// GLAPI PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC pglGetActiveUniformBlockName = nullptr;
GLAPI PFNGLUNIFORMBLOCKBINDINGPROC pglUniformBlockBinding = nullptr;



// GL_ARB_copy_buffer : no opengl version
///////////////////////////////////////////

GLAPI PFNGLCOPYBUFFERSUBDATAPROC pglCopyBufferSubData = nullptr;



// GL_ARB_texture_multisample : no opengl version
///////////////////////////////////////////////////



// GL_ARB_get_program_binary : no opengl version
//////////////////////////////////////////////////

GLAPI PFNGLGETPROGRAMBINARYPROC pglGetProgramBinary = nullptr;
GLAPI PFNGLPROGRAMBINARYPROC pglProgramBinary = nullptr;
// GLAPI PFNGLPROGRAMPARAMETERIPROC pglProgramParameteri = nullptr;


// GL_ARB_separate_shader_objects : no opengl version
///////////////////////////////////////////////////////



// GL_ARB_debug_output : no opengl version
////////////////////////////////////////////

GLAPI PFNGLDEBUGMESSAGECONTROLARBPROC pglDebugMessageControl = nullptr;
GLAPI PFNGLDEBUGMESSAGECALLBACKARBPROC pglDebugMessageCallback = nullptr;
GLAPI PFNGLGETDEBUGMESSAGELOGARBPROC pglGetDebugMessageLog = nullptr;



// GL_AMD_debug_output : no opengl version
////////////////////////////////////////////

GLAPI PFNGLDEBUGMESSAGEENABLEAMDPROC pglDebugMessageEnableAMD = nullptr;
GLAPI PFNGLDEBUGMESSAGECALLBACKAMDPROC pglDebugMessageCallbackAMD = nullptr;
GLAPI PFNGLGETDEBUGMESSAGELOGAMDPROC pglGetDebugMessageLogAMD= NULL;



// GL_AMD_performance_monitor : no opengl version
///////////////////////////////////////////////////

GLAPI PFNGLGETPERFMONITORGROUPSAMDPROC pglGetPerfMonitorGroupsAMD = nullptr;
GLAPI PFNGLGETPERFMONITORCOUNTERSAMDPROC pglGetPerfMonitorCountersAMD = nullptr;
GLAPI PFNGLGETPERFMONITORGROUPSTRINGAMDPROC pglGetPerfMonitorGroupStringAMD = nullptr;
GLAPI PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC pglGetPerfMonitorCounterStringAMD = nullptr;
GLAPI PFNGLGETPERFMONITORCOUNTERINFOAMDPROC pglGetPerfMonitorCounterInfoAMD = nullptr;
GLAPI PFNGLGENPERFMONITORSAMDPROC pglGenPerfMonitorsAMD = nullptr;
GLAPI PFNGLDELETEPERFMONITORSAMDPROC pglDeletePerfMonitorsAMD = nullptr;
GLAPI PFNGLSELECTPERFMONITORCOUNTERSAMDPROC pglSelectPerfMonitorCountersAMD = nullptr;
GLAPI PFNGLBEGINPERFMONITORAMDPROC pglBeginPerfMonitorAMD = nullptr;
GLAPI PFNGLENDPERFMONITORAMDPROC pglEndPerfMonitorAMD = nullptr;
GLAPI PFNGLGETPERFMONITORCOUNTERDATAAMDPROC pglGetPerfMonitorCounterDataAMD = nullptr;



// GL_NV_transform_feedback : no opengl version
/////////////////////////////////////////////////

GLAPI PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC pglTransformFeedbackVaryingsNV = nullptr;
GLAPI PFNGLACTIVEVARYINGNVPROC pglActiveVaryingNV = nullptr;
GLAPI PFNGLGETVARYINGLOCATIONNVPROC pglGetVaryingLocationNV = nullptr;



// GL_NV_transform_feedback2 : no opengl version
//////////////////////////////////////////////////



// ARB_shader_image_load_store : 4.6
//////////////////////////////////////

GLAPI PFNGLBINDIMAGETEXTUREPROC pglBindImageTexture = nullptr;


// OpenGL 4.2 : no extension
//////////////////////////////

GLAPI PFNGLMEMORYBARRIERPROC pglMemoryBarrier = nullptr;



// OpenGL 4.3 : no extension
//////////////////////////////

GLAPI PFNGLINVALIDATEFRAMEBUFFERPROC pglInvalidateFramebuffer = nullptr;

// OpenGL 3.2 : no extension
//////////////////////////////

GLAPI PFNGLGETINTEGER64VPROC pglGetInteger64v = nullptr;

// OpenGL 2.0 : no extension
//////////////////////////////

GLAPI PFNGLMULTIDRAWARRAYSPROC pglMultiDrawArrays = nullptr;
GLAPI PFNGLMULTIDRAWELEMENTSPROC pglMultiDrawElements = nullptr;



// GL_NV_texture_barrier : no opengl version
//////////////////////////////////////////////

GLAPI PFNGLTEXTUREBARRIERNVPROC pglTextureBarrier = nullptr;



// GL_ARB_bindless_texture : no opengl version
////////////////////////////////////////////////

GLAPI PFNGLGETTEXTUREHANDLEARBPROC pglGetTextureHandleARB = nullptr;
GLAPI PFNGLGETTEXTURESAMPLERHANDLEARBPROC pglGetTextureSamplerHandleARB = nullptr;
GLAPI PFNGLMAKETEXTUREHANDLERESIDENTARBPROC pglMakeTextureHandleResidentARB = nullptr;
GLAPI PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC pglMakeTextureHandleNonResidentARB = nullptr;
GLAPI PFNGLGETIMAGEHANDLEARBPROC pglGetImageHandleARB = nullptr;
GLAPI PFNGLMAKEIMAGEHANDLERESIDENTARBPROC pglMakeImageHandleResidentARB = nullptr;
GLAPI PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC pglMakeImageHandleNonResidentARB = nullptr;
GLAPI PFNGLUNIFORMHANDLEUI64ARBPROC pglUniformHandleui64ARB = nullptr;
GLAPI PFNGLUNIFORMHANDLEUI64VARBPROC pglUniformHandleui64vARB = nullptr;
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC pglProgramUniformHandleui64ARB = nullptr;
GLAPI PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC pglProgramUniformHandleui64vARB = nullptr;
GLAPI PFNGLVERTEXATTRIBL1UI64ARBPROC pglVertexAttribL1ui64ARB = nullptr;
GLAPI PFNGLVERTEXATTRIBL1UI64VARBPROC pglVertexAttribL1ui64vARB = nullptr;
GLAPI PFNGLGETVERTEXATTRIBLUI64VARBPROC pglGetVertexAttribLui64vARB = nullptr;



// GL_KHR_debug : no opengl version
/////////////////////////////////////

GLAPI PFNGLPUSHDEBUGGROUPPROC pglPushDebugGroup = nullptr;
GLAPI PFNGLPOPDEBUGGROUPPROC pglPopDebugGroup = nullptr;
GLAPI PFNGLOBJECTLABELPROC pglObjectLabel = nullptr;



// GL_ARB_texture_storage : opengl version 4.1 , opengl es version 3.1
////////////////////////////////////////////////////////////////////////

GLAPI PFNGLTEXSTORAGE2DPROC pglTexStorage2D = nullptr;
GLAPI PFNGLTEXSTORAGE3DPROC pglTexStorage3D = nullptr;

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglExtensions.h"
#include "../deoglBasics.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTContext.h"
#include "../renderthread/deoglRTLogger.h"

#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
#include <GL/glx.h>
#include "deoglXExtResult.h"
#endif

#ifdef OS_BEOS
#include <GLView.h>
#include "../window/deoglRRenderWindow.h"
#endif

#ifdef ANDROID
extern __eglMustCastToProperFunctionPointerType androidGetProcAddress( const char *name );
#endif

#ifdef OS_MACOS
#include "macosfix.h"
#endif

#ifdef OS_W32
#include "deoglWExtResult.h"
#endif

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

static const char * const vVendorNames[ deoglExtensions::VendorCount ] = {
	"ATI/AMD",
	"nVidia",
	"Unknown"
};

#ifdef OS_ANDROID
static const char * const vGLESVersionNames[ deoglExtensions::evglesUnknown + 1 ] = {
	"<ES 3.0",
	"ES 3.0",
	"ES 3.1",
	"ES 3.2",
	">ES 3.2"
};

#else
static const char * const vGLVersionNames[ deoglExtensions::evglUnknown + 1 ] = {
	"1.1",
	"1.2",
	"1.3",
	"1.4",
	"1.5",
	"2.0",
	"2.1",
	"3.0",
	"3.1",
	"3.2",
	"3.3",
	"4.0",
	"4.1",
	"4.2",
	"4.3",
	"4.4",
	"4.5",
	"4.6",
	">4.6"
};
#endif

// Convert regular expression:
// Search: ext([^,]+)
// Replace: "GL\1"
static const char * const vExtensionNames[ deoglExtensions::EXT_COUNT ] = {
	"GL_ARB_copy_buffer",
	"GL_ARB_copy_image",
	"GL_ARB_debug_output",
	"GL_ARB_draw_buffers",
	"GL_ARB_draw_buffers_blend",
	"GL_ARB_draw_elements_base_vertex",
	"GL_ARB_draw_instanced",
	"GL_ARB_framebuffer_object",
	"GL_ARB_geometry_shader4",
	"GL_ARB_gpu_shader5",
	"GL_ARB_get_program_binary",
	"GL_ARB_multisample",
	"GL_ARB_multitexture",
	"GL_ARB_occlusion_query",
	"GL_ARB_provoking_vertex",
	"GL_ARB_sampler_objects",
	"GL_ARB_seamless_cube_map",
	"GL_ARB_separate_shader_objects",
	"GL_ARB_shader_objects",
	"GL_ARB_tessellation_shader",
	"GL_ARB_texture_buffer_object",
	"GL_ARB_texture_compression",
	"GL_ARB_texture_cube_map_array",
	"GL_ARB_texture_multisample",
	"GL_ARB_timer_query",
	"GL_ARB_transform_feedback2",
	"GL_ARB_transform_feedback3",
	"GL_ARB_transform_feedback_instanced",
	"GL_ARB_uniform_buffer_object",
	"GL_ARB_vertex_array_object",
	"GL_ARB_vertex_buffer_object",
	"GL_ARB_vertex_program",
	"GL_ARB_vertex_shader",
	"GL_ARB_viewport_array",
	"GL_ARB_clip_control",
	"GL_ARB_shader_storage_buffer_object",
	"GL_ARB_program_interface_query",
	"GL_ARB_shader_image_load_store",
	"GL_ARB_compute_shader",
	"GL_ARB_draw_indirect",
	"GL_ARB_multi_draw_indirect",
	"GL_ARB_indirect_parameters",
	"GL_ARB_bindless_texture",
	"GL_ARB_fragment_layer_viewport",
	"GL_ARB_shader_draw_parameters",
	"GL_ARB_shader_viewport_layer_array",
	"GL_ARB_depth_clamp",
	"GL_ARB_shading_language_420pack",
	"GL_ARB_shader_atomic_counters",
	"GL_ARB_shader_atomic_counter_ops",
	"GL_ARB_gpu_shader_fp64",
	"GL_ARB_direct_state_access",
	"GL_ARB_clear_buffer_object",
	"GL_ARB_buffer_storage",
	
	"GL_EXT_bindable_uniform",
	"GL_EXT_blend_equation_separate",
	"GL_EXT_blend_func_separate",
	"GL_EXT_blend_minmax",
	"GL_EXT_copy_texture",
	"GL_EXT_draw_range_elements",
	"GL_EXT_gpu_shader4",
	"GL_EXT_multi_draw_arrays",
	"GL_EXT_packed_depth_stencil",
	"GL_EXT_polygon_offset",
	"GL_EXT_subtexture",
	"GL_EXT_texture3D",
	"GL_EXT_texture_array",
	"GL_EXT_texture_integer",
	"GL_EXT_texture_object",
	"GL_EXT_transform_feedback",
	
	"GL_AMD_debug_output",
	"GL_AMD_performance_monitor",
	"GL_AMD_seamless_cubemap_per_texture",
	"GL_AMD_vertex_shader_layer",
	
	"GL_ATI_meminfo",
	"GL_ATI_separate_stencil",
	
	"GL_NV_copy_image",
	"GL_NV_packed_depth_stencil",
	"GL_NV_texture_barrier",
	"GL_NV_transform_feedback",
	"GL_NV_transform_feedback2",
	"GL_NV_transform_feedback3",
	
	"GL_KHR_debug",
	
	"GLX_EXT_swap_control",
	"GLX_EXT_swap_control_tear",
	
	"WGL_EXT_swap_control",
	"WGL_EXT_swap_control_tear"
};



// Class deoglExtensions
//////////////////////////

bool deoglExtensions::pInitialized = false;

// Constructor, destructor
////////////////////////////

deoglExtensions::deoglExtensions( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pVendor( evUnknown ),
pGLVersionMajor( 0 ),
pGLVersionMinor( 0 ),
pGLVersion( evgl1p1 ),
pGLESVersion( evglesUnsupported ),

pHasRequiredFunctions( true ),

pHasArrayCubeMap( false ),
pHasSeamlessCubeMap( false ),
pHasCopyImage( false ),
pSupportsGeometryShader( false ),
pSupportsGSInstancing( false ),
pSupportsComputeShader( false ),
pSupportsVSLayer( false )
{
	int i;
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = false;
		pDisableExtension[ i ] = false;
	}
}

deoglExtensions::~deoglExtensions(){
}



// Management
///////////////

void deoglExtensions::Initialize(){
	pScanVendor();
	pScanVersion();
	pScanExtensions();
	pDisableExtensions();
	
	if( ! pInitialized ){
		pFetchRequiredFunctions();
		pFetchOptionalFunctions();
		pOptionalDisableExtensions();
		pFixBuggedFunctions();
		
		pInitialized = true;
	}
}

void deoglExtensions::PrintSummary(){
	int i;
	
	pRenderThread.GetLogger().LogInfoFormat( "Vendor: %s (%s)",
		vVendorNames[ pVendor ], glGetString( GL_VENDOR ) );
	
	#ifdef OS_ANDROID
		pRenderThread.GetLogger().LogInfoFormat( "Version: %s (%s) [%d, %d]",
			vGLESVersionNames[ pGLESVersion ], glGetString( GL_VERSION ),
			pGLVersionMajor, pGLVersionMinor );
	#else
		pRenderThread.GetLogger().LogInfoFormat( "Version: %s (%s) [%d, %d]",
			vGLVersionNames[ pGLVersion ], glGetString( GL_VERSION ),
			pGLVersionMajor, pGLVersionMinor );
	#endif
	
	pRenderThread.GetLogger().LogInfo( "Supported Extensions:" );
	for( i=0; i<EXT_COUNT; i++ ){
		if( pHasExtension[ i ] ){
			pRenderThread.GetLogger().LogInfoFormat( "- %s", vExtensionNames[ i ] );
		}
	}
	
	pRenderThread.GetLogger().LogInfo( "Not Supported Extensions:" );
	for( i=0; i<EXT_COUNT; i++ ){
		if( ! pHasExtension[ i ] ){
			pRenderThread.GetLogger().LogInfoFormat( "- %s", vExtensionNames[ i ] );
		}
	}
	
	pRenderThread.GetLogger().LogInfo( "Extension Choices:" );
	pRenderThread.GetLogger().LogInfoFormat( "- Has Arry Cube Map: %s", pHasArrayCubeMap ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Has Seamless Cube Map: %s", pHasSeamlessCubeMap ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Has Copy Image: %s", pHasCopyImage ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Supports Geometry Shader: %s", pSupportsGeometryShader ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Supports Geometry Shader Instancing: %s", pSupportsGSInstancing ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Supports Compute Shader: %s", pSupportsComputeShader ? "Yes" : "No" );
	pRenderThread.GetLogger().LogInfoFormat( "- Supports Vertex Shader Layer: %s", pSupportsVSLayer ? "Yes" : "No" );
}

bool deoglExtensions::VerifyPresence() const{
	bool allPresent = pHasRequiredFunctions;
	
	allPresent &= pVerifyExtensionPresent( ext_ARB_depth_clamp );
	allPresent &= pVerifyExtensionPresent( ext_ARB_copy_image, ext_NV_copy_image );
	allPresent &= pVerifyExtensionPresent( ext_ARB_compute_shader );
	allPresent &= pVerifyExtensionPresent( ext_ARB_shader_storage_buffer_object );
	allPresent &= pVerifyExtensionPresent( ext_ARB_shader_image_load_store );
	allPresent &= pVerifyExtensionPresent( ext_ARB_shading_language_420pack );
	allPresent &= pVerifyExtensionPresent( ext_ARB_shader_atomic_counters );
	allPresent &= pVerifyExtensionPresent( ext_ARB_clear_buffer_object );
	allPresent &= pVerifyExtensionPresent( ext_ARB_buffer_storage );
	// allPresent &= pVerifyExtensionPresent( ext_ARB_gpu_shader_fp64 );
	allPresent &= pSupportsGeometryShader;
	
	return allPresent;
}

const char *deoglExtensions::GetExtensionName( eExtensions extension ) const{
	if( extension < 0 || extension >= EXT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return vExtensionNames[ extension ];
}

void deoglExtensions::DisableExtension( eExtensions extension ){
	pHasExtension[ extension ] = false;
	pDisableExtension[ extension ] = true;
	
	switch( extension ){
	case ext_ARB_shader_storage_buffer_object:
		pglShaderStorageBlockBinding = NULL;
		break;
		
	case ext_ARB_program_interface_query:
		pglGetProgramInterfaceiv = NULL;
		pglGetProgramResourceIndex = NULL;
		break;
		
	case ext_ARB_uniform_buffer_object:
		pglGetUniformIndices = NULL;
		pglGetActiveUniformsiv = NULL;
		pglGetActiveUniformName = NULL;
		pglGetUniformBlockIndex = NULL;
		pglGetActiveUniformBlockiv = NULL;
		pglGetActiveUniformBlockName = NULL;
		pglUniformBlockBinding = NULL;
		break;
		
	default:
		break;
	}
}



// Private functions
//////////////////////

void deoglExtensions::pScanVendor(){
	pStrVendor = ( const char * )glGetString( GL_VENDOR );

	if( pStrVendor.BeginsWithInsensitive( "ati" ) ){
		pVendor = evATI;
		
	}else if( pStrVendor.BeginsWithInsensitive( "amd" ) ){
		pVendor = evATI;
		
	}else if( pStrVendor.BeginsWithInsensitive( "nvidia" ) ){
		pVendor = evNVidia;
		
	}else{
		pVendor = evUnknown;
	}
}

void deoglExtensions::pScanVersion(){
	glGetIntegerv( GL_MAJOR_VERSION, ( GLint* )&pGLVersionMajor );
	glGetIntegerv( GL_MINOR_VERSION, ( GLint* )&pGLVersionMinor );
	pStrGLVersion = ( const char * )glGetString( GL_VERSION );
	
	#ifdef OS_ANDROID
	if( pGLVersionMajor == 3 ){
		if( pGLVersionMinor <= 0 ){
			pGLESVersion = evgles3p0;
			
		}else if( pGLVersionMinor == 1 ){
			pGLESVersion = evgles3p1;
			
		}else{
			pGLESVersion = evgles3p2;
		}
		
	}else if( pGLVersionMajor < 1 ){
		pGLESVersion = evglesUnsupported; // broken implementation
		
	}else{
		pGLESVersion = evglesUnknown; // anything higher than what we care for
	}
	
	#else
	if( pGLVersionMajor == 1 ){
		if( pGLVersionMinor <= 1 ){
			pGLVersion = evgl1p1;
			
		}else if( pGLVersionMinor == 2 ){
			pGLVersion = evgl1p2;
			
		}else if( pGLVersionMinor == 3 ){
			pGLVersion = evgl1p3;
			
		}else if( pGLVersionMinor == 4 ){
			pGLVersion = evgl1p4;
			
		}else{
			pGLVersion = evgl1p5;
		}
		
	}else if( pGLVersionMajor == 2 ){
		if( pGLVersionMinor <= 0 ){
			pGLVersion = evgl2p0;
			
		}else{
			pGLVersion = evgl2p1;
		}
		
	}else if( pGLVersionMajor == 3 ){
		if( pGLVersionMinor <= 0 ){
			pGLVersion = evgl3p0;
			
		}else if( pGLVersionMinor == 1 ){
			pGLVersion = evgl3p1;
			
		}else if( pGLVersionMinor == 2 ){
			pGLVersion = evgl3p2;
			
		}else{
			pGLVersion = evgl3p3;
		}
		
	}else if( pGLVersionMajor == 4 ){
		if( pGLVersionMinor <= 0 ){
			pGLVersion = evgl4p0;
			
		}else if( pGLVersionMinor == 1 ){
			pGLVersion = evgl4p1;
			
		}else if( pGLVersionMinor == 2 ){
			pGLVersion = evgl4p2;
			
		}else if( pGLVersionMinor == 3 ){
			pGLVersion = evgl4p3;
			
		}else if( pGLVersionMinor == 4 ){
			pGLVersion = evgl4p4;
			
		}else if( pGLVersionMinor == 5 ){
			pGLVersion = evgl4p5;
			
		}else{
			pGLVersion = evgl4p6;
		}
		
	}else if( pGLVersionMajor < 1 ){
		pGLVersion = evgl1p1; // broken implementation
		
	}else{
		pGLVersion = evgl4p6; // anything higher than what we care for
	}
	#endif
	
#if 0
	#ifdef OS_ANDROID
	if( strncmp( pStrGLVersion.GetString(), "3.0", 3 ) == 0 ){
		pGLESVersion = evgles3p0;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.1", 3 ) == 0 ){
		pGLESVersion = evgles3p1;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.2", 3 ) == 0 ){
		pGLESVersion = evgles3p2;
		
	}else{
		pGLESVersion = evglesUnknown;
	}
	
	#else
	if( strncmp( pStrGLVersion.GetString(), "1.1", 3 ) == 0 ){
		pGLVersion = evgl1p1;
		
	}else if( strncmp( pStrGLVersion.GetString(), "1.2", 3 ) == 0 ){
		pGLVersion = evgl1p2;
		
	}else if( strncmp( pStrGLVersion.GetString(), "1.3", 3 ) == 0 ){
		pGLVersion = evgl1p3;
		
	}else if( strncmp( pStrGLVersion.GetString(), "1.4", 3 ) == 0 ){
		pGLVersion = evgl1p4;
		
	}else if( strncmp( pStrGLVersion.GetString(), "1.5", 3 ) == 0 ){
		pGLVersion = evgl1p5;
		
	}else if( strncmp( pStrGLVersion.GetString(), "2.0", 3 ) == 0 ){
		pGLVersion = evgl2p0;
		
	}else if( strncmp( pStrGLVersion.GetString(), "2.1", 3 ) == 0 ){
		pGLVersion = evgl2p1;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.0", 3 ) == 0 ){
		pGLVersion = evgl3p0;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.1", 3 ) == 0 ){
		pGLVersion = evgl3p1;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.2", 3 ) == 0 ){
		pGLVersion = evgl3p2;
		
	}else if( strncmp( pStrGLVersion.GetString(), "3.3", 3 ) == 0 ){
		pGLVersion = evgl3p3;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.0", 3 ) == 0 ){
		pGLVersion = evgl4p0;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.1", 3 ) == 0 ){
		pGLVersion = evgl4p1;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.2", 3 ) == 0 ){
		pGLVersion = evgl4p2;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.3", 3 ) == 0 ){
		pGLVersion = evgl4p3;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.4", 3 ) == 0 ){
		pGLVersion = evgl4p4;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.5", 3 ) == 0 ){
		pGLVersion = evgl4p5;
		
	}else if( strncmp( pStrGLVersion.GetString(), "4.6", 3 ) == 0 ){
		pGLVersion = evgl4p6;
		
	}else{
		pGLVersion = evglUnknown;
	}
	#endif
#endif
}

void deoglExtensions::pScanExtensions(){
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	const char *strXExtensions = ( const char * )glXGetClientString( pRenderThread.GetContext().GetDisplay(), GLX_EXTENSIONS );
	if( ! strXExtensions ){
		strXExtensions = "";
	}
#endif
	
#ifdef ANDROID
	const char *strAExtensions = ( const char * )eglQueryString( pRenderThread.GetContext().GetDisplay(), EGL_EXTENSIONS );
	if( ! strAExtensions ){
		strAExtensions = "";
	}
#endif
	
	PFNGLGETSTRINGIPROC pglGetStringi = ( PFNGLGETSTRINGIPROC )pRenderThread.GetContext().GetFunctionPointer( "glGetStringi" );
	int i;
	
	if( pglGetStringi ){
		GLint extensionCount = 0;
		glGetIntegerv( GL_NUM_EXTENSIONS, &extensionCount );
		
		for( i=0; i<extensionCount; i++ ){
			pStrListExtensions.Add( ( const char * )pglGetStringi( GL_EXTENSIONS, i ) );
		}
		
	}else{
		const char * const strExtensions = ( const char * )glGetString( GL_EXTENSIONS );
		if( strExtensions ){
			pStrListExtensions = decString( strExtensions ).Split( ' ' );
		}
	}
	
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	pStrListExtensions += decString( strXExtensions ).Split( ' ' );
#endif
	
	pStrListExtensions.SortAscending();
	
	for( i=0; i<EXT_COUNT; i++ ){
		pHasExtension[ i ] = pStrListExtensions.Has( vExtensionNames[ i ] );
	}
	
	// this one is strange. ARB_shader_draw_parameters yields gl_DrawID in vertex shaders.
	// this extension can be found on <4.6 core GPUs. but without 4.6 core gl_DrawID causes
	// shader compilation to fail. sometimes OpenGL is a huge mess. why expose the presence
	// of gl_DrawID using an extension on a lower core version if a higher core version is
	// mandatory to use it?
	if( pHasExtension[ ext_ARB_shader_draw_parameters ] && pGLVersion < evgl4p6 ){
		pRenderThread.GetLogger().LogWarn( "Extension ARB_shader_draw_parameters forcefully"
			" disabled since OpenGL is less then 4.6 Core" );
		pHasExtension[ ext_ARB_shader_draw_parameters ] = false;
	}
	
	// the same problem for gl_Layer in vertex shaders. looks like core 4.5 is required
	// otherwise it fails. maybe later on create a bug-test to figure out if these two
	// parameters are supported so we do not have to hard-code things here
	if( pHasExtension[ ext_ARB_shader_viewport_layer_array ] && pGLVersion < evgl4p5 ){
		pRenderThread.GetLogger().LogWarn( "Extension ARB_shader_viewport_layer_array forcefully"
			" disabled since OpenGL is less then 4.5 Core" );
		pHasExtension[ ext_ARB_shader_viewport_layer_array ] = false;
	}
	if( pHasExtension[ ext_AMD_vertex_shader_layer ] && pGLVersion < evgl4p5 ){
		pRenderThread.GetLogger().LogWarn( "Extension AMD_vertex_shader_layer forcefully"
			" disabled since OpenGL is less then 4.5 Core" );
		pHasExtension[ ext_AMD_vertex_shader_layer ] = false;
	}
	
	pHasSeamlessCubeMap = pHasExtension[ ext_ARB_seamless_cube_map ]
		|| pHasExtension[ ext_AMD_seamless_cubemap_per_texture ];
	
	pHasArrayCubeMap = pHasExtension[ ext_ARB_texture_cube_map_array ];
	
	pHasCopyImage = pHasExtension[ ext_ARB_copy_image ]
		|| pHasExtension[ ext_NV_copy_image ];
	
	pSupportsGeometryShader = pGLVersion >= evgl3p2
		|| pGLESVersion >= evgles3p2
		|| pHasExtension[ ext_ARB_geometry_shader4 ]
		|| pHasExtension[ ext_EXT_gpu_shader4 ];
	
	// "core since" = "4.0". "core in" = "4.6". 4.0 is not working
	pSupportsGSInstancing = pGLVersion >= evgl4p6
		|| pGLESVersion >= evgles3p2
		|| pHasExtension[ ext_ARB_gpu_shader5 ];
	
	// "core since" = "4.3". "core in" = "4.6"
	pSupportsComputeShader = pGLVersion >= evgl4p6
		|| pGLESVersion >= evgles3p2
		|| pHasExtension[ ext_ARB_compute_shader ];
	
	pSupportsVSLayer = pHasExtension[ ext_ARB_shader_viewport_layer_array ]
		|| pHasExtension[ ext_AMD_vertex_shader_layer ];
	
	#ifdef OS_ANDROID
	/*
	pRenderThread.GetLogger().LogInfo( "Extensions reported by driver:" );
	for( i=0; i<pStrListExtensions.GetCount(); i++ ){
		pRenderThread.GetLogger().LogInfoFormat( "- %s", pStrListExtensions.GetAt( i ).GetString() );
	}
	*/
	#endif
}

void deoglExtensions::pDisableExtensions(){
	const decStringSet &list = pRenderThread.GetConfiguration().GetDisableExtensions();
	const int count = list.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		const decString &name = list.GetAt( i );
		
		for( j=0; j<EXT_COUNT; j++ ){
			if( name == vExtensionNames[ j ] ){
				pRenderThread.GetLogger().LogInfoFormat( "Disable extension '%s'", name.GetString() );
				DisableExtension( ( eExtensions )j );
			}
		}
	}
}

void deoglExtensions::pFetchRequiredFunctions(){
	// GL_EXT_polygon_offset . sort of basic function extension as it is sometimes not listed
	#ifdef OS_W32
		pglPolygonOffset = &glPolygonOffset;
	#else
		pGetRequiredFunction( (void**)&pglPolygonOffset, "glPolygonOffset" );
	#endif
	
	// GL_EXT_texture3D : opengl version 1.2
	pGetRequiredFunction( (void**)&pglTexImage3D, "glTexImage3D" );
	pGetRequiredFunction( (void**)&pglTexSubImage3D, "glTexSubImage3D" );
	
	// GL_EXT_blend_color : opengl version 1.2
	pGetRequiredFunction( (void**)&pglBlendColor, "glBlendColor" );
	
	// GL_EXT_blend_minmax : opengl version 1.2
	pGetRequiredFunction( (void**)&pglBlendEquation, "glBlendEquation" );
	
	// GL_ARB_multisample : opengl version 1.3
	
	// GL_ARB_multitexture . opengl version 1.3
	pGetRequiredFunction( (void**)&pglActiveTexture, "glActiveTexture" );
	
	// GL_ARB_texture_compression : opengl version 1.3
	pGetRequiredFunction( (void**)&pglCompressedTexImage3D, "glCompressedTexImage3D" );
	pGetRequiredFunction( (void**)&pglCompressedTexImage2D, "glCompressedTexImage2D" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglCompressedTexImage1D, "glCompressedTexImage1D" );
	#endif
	pGetRequiredFunction( (void**)&pglCompressedTexSubImage3D, "glCompressedTexSubImage3D" );
	pGetRequiredFunction( (void**)&pglCompressedTexSubImage2D, "glCompressedTexSubImage2D" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglCompressedTexSubImage1D, "glCompressedTexSubImage1D" );
	#endif
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglGetCompressedTexImage, "glGetCompressedTexImage" );
	#endif
	
	// opengl version 1.4
	pGetRequiredFunction( (void**)&pglBlendFuncSeparate, "glBlendFuncSeparate" );
	
	// GL_ARB_vertex_buffer_object . opengl version 1.5
	pGetRequiredFunction( (void**)&pglBindBuffer, "glBindBuffer" );
	pGetRequiredFunction( (void**)&pglDeleteBuffers, "glDeleteBuffers" );
	pGetRequiredFunction( (void**)&pglGenBuffers, "glGenBuffers" );
	pGetRequiredFunction( (void**)&pglBufferData, "glBufferData" );
	pGetRequiredFunction( (void**)&pglBufferSubData, "glBufferSubData" );
	pGetRequiredFunction( (void**)&pglGetBufferSubData, "glGetBufferSubData" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglMapBuffer, "glMapBuffer" );
	#endif
	pGetRequiredFunction( (void**)&pglMapBufferRange, "glMapBufferRange" );
	pGetRequiredFunction( (void**)&pglUnmapBuffer, "glUnmapBuffer" );
	
	// GL_ARB_occlusion_query . opengl version 1.5
	pGetRequiredFunction( (void**)&pglGenQueries, "glGenQueries" );
	pGetRequiredFunction( (void**)&pglDeleteQueries, "glDeleteQueries" );
	pGetRequiredFunction( (void**)&pglBeginQuery, "glBeginQuery" );
	pGetRequiredFunction( (void**)&pglEndQuery, "glEndQuery" );
	pGetRequiredFunction( (void**)&pglGetQueryObjectuiv, "glGetQueryObjectuiv" );
	
	// opengl version 2.0
	pGetRequiredFunction( (void**)&pglStencilOpSeparate, "glStencilOpSeparate" );
	pGetRequiredFunction( (void**)&pglStencilFuncSeparate, "glStencilFuncSeparate" );
	pGetRequiredFunction( (void**)&pglStencilMaskSeparate, "glStencilMaskSeparate" );
	
	// GL_ARB_vertex_program . opengl version 2.0
	pGetRequiredFunction( (void**)&pglVertexAttribPointer, "glVertexAttribPointer" );
	pGetRequiredFunction( (void**)&pglEnableVertexAttribArray, "glEnableVertexAttribArray" );
	
	// GL_ARB_vertex_shader : opengl version 2.0
	pGetRequiredFunction( (void**)&pglBindAttribLocation, "glBindAttribLocation" );
	
	// GL_ARB_draw_buffers : opengl version 2.0
	pGetRequiredFunction( (void**)&pglDrawBuffers, "glDrawBuffers" );
	
	// GL_ARB_shader_objects . no opengl version, most probably 2.1
	pGetRequiredFunction( (void**)&pglDeleteShader, "glDeleteShader" ); // core only
	pGetRequiredFunction( (void**)&pglDeleteProgram, "glDeleteProgram" ); // core only
	pGetRequiredFunction( (void**)&pglCreateShader, "glCreateShader" ); // core only
	pGetRequiredFunction( (void**)&pglCreateProgram, "glCreateProgram" ); // core only
	pGetRequiredFunction( (void**)&pglAttachShader, "glAttachShader" ); // core only
	pGetRequiredFunction( (void**)&pglUseProgram, "glUseProgram" ); // core only
	pGetRequiredFunction( (void**)&pglGetShaderiv, "glGetShaderiv" ); // core only
	pGetRequiredFunction( (void**)&pglGetProgramiv, "glGetProgramiv" ); // core only
	pGetRequiredFunction( (void**)&pglGetShaderInfoLog, "glGetShaderInfoLog" ); // core only
	pGetRequiredFunction( (void**)&pglGetProgramInfoLog, "glGetProgramInfoLog" ); // core only
	
// 	pGetRequiredFunction( (void**)&pglDeleteObject, "glDeleteObject" );
// 	pGetRequiredFunction( (void**)&pglCreateShaderObject, "glCreateShaderObject" );
	pGetRequiredFunction( (void**)&pglShaderSource, "glShaderSource" );
	pGetRequiredFunction( (void**)&pglCompileShader, "glCompileShader" );
// 	pGetRequiredFunction( (void**)&pglCreateProgramObject, "glCreateProgramObject" );
// 	pGetRequiredFunction( (void**)&pglAttachObject, "glAttachObject" );
	pGetRequiredFunction( (void**)&pglLinkProgram, "glLinkProgram" );
// 	pGetRequiredFunction( (void**)&pglUseProgramObject, "glUseProgramObject" );
	pGetRequiredFunction( (void**)&pglUniform1f, "glUniform1f" );
	pGetRequiredFunction( (void**)&pglUniform2f, "glUniform2f" );
	pGetRequiredFunction( (void**)&pglUniform3f, "glUniform3f" );
	pGetRequiredFunction( (void**)&pglUniform4f, "glUniform4f" );
	pGetRequiredFunction( (void**)&pglUniform1i, "glUniform1i" );
	pGetRequiredFunction( (void**)&pglUniform2i, "glUniform2i" );
	pGetRequiredFunction( (void**)&pglUniform3i, "glUniform3i" );
	pGetRequiredFunction( (void**)&pglUniform4i, "glUniform4i" );
	pGetRequiredFunction( (void**)&pglUniform1fv, "glUniform1fv" );
	pGetRequiredFunction( (void**)&pglUniform2fv, "glUniform2fv" );
	pGetRequiredFunction( (void**)&pglUniform3fv, "glUniform3fv" );
	pGetRequiredFunction( (void**)&pglUniform4fv, "glUniform4fv" );
	pGetRequiredFunction( (void**)&pglUniform1iv, "glUniform1iv" );
	pGetRequiredFunction( (void**)&pglUniform2iv, "glUniform2iv" );
	pGetRequiredFunction( (void**)&pglUniform3iv, "glUniform3iv" );
	pGetRequiredFunction( (void**)&pglUniform4iv, "glUniform4iv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix2fv, "glUniformMatrix2fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix3fv, "glUniformMatrix3fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix4fv, "glUniformMatrix4fv" );
// 	pGetRequiredFunction( (void**)&pglGetObjectParameteriv, "glGetObjectParameteriv" );
// 	pGetRequiredFunction( (void**)&pglGetInfoLog, "glGetInfoLog" );
	pGetRequiredFunction( (void**)&pglGetUniformLocation, "glGetUniformLocation" );
	
	// these are listed in opengl version 2.1 but do not look like belonging directly to an extension
	pGetRequiredFunction( (void**)&pglUniformMatrix2x3fv, "glUniformMatrix2x3fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix3x2fv, "glUniformMatrix3x2fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix2x4fv, "glUniformMatrix2x4fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix4x2fv, "glUniformMatrix4x2fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix3x4fv, "glUniformMatrix3x4fv" );
	pGetRequiredFunction( (void**)&pglUniformMatrix4x3fv, "glUniformMatrix4x3fv" );
	
	// these are listed in opengl version 3.0 but do not have an extension assigned
	pGetRequiredFunction( (void**)&pglClearBufferiv, "glClearBufferiv" );
	pGetRequiredFunction( (void**)&pglClearBufferuiv, "glClearBufferuiv" );
	pGetRequiredFunction( (void**)&pglClearBufferfv, "glClearBufferfv" );
	pGetRequiredFunction( (void**)&pglClearBufferfi, "glClearBufferfi" );
	
	// GL_EXT_transform_feedback : opengl version 3.0
	pGetRequiredFunction( (void**)&pglBeginTransformFeedback, "glBeginTransformFeedback" );
	pGetRequiredFunction( (void**)&pglEndTransformFeedback, "glEndTransformFeedback" );
	pGetRequiredFunction( (void**)&pglBindBufferRange, "glBindBufferRange" );
	pGetRequiredFunction( (void**)&pglBindBufferBase, "glBindBufferBase" );
	pGetRequiredFunction( (void**)&pglTransformFeedbackVaryings, "glTransformFeedbackVaryings" );
	pGetRequiredFunction( (void**)&pglGetTransformFeedbackVarying, "glGetTransformFeedbackVarying" );
	
	// GL_EXT_gpu_shader4 : opengl version 3.0
	pGetRequiredFunction( (void**)&pglBindFragDataLocation, "glBindFragDataLocation" );
	pGetRequiredFunction( (void**)&pglUniform1ui, "glUniform1ui" );
	pGetRequiredFunction( (void**)&pglUniform2ui, "glUniform2ui" );
	pGetRequiredFunction( (void**)&pglUniform3ui, "glUniform3ui" );
	pGetRequiredFunction( (void**)&pglUniform4ui, "glUniform4ui" );
	pGetRequiredFunction( (void**)&pglUniform1uiv, "glUniform1uiv" );
	pGetRequiredFunction( (void**)&pglUniform2uiv, "glUniform2uiv" );
	pGetRequiredFunction( (void**)&pglUniform3uiv, "glUniform3uiv" );
	pGetRequiredFunction( (void**)&pglUniform4uiv, "glUniform4uiv" );
	
	// GL_EXT_texture_array : opengl version 3.0
	pGetRequiredFunction( (void**)&pglFramebufferTextureLayer, "glFramebufferTextureLayer" );
	
	// GL_ARB_draw_instanced : opengl version 3.1
	pGetRequiredFunction( (void**)&pglDrawArraysInstanced, "glDrawArraysInstanced" );
	pGetRequiredFunction( (void**)&pglDrawElementsInstanced, "glDrawElementsInstanced" );
	
	// GL_ARB_texture_buffer_object : opengl version 3.1
	pGetRequiredFunction( (void**)&pglTexBuffer, "glTexBuffer" );
	
	// GL_ARB_uniform_buffer_object : opengl version 3.1
	pGetRequiredFunction( (void**)&pglGetUniformIndices, "glGetUniformIndices" );
	pGetRequiredFunction( (void**)&pglGetActiveUniformsiv, "glGetActiveUniformsiv" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglGetActiveUniformName, "glGetActiveUniformName" );
	#endif
	pGetRequiredFunction( (void**)&pglGetUniformBlockIndex, "glGetUniformBlockIndex" );
	pGetRequiredFunction( (void**)&pglGetActiveUniformBlockiv, "glGetActiveUniformBlockiv" );
	pGetRequiredFunction( (void**)&pglGetActiveUniformBlockName, "glGetActiveUniformBlockName" );
	pGetRequiredFunction( (void**)&pglUniformBlockBinding, "glUniformBlockBinding" );
	
	// OpenGL 3.2 core stuff
	pGetRequiredFunction( (void**)&pglFenceSync, "glFenceSync" );
	pGetRequiredFunction( (void**)&pglDeleteSync, "glDeleteSync" );
	pGetRequiredFunction( (void**)&pglClientWaitSync, "glClientWaitSync" );
	pGetRequiredFunction( (void**)&pglWaitSync, "glWaitSync" );
	
	// GL_ARB_timer_query : opengl version 3.3
	pGetRequiredFunction( (void**)&pglQueryCounter, "glQueryCounter" );
	pGetRequiredFunction( (void**)&pglGetQueryObjectui64v, "glGetQueryObjectui64v" );
	
	// GL_ARB_sampler_objects : opengl version 3.3
	pGetRequiredFunction( (void**)&pglGenSamplers, "glGenSamplers" );
	pGetRequiredFunction( (void**)&pglDeleteSamplers, "glDeleteSamplers" );
	pGetRequiredFunction( (void**)&pglBindSampler, "glBindSampler" );
	pGetRequiredFunction( (void**)&pglSamplerParameteri, "glSamplerParameteri" );
	pGetRequiredFunction( (void**)&pglSamplerParameterf, "glSamplerParameterf" );
	
	// GL_ARB_framebuffer_object : no opengl version
	pGetRequiredFunction( (void**)&pglBindRenderbuffer, "glBindRenderbuffer" );
	pGetRequiredFunction( (void**)&pglDeleteRenderbuffers, "glDeleteRenderbuffers" );
	pGetRequiredFunction( (void**)&pglGenRenderbuffers, "glGenRenderbuffers" );
	pGetRequiredFunction( (void**)&pglRenderbufferStorage, "glRenderbufferStorage" );
	pGetRequiredFunction( (void**)&pglBindFramebuffer, "glBindFramebuffer" );
	pGetRequiredFunction( (void**)&pglDeleteFramebuffers, "glDeleteFramebuffers" );
	pGetRequiredFunction( (void**)&pglGenFramebuffers, "glGenFramebuffers" );
	pGetRequiredFunction( (void**)&pglCheckFramebufferStatus, "glCheckFramebufferStatus" );
	pGetRequiredFunction( (void**)&pglFramebufferTexture2D, "glFramebufferTexture2D" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglFramebufferTexture3D, "glFramebufferTexture3D" );
	#endif
	pGetRequiredFunction( (void**)&pglFramebufferRenderbuffer, "glFramebufferRenderbuffer" );
	pGetRequiredFunction( (void**)&pglGenerateMipmap, "glGenerateMipmap" );
	pGetRequiredFunction( (void**)&pglBlitFramebuffer, "glBlitFramebuffer" );
	#ifndef ANDROID
	pGetRequiredFunction( (void**)&pglFramebufferTexture, "glFramebufferTexture" );
	#endif
	
	// GL_ARB_geometry_shader4 : no opengl version
	
	// GL_ARB_gpu_shader5 : opengl version 4.0
	
	// GL_ARB_vertex_array_object : no opengl version
	pGetRequiredFunction( (void**)&pglBindVertexArray, "glBindVertexArray" );
	pGetRequiredFunction( (void**)&pglDeleteVertexArrays, "glDeleteVertexArrays" );
	pGetRequiredFunction( (void**)&pglGenVertexArrays, "glGenVertexArrays" );
	
	// GL_EXT_subtexture : no opengl version
	#ifdef OS_W32
	pglTexSubImage1D = &glTexSubImage1D;
	#elif ! defined ANDROID
	pGetRequiredFunction( (void**)&pglTexSubImage1D, "glTexSubImage1D" );
	#endif
	
	#ifdef OS_W32
	pglTexSubImage2D = glTexSubImage2D;
	#else
	pGetRequiredFunction( (void**)&pglTexSubImage2D, "glTexSubImage2D" );
	#endif
	
	// GL_EXT_copy_texture : no opengl version
	
	// GL_EXT_texture_object : no opengl version
	
	// GL_ARB_compute_shader : opengl version 4.3
	#ifdef ANDROID
	if( ! pSupportsComputeShader ){
		DETHROW_INFO( deeInvalidParam, "Computer Shader support missing" );
	}
	pglDispatchCompute = eglDispatchCompute;
	pglDispatchComputeIndirect = eglDispatchComputeIndirect;
	#else
	pGetRequiredFunction( (void**)&pglDispatchCompute, "glDispatchCompute" );
	pGetRequiredFunction( (void**)&pglDispatchComputeIndirect, "glDispatchComputeIndirect" );
	#endif
	
	// memory barrier required by compute shaders
	pGetRequiredFunction( (void**)&pglMemoryBarrier, "glMemoryBarrier" );
	
	// no opengl version: 2.0 stuff
	pGetRequiredFunction( (void**)&pglMultiDrawArrays, "glMultiDrawArrays" );
	pGetRequiredFunction( (void**)&pglMultiDrawElements, "glMultiDrawElements" );
	
	// GL_ARB_copy_buffer : no opengl version
	pGetRequiredFunction( (void**)&pglCopyBufferSubData, "glCopyBufferSubData" );
}

void deoglExtensions::pFetchOptionalFunctions(){
	// Regular expressions to convert deoglExtResult.h declarations into function fetch calls
	// Search: extern PFN[A-Z0-9]+PROC p(gl[A-Za-z0-9_]+);
	// Replace: pGetOptionalFunction( (void**)&p\1, "\1", "\1ARB", ext_XXX );
	
	// GL_ARB_draw_elements_base_vertex : opengl version 3.2 / opengl es version 3.2
	if( pGLVersion >= evgl3p2 || pGLESVersion >= evgles3p2 ){
		pHasExtension[ ext_ARB_draw_elements_base_vertex ] = ! pDisableExtension[ ext_ARB_draw_elements_base_vertex ];
	}
	if( pHasExtension[ ext_ARB_draw_elements_base_vertex ] ){
		pGetOptionalFunctionArbExt( (void**)&pglDrawElementsBaseVertex,
			"glDrawElementsBaseVertex", ext_ARB_draw_elements_base_vertex );
		pGetOptionalFunctionArbExt( (void**)&pglDrawRangeElementsBaseVertex,
			"glDrawRangeElementsBaseVertex", ext_ARB_draw_elements_base_vertex );
		pGetOptionalFunctionArbExt( (void**)&pglDrawElementsInstancedBaseVertex,
			"glDrawElementsInstancedBaseVertex", ext_ARB_draw_elements_base_vertex );
		pGetOptionalFunctionArbExt( (void**)&pglMultiDrawElementsBaseVertex,
			"glMultiDrawElementsBaseVertex", ext_ARB_draw_elements_base_vertex );
		pGetOptionalFunctionArbExt( (void**)&pglVertexAttribIPointer,
			"glVertexAttribIPointer", ext_ARB_draw_elements_base_vertex );
	}
	
	// GL_ARB_draw_buffers_blend : opengl version 4.0
	if( pHasExtension[ ext_ARB_draw_buffers_blend ] ){
	}
	
	// GL_ARB_tessellation_shader : opengl version 4.0
	if( pHasExtension[ ext_ARB_tessellation_shader ] ){
		pGetOptionalFunctionArbExt( (void**)&pglPatchParameteri, "glPatchParameteri", ext_ARB_tessellation_shader );
		pGetOptionalFunctionArbExt( (void**)&pglPatchParameterfv, "glPatchParameterfv", ext_ARB_tessellation_shader );
	}
	
	// GL_EXT_transform_feedback2 : opengl version 4.0
	if( pHasExtension[ ext_ARB_transform_feedback2 ] ){
		pGetOptionalFunctionArbExt( (void**)&pglBindTransformFeedback, "glBindTransformFeedback",  ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglDeleteTransformFeedbacks, "glDeleteTransformFeedbacks", ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglGenTransformFeedbacks, "glGenTransformFeedbacks", ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglIsTransformFeedback, "glIsTransformFeedback", ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglPauseTransformFeedback, "glPauseTransformFeedback", ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglResumeTransformFeedback, "glResumeTransformFeedback", ext_ARB_transform_feedback2 );
		pGetOptionalFunctionArbExt( (void**)&pglDrawTransformFeedback, "glDrawTransformFeedback", ext_ARB_transform_feedback2 );
	}
	
	// GL_EXT_transform_feedback3 : opengl version 4.0
	if( pHasExtension[ ext_ARB_transform_feedback3 ] ){
		pGetOptionalFunctionArbExt( (void**)&pglDrawTransformFeedbackStream, "glDrawTransformFeedbackStream", ext_ARB_transform_feedback3 );
		pGetOptionalFunctionArbExt( (void**)&pglBeginQueryIndexed, "glBeginQueryIndexed", ext_ARB_transform_feedback3 );
		pGetOptionalFunctionArbExt( (void**)&pglEndQueryIndexed, "glEndQueryIndexed", ext_ARB_transform_feedback3 );
		pGetOptionalFunctionArbExt( (void**)&pglGetQueryIndexediv, "glGetQueryIndexediv", ext_ARB_transform_feedback3 );
	}
	
	// GL_ARB_viewport_array : opengl version 4.1
	if( pHasExtension[ ext_ARB_viewport_array ] ){
		pGetOptionalFunctionArbExt( (void**)&pglViewportArrayv, "glViewportArrayv", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglViewportIndexedf, "glViewportIndexedf", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglViewportIndexedfv, "glViewportIndexedfv", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglScissorArrayv, "glScissorArrayv", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglScissorIndexed, "glScissorIndexed", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglScissorIndexedv, "glScissorIndexedv", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglDepthRangeArrayv, "glDepthRangeArrayv", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglDepthRangeIndexed, "glDepthRangeIndexed", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglGetFloati_v, "glGetFloati_v", ext_ARB_viewport_array );
		pGetOptionalFunctionArbExt( (void**)&pglGetDoublei_v, "glGetDoublei_v", ext_ARB_viewport_array );
		
		// these are wrong here. they are in GL_EXT_direct_state_access and Core 4.1 . doesn't work on nvidia
// 		pGetOptionalFunctionArbExt( (void**)&pglEnableIndexed, "glEnableIndexed", ext_ARB_viewport_array );
// 		pGetOptionalFunctionArbExt( (void**)&pglDisableIndexed, "glDisableIndexed", ext_ARB_viewport_array );
// 		pGetOptionalFunctionArbExt( (void**)&pglIsEnabledIndexed, "glIsEnabledIndexed", ext_ARB_viewport_array );
// 		pGetOptionalFunctionArbExt( (void**)&pglGetIntegerIndexedv, "glGetIntegerIndexedv", ext_ARB_viewport_array );
	}
	
	// GL_ARB_get_program_binar : opengl version 4.1
	if( pHasExtension[ ext_ARB_get_program_binary ] ){
		pGetOptionalFunctionArbExt( (void**)&pglGetProgramBinary, "glGetProgramBinary", ext_ARB_get_program_binary );
		pGetOptionalFunctionArbExt( (void**)&pglProgramBinary, "glProgramBinary", ext_ARB_get_program_binary );
		pGetOptionalFunctionArbExt( (void**)&pglProgramParameteri, "glProgramParameteri", ext_ARB_get_program_binary );
	}
	
	// GL_EXT_transform_feedback_instanced : opengl version 4.2
	if( pHasExtension[ ext_ARB_transform_feedback_instanced ] ){
		pGetOptionalFunctionArbExt( (void**)&pglDrawTransformFeedbackInstanced,
			"glDrawTransformFeedbackInstanced", ext_ARB_transform_feedback_instanced );
		pGetOptionalFunctionArbExt( (void**)&pglDrawTransformFeedbackStreamInstanced,
			"glDrawTransformFeedbackStreamInstanced", ext_ARB_transform_feedback_instanced );
	}
	
	// GL_ARB_copy_image : opengl version 4.3
	if( pHasExtension[ ext_ARB_copy_image ] ){
		pGetOptionalFunctionArbExt( (void**)&pglCopyImageSubData, "glCopyImageSubData", ext_ARB_copy_image );
	}
	
	// GL_NV_copy_image : opengl version 4.3
	if( pHasExtension[ ext_NV_copy_image ] ){
		pGetOptionalFunction( (void**)&pglCopyImageSubDataNV, "glCopyImageSubDataNV", ext_NV_copy_image );
	}
	
	// GL_ARB_shader_storage_buffer_object : opengl version 4.3
	pHasExtension[ ext_ARB_shader_storage_buffer_object ] &= ! pDisableExtension[ ext_ARB_shader_storage_buffer_object ];
	if( pHasExtension[ ext_ARB_shader_storage_buffer_object ] ){
		#ifdef ANDROID
		pglShaderStorageBlockBinding = eglShaderStorageBlockBinding;
		#else
		pGetOptionalFunction( (void**)&pglShaderStorageBlockBinding,
			"glShaderStorageBlockBinding", ext_ARB_shader_storage_buffer_object );
		#endif
	}
	
	// GL_ARB_program_interface_query : opengl version 4.3
	pHasExtension[ ext_ARB_program_interface_query ] &= ! pDisableExtension[ ext_ARB_program_interface_query ];
	if( pHasExtension[ ext_ARB_program_interface_query ] ){
		#ifdef ANDROID
		pglGetProgramInterfaceiv = eglGetProgramInterfaceiv;
		pglGetProgramResourceIndex = eglGetProgramResourceIndex;
		#else
		pGetOptionalFunction( (void**)&pglGetProgramInterfaceiv,
			"glGetProgramInterfaceiv", ext_ARB_program_interface_query );
		pGetOptionalFunction( (void**)&pglGetProgramResourceIndex,
			"glGetProgramResourceIndex", ext_ARB_program_interface_query );
		#endif
	}
	
	// GL_ARB_draw_indirect : opengl version 4.3
	if( pHasExtension[ ext_ARB_draw_indirect ] ){
		pGetOptionalFunction( (void**)&pglDrawArraysIndirect, "glDrawArraysIndirect", ext_ARB_draw_indirect );
		pGetOptionalFunction( (void**)&pglDrawElementsIndirect, "glDrawElementsIndirect", ext_ARB_draw_indirect );
	}
	
	// GL_ARB_multi_draw_indirect : opengl version 4.3
	if( pHasExtension[ ext_ARB_multi_draw_indirect ] ){
		pGetOptionalFunction( (void**)&pglMultiDrawArraysIndirect, "glMultiDrawArraysIndirect", ext_ARB_multi_draw_indirect );
		pGetOptionalFunction( (void**)&pglMultiDrawElementsIndirect, "glMultiDrawElementsIndirect", ext_ARB_multi_draw_indirect );
	}
	
	// GL_ARB_clear_buffer_object : opengl version 4.3
	if( pHasExtension[ ext_ARB_clear_buffer_object ] ){
		pGetOptionalFunction( (void**)&pglClearBufferSubData, "glClearBufferSubData", ext_ARB_clear_buffer_object );
	}
	
	// GL_ARB_buffer_storage : opengl version 4.3
	if( pHasExtension[ ext_ARB_buffer_storage ] ){
		pGetOptionalFunction( (void**)&pglBufferStorage, "glBufferStorage", ext_ARB_buffer_storage );
	}
	
	// GL_ARB_direct_state_access : opengl version 4.4
	if( pHasExtension[ ext_ARB_direct_state_access ] ){
		pGetOptionalFunction( (void**)&pglCreateBuffers, "glCreateBuffers", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedBufferStorage, "glNamedBufferStorage", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedBufferData, "glNamedBufferData", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedBufferSubData, "glNamedBufferSubData", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCopyNamedBufferSubData, "glCopyNamedBufferSubData", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglClearNamedBufferData, "glClearNamedBufferData", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglClearNamedBufferSubData, "glClearNamedBufferSubData", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglMapNamedBuffer, "glMapNamedBuffer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglMapNamedBufferRange, "glMapNamedBufferRange", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglUnmapNamedBuffer, "glUnmapNamedBuffer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCreateFramebuffers, "glCreateFramebuffers", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedFramebufferTexture, "glNamedFramebufferTexture", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedFramebufferTextureLayer, "glNamedFramebufferTextureLayer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglNamedFramebufferDrawBuffers, "glNamedFramebufferDrawBuffers", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglClearNamedFramebufferfv, "glClearNamedFramebufferfv", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglClearNamedFramebufferfi, "glClearNamedFramebufferfi", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglBlitNamedFramebuffer, "glBlitNamedFramebuffer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCheckNamedFramebufferStatus, "glCheckNamedFramebufferStatus", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCreateTextures, "glCreateTextures", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureBuffer, "glTextureBuffer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureBufferRange, "glTextureBufferRange", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureStorage2D, "glTextureStorage2D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureStorage3D, "glTextureStorage3D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureSubImage2D, "glTextureSubImage2D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureSubImage3D, "glTextureSubImage3D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCompressedTextureSubImage2D, "glCompressedTextureSubImage2D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCompressedTextureSubImage3D, "glCompressedTextureSubImage3D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCopyTextureSubImage2D, "glCopyTextureSubImage2D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCopyTextureSubImage3D, "glCopyTextureSubImage3D", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglTextureParameteri, "glTextureParameteri", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglGenerateTextureMipmap, "glGenerateTextureMipmap", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglBindTextureUnit, "glBindTextureUnit", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglGetCompressedTextureImage, "glGetCompressedTextureImage", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglGetTextureLevelParameteriv, "glGetTextureLevelParameteriv", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglCreateVertexArrays, "glCreateVertexArrays", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglEnableVertexArrayAttrib, "glEnableVertexArrayAttrib", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglVertexArrayElementBuffer, "glVertexArrayElementBuffer", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglVertexArrayVertexBuffers, "glVertexArrayVertexBuffers", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglVertexArrayAttribBinding, "glVertexArrayAttribBinding", ext_ARB_direct_state_access );
		pGetOptionalFunction( (void**)&pglVertexArrayBindingDivisor, "glVertexArrayBindingDivisor", ext_ARB_direct_state_access );
	}
	
	// GL_ARB_clip_control : opengl version 4.5
	if( pHasExtension[ ext_ARB_clip_control ] ){
		pGetOptionalFunction( (void**)&pglClipControl, "glClipControl", ext_ARB_clip_control );
	}
	
	// GL_ARB_indirect_parameters : opengl version 4.6
	if( pHasExtension[ ext_ARB_indirect_parameters ] ){
		pGetOptionalFunction( (void**)&pglMultiDrawArraysIndirectCountARB, "glMultiDrawArraysIndirectCountARB", ext_ARB_indirect_parameters );
		pGetOptionalFunction( (void**)&pglMultiDrawElementsIndirectCountARB, "glMultiDrawElementsIndirectCountARB", ext_ARB_indirect_parameters );
	}
	
	// GL_ARB_geometry_shader4 : no opengl version
	if( pHasExtension[ ext_ARB_geometry_shader4 ] ){
		pGetOptionalFunctionArbExt( (void**)&pglFramebufferTextureFace, "glFramebufferTextureFace", ext_ARB_geometry_shader4 );
	}
	
	// GL_ARB_texture_multisample : no opengl version
	if( pHasExtension[ ext_ARB_texture_multisample ] ){
	}
	
	// GL_ARB_get_program_binary : no opengl version
	if( pHasExtension[ ext_ARB_get_program_binary ] ){
	}
	
	// GL_ARB_separate_shader_objects : no opengl version
	if( pHasExtension[ ext_ARB_separate_shader_objects ] ){
	}
	
	// GL_ARB_debug_output : no opengl version
	if( pHasExtension[ ext_KHR_debug ] ){
		pGetOptionalFunctionArbExt( (void**)&pglDebugMessageControl, "glDebugMessageControl", ext_KHR_debug );
		pGetOptionalFunctionArbExt( (void**)&pglDebugMessageCallback, "glDebugMessageCallback", ext_KHR_debug );
		pGetOptionalFunctionArbExt( (void**)&pglGetDebugMessageLog, "glGetDebugMessageLog", ext_KHR_debug );
		pGetOptionalFunctionArbExt( (void**)&pglPushDebugGroup, "glPushDebugGroup", ext_KHR_debug );
		pGetOptionalFunctionArbExt( (void**)&pglPopDebugGroup, "glPopDebugGroup", ext_KHR_debug );
		pGetOptionalFunctionArbExt( (void**)&pglObjectLabel, "glObjectLabel", ext_KHR_debug );
		
	}else if( pHasExtension[ ext_ARB_debug_output ] ){
		pGetOptionalFunctionArbExt( (void**)&pglDebugMessageControl, "glDebugMessageControl", ext_ARB_debug_output );
		pGetOptionalFunctionArbExt( (void**)&pglDebugMessageCallback, "glDebugMessageCallback", ext_ARB_debug_output );
		pGetOptionalFunctionArbExt( (void**)&pglGetDebugMessageLog, "glGetDebugMessageLog", ext_ARB_debug_output );
	}
	
	// GL_AMD_debug_output : no opengl version
	if( pHasExtension[ ext_AMD_debug_output ] ){
		pGetOptionalFunction( (void**)&pglDebugMessageEnableAMD, "glDebugMessageEnableAMD", ext_AMD_debug_output );
		pGetOptionalFunction( (void**)&pglDebugMessageCallbackAMD, "glDebugMessageCallbackAMD", ext_AMD_debug_output );
		pGetOptionalFunction( (void**)&pglGetDebugMessageLogAMD, "glGetDebugMessageLogAMD", ext_AMD_debug_output );
	}
	
	// GL_NV_transform_feedback : no opengl version
// 	if( pHasExtension[ ext_NV_transform_feedback ] ){
// 		pGetOptionalFunction( (void**)&pglBeginTransformFeedback, "glBeginTransformFeedbackNV", ext_NV_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglEndTransformFeedback, "glEndTransformFeedbackNV", ext_NV_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglBindBufferRange, "glBindBufferRangeNV", ext_EXT_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglBindBufferBase, "glBindBufferBaseNV", ext_NV_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglGetTransformFeedbackVarying, "glGetTransformFeedbackVaryingNV", ext_NV_transform_feedback );
// 		
// 		pGetOptionalFunction( (void**)&pglTransformFeedbackVaryingsNV, "glTransformFeedbackVaryingsNV", ext_NV_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglActiveVaryingNV, "glActiveVaryingNV", ext_NV_transform_feedback );
// 		pGetOptionalFunction( (void**)&pglGetVaryingLocationNV, "glGetVaryingLocationNV", ext_NV_transform_feedback );
// 	}
	
	// GL_NV_transform_feedback2 : no opengl version
// 	if( pHasExtension[ ext_NV_transform_feedback2 ] ){
// 		pGetOptionalFunction( (void**)&pglBindTransformFeedback, "glBindTransformFeedbackNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglDeleteTransformFeedbacks, "glDeleteTransformFeedbacksNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglGenTransformFeedbacks, "glGenTransformFeedbacksNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglIsTransformFeedback, "glIsTransformFeedbackNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglPauseTransformFeedback, "glPauseTransformFeedbackNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglResumeTransformFeedback, "glResumeTransformFeedbackNV", ext_NV_transform_feedback2 );
// 		pGetOptionalFunction( (void**)&pglDrawTransformFeedback, "glDrawTransformFeedbackNV", ext_NV_transform_feedback2 );
// 	}
	
	// GL_NV_transform_feedback3 : no opengl version
// 	if( pHasExtension[ ext_NV_transform_feedback3 ] ){
// 		pGetOptionalFunction( (void**)&pglDrawTransformFeedbackStream, "glDrawTransformFeedbackStreamNV", ext_NV_transform_feedback3 );
// 		pGetOptionalFunction( (void**)&pglBeginQueryIndexed, "glBeginQueryIndexedNV", ext_NV_transform_feedback3 );
// 		pGetOptionalFunction( (void**)&pglEndQueryIndexed, "glEndQueryIndexedNV", ext_NV_transform_feedback3 );
// 		pGetOptionalFunction( (void**)&pglGetQueryIndexediv, "glGetQueryIndexedivNV", ext_NV_transform_feedback3 );
// 	}
	
	// GL_AMD_performance_monitor : no opengl version
	if( pHasExtension[ ext_AMD_performance_monitor ] ){
		pGetOptionalFunction( (void**)&pglGetPerfMonitorGroupsAMD, "glGetPerfMonitorGroupsAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGetPerfMonitorCountersAMD, "glGetPerfMonitorCountersAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGetPerfMonitorGroupStringAMD, "glGetPerfMonitorGroupStringAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGetPerfMonitorCounterStringAMD, "glGetPerfMonitorCounterStringAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGetPerfMonitorCounterInfoAMD, "glGetPerfMonitorCounterInfoAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGenPerfMonitorsAMD, "glGenPerfMonitorsAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglDeletePerfMonitorsAMD, "glDeletePerfMonitorsAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglSelectPerfMonitorCountersAMD, "glSelectPerfMonitorCountersAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglBeginPerfMonitorAMD, "glBeginPerfMonitorAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglEndPerfMonitorAMD, "glEndPerfMonitorAMD", ext_AMD_performance_monitor );
		pGetOptionalFunction( (void**)&pglGetPerfMonitorCounterDataAMD, "glGetPerfMonitorCounterDataAMD", ext_AMD_performance_monitor );
	}
	
	// GL_ARB_shader_image_load_store : 4.6
	if( pHasExtension[ ext_ARB_shader_image_load_store ] ){
		pGetOptionalFunction( (void**)&pglBindImageTexture, "glBindImageTexture", ext_ARB_shader_image_load_store );
	}
	
	// GL_NV_texture_barrier : no opengl version
	#ifndef ANDROID
	if( pHasExtension[ ext_NV_texture_barrier ] ){
		pGetOptionalFunction( (void**)&pglTextureBarrier, "glTextureBarrierNV", ext_NV_texture_barrier );
	}
	#endif
	
	// OpenGL 4.2 : no extension (moved to required)
// 	if( pGLVersion >= evgl4p2 || pGLESVersion >= evgles3p1 ){
// 		pGetRequiredFunction( (void**)&pglMemoryBarrier, "glMemoryBarrier" );
// 	}
	
	// OpenGL 4.3 : no extension
	if( pGLVersion >= evgl4p3 || pGLESVersion >= evgles3p0 ){
		pGetRequiredFunction( (void**)&pglInvalidateFramebuffer, "glInvalidateFramebuffer" );
	}
	
	// OpenGL 3.2 : no extension
	if( pGLVersion >= evgl3p2 || pGLESVersion >= evgles3p0 ){
		pGetRequiredFunction( (void**)&pglGetInteger64v, "glGetInteger64v" );
	}
	
	// GL_ARB_bindless_texture : opengl version 4.6
	if( pHasExtension[ ext_ARB_bindless_texture ] ){
		pGetOptionalFunction( (void**)&pglGetTextureHandleARB, "glGetTextureHandleARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglGetTextureSamplerHandleARB, "glGetTextureSamplerHandleARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglMakeTextureHandleResidentARB, "glMakeTextureHandleResidentARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglMakeTextureHandleNonResidentARB, "glMakeTextureHandleNonResidentARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglGetImageHandleARB, "glGetImageHandleARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglMakeImageHandleResidentARB, "glMakeImageHandleResidentARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglMakeImageHandleNonResidentARB, "glMakeImageHandleNonResidentARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglUniformHandleui64ARB, "glUniformHandleui64ARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglUniformHandleui64vARB, "glUniformHandleui64vARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglProgramUniformHandleui64ARB, "glProgramUniformHandleui64ARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglProgramUniformHandleui64vARB, "glProgramUniformHandleui64vARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglVertexAttribL1ui64ARB, "glVertexAttribL1ui64ARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglVertexAttribL1ui64vARB, "glVertexAttribL1ui64vARB", ext_ARB_bindless_texture );
		pGetOptionalFunction( (void**)&pglGetVertexAttribLui64vARB, "glGetVertexAttribLui64vARB", ext_ARB_bindless_texture );
	}
	
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS && ! defined OS_MACOS
	// GLX_EXT_swap_control
	if( pHasExtension[ ext_GLX_EXT_swap_control ] ){
		pGetOptionalFunction( (void**)&pglXSwapInterval, "glXSwapInterval", "glXSwapIntervalEXT", ext_GLX_EXT_swap_control );
	}
#endif
	
#ifdef OS_W32
	// WGL_EXT_swap_control
	if( pHasExtension[ ext_WGL_EXT_swap_control ] ){
		pGetOptionalFunction( (void**)&pwglSwapInterval, "wglSwapInterval", "wglSwapIntervalEXT", ext_WGL_EXT_swap_control );
	}
#endif
}

void deoglExtensions::pOptionalDisableExtensions(){
	// ext_ARB_compute_shader without ext_ARB_shader_image_load_store is useless
	if( ! pglBindImageTexture ){
		DisableExtension( ext_ARB_compute_shader );
	}
}

void deoglExtensions::pFixBuggedFunctions(){
}



void deoglExtensions::pGetRequiredFunction( void **funcPointer, const char *funcName ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcName );
	
	if( ! fp ){
		pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function address for %s", funcName );
		pHasRequiredFunctions = false;
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetRequiredFunction( void **funcPointer, const char *funcNameBase, const char *funcNameExtension ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameBase );
	
	if( ! fp ){
		fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension );
		
		if( ! fp ){
			pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function address for %s", funcNameBase );
			pHasRequiredFunctions = false;
		}
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetRequiredFunction( void **funcPointer, const char *funcNameBase, const char *funcNameExtension1, const char *funcNameExtension2 ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameBase );
	
	if( ! fp ){
		fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension1 );
		
		if( ! fp ){
			fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension2 );
			
			if( ! fp ){
				pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function address for %s", funcNameBase );
				pHasRequiredFunctions = false;
			}
		}
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetRequiredFunctionExt( void **funcPointer, const char *funcName ){
	const decString funcNameExt = decString( funcName ) + "Ext";
	pGetRequiredFunction( funcPointer, funcName, funcNameExt );
}

void deoglExtensions::pGetRequiredFunctionArbExt( void **funcPointer, const char *funcName ){
	const decString funcNameARB = decString( funcName ) + "ARB";
	const decString funcNameExt = decString( funcName ) + "Ext";
	pGetRequiredFunction( funcPointer, funcName, funcNameARB, funcNameExt );
}



void deoglExtensions::pGetOptionalFunction( void **funcPointer, const char *funcName, int extensionIndex ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcName );
	
	if( ! fp ){
		pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function address for %s although extension %s is listed. This is a driver bug!",
			funcName, vExtensionNames[ extensionIndex ] );
		pHasExtension[ extensionIndex ] = false;
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetOptionalFunction( void **funcPointer, const char *funcNameBase,
const char *funcNameExtension, int extensionIndex ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameBase );
	
	if( ! fp ){
		fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension );
		
		if( ! fp ){
			pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function "
				"address for %s although extension %s is listed. This is a driver bug!",
					funcNameBase, vExtensionNames[ extensionIndex ] );
			pHasExtension[ extensionIndex ] = false;
		}
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetOptionalFunction( void **funcPointer, const char *funcNameBase,
const char *funcNameExtension1, const char *funcNameExtension2, int extensionIndex ){
	// find matching function pointer. this is done no matter if kept later on to do driver bug checking
	void *fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameBase );
	
	if( ! fp ){
		fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension1 );
		
		if( ! fp ){
			fp = ( void* )pRenderThread.GetContext().GetFunctionPointer( funcNameExtension2 );
			
			if( ! fp ){
				pRenderThread.GetLogger().LogErrorFormat( "Failed to get a suitable function "
					"address for %s although extension %s is listed. This is a driver bug!",
						funcNameBase, vExtensionNames[ extensionIndex ] );
				pHasExtension[ extensionIndex ] = false;
			}
		}
	}
	
	// set the function pointer only if not found already. avoids replacing better versions wtih inferior ones
	if( ! *funcPointer ){
		*funcPointer = fp;
	}
}

void deoglExtensions::pGetOptionalFunctionExt( void **funcPointer, const char *funcName, int extensionIndex ) {
	const decString funcNameExt = decString( funcName ) + "Ext";
	pGetOptionalFunction( funcPointer, funcName, funcNameExt, extensionIndex );
}

void deoglExtensions::pGetOptionalFunctionArbExt( void **funcPointer, const char *funcName, int extensionIndex ) {
	const decString funcNameARB = decString( funcName ) + "ARB";
	const decString funcNameExt = decString( funcName ) + "Ext";
	pGetOptionalFunction( funcPointer, funcName, funcNameARB, funcNameExt, extensionIndex );
}

bool deoglExtensions::pVerifyExtensionPresent( eExtensions extension ) const{
	if( ! pHasExtension[ extension ] ){
		pRenderThread.GetLogger().LogErrorFormat( "Missing required extension: %s", vExtensionNames[ extension ] );
	}
	return pHasExtension[ extension ];
}

bool deoglExtensions::pVerifyExtensionPresent( eExtensions extension1, eExtensions extension2 ) const{
	if( ! pHasExtension[ extension1 ] && ! pHasExtension[ extension2 ] ){
		pRenderThread.GetLogger().LogErrorFormat( "Missing required extension: %s or %s",
			vExtensionNames[ extension1 ], vExtensionNames[ extension2 ] );
	}
	return pHasExtension[ extension1 ] || pHasExtension[ extension2 ];
}

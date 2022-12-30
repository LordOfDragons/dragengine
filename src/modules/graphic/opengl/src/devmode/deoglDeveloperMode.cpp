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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglDeveloperMode.h"
#include "deoglDeveloperModeTests.h"
#include "deoglDeveloperModeStats.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../extensions/deoglExtensions.h"
#include "../sptree/deoglSPTree.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/deoglRSkin.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderDefines.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/deoglImage.h"
#include "../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texture2d/deoglRenderableColorTexture.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/exceptions.h>

#include <dragengine/dragengine_configuration.h>



// String tables
//////////////////

struct sUseTextureFormat{
	deoglCapsFmtSupport::eUseTextureFormats type;
	const char *name;
};

static sUseTextureFormat ST_UseTextureFormats[ deoglCapsFmtSupport::UseTextureFormatCount ] = {
	{ deoglCapsFmtSupport::eutfR8, "R 8" },
	{ deoglCapsFmtSupport::eutfR16, "R 16" },
	{ deoglCapsFmtSupport::eutfR8_C, "R 8 Compressed" },
	{ deoglCapsFmtSupport::eutfR16F, "R 16 Float" },
	{ deoglCapsFmtSupport::eutfR32F, "R 32 Float" },
	{ deoglCapsFmtSupport::eutfR8I, "R 8 Integral" },
	{ deoglCapsFmtSupport::eutfR8UI, "R 8 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfR16I, "R 16 Integral" },
	{ deoglCapsFmtSupport::eutfR16UI, "R 16 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfR8_S, "R 8 SNorm" },
	{ deoglCapsFmtSupport::eutfR16_S, "R 16 SNorm" },
	
	{ deoglCapsFmtSupport::eutfRG8, "RG 8" },
	{ deoglCapsFmtSupport::eutfRG16, "RG 16" },
	{ deoglCapsFmtSupport::eutfRG8_C, "RG 8 Compressed" },
	{ deoglCapsFmtSupport::eutfRG16F, "RG 16 Float" },
	{ deoglCapsFmtSupport::eutfRG32F, "RG 32 Float" },
	{ deoglCapsFmtSupport::eutfRG8I, "RG 8 Integral" },
	{ deoglCapsFmtSupport::eutfRG8UI, "RG 8 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRG16I, "RG 16 Integral" },
	{ deoglCapsFmtSupport::eutfRG16UI, "RG 16 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRG8_S, "RG 8 SNorm" },
	{ deoglCapsFmtSupport::eutfRG16_S, "RG 16 SNorm" },
	
	{ deoglCapsFmtSupport::eutfR3G3B2, "RGB 3-3-2" },
	{ deoglCapsFmtSupport::eutfRGB4, "RGB 4" },
	{ deoglCapsFmtSupport::eutfRGB5, "RGB 5" },
	{ deoglCapsFmtSupport::eutfRGB8, "RGB 8" },
	{ deoglCapsFmtSupport::eutfRGB16, "RGB 16" },
	{ deoglCapsFmtSupport::eutfRGB8_C, "RGB 8 Compressed" },
	{ deoglCapsFmtSupport::eutfRG11B10F, "RGB 11-11-10 Float" },
	{ deoglCapsFmtSupport::eutfRGB16F, "RGB 16 Float" },
	{ deoglCapsFmtSupport::eutfRGB32F, "RGB 32 Float" },
	{ deoglCapsFmtSupport::eutfRGB8I, "RGB 8 Integral" },
	{ deoglCapsFmtSupport::eutfRGB8UI, "RGB 8 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRGB16I, "RGB 16 Integral" },
	{ deoglCapsFmtSupport::eutfRGB16UI, "RGB 16 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRGB8_S, "RGB 8 SNorm" },
	{ deoglCapsFmtSupport::eutfRGB16_S, "RGB 16 SNorm" },
	
	{ deoglCapsFmtSupport::eutfRGBA2, "RGBA 2" },
	{ deoglCapsFmtSupport::eutfRGBA4, "RGBA 4" },
	{ deoglCapsFmtSupport::eutfRGB5A1, "RGBA 5-5-5-1" },
	{ deoglCapsFmtSupport::eutfRGBA8, "RGBA 8" },
	{ deoglCapsFmtSupport::eutfRGB10A2, "RGBA 10-10-10-2" },
	{ deoglCapsFmtSupport::eutfRGBA16, "RGBA 16" },
	{ deoglCapsFmtSupport::eutfRGBA8_C, "RGBA 8 Compressed" },
	{ deoglCapsFmtSupport::eutfRGBA16F, "RGBA 16 Float" },
	{ deoglCapsFmtSupport::eutfRGBA32F, "RGBA 32 Float" },
	{ deoglCapsFmtSupport::eutfRGBA8I, "RGBA 8 Integral" },
	{ deoglCapsFmtSupport::eutfRGBA8UI, "RGBA 8 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRGBA16I, "RGBA 16 Integral" },
	{ deoglCapsFmtSupport::eutfRGBA16UI, "RGBA 16 Unsigned Integral" },
	{ deoglCapsFmtSupport::eutfRGBA8_S, "RGBA 8 SNorm" },
	{ deoglCapsFmtSupport::eutfRGBA16_S, "RGBA 16 SNorm" },
	
	{ deoglCapsFmtSupport::eutfDepth, "Depth" },
	{ deoglCapsFmtSupport::eutfDepth_Stencil, "Packed Depth/Stencil" },
	{ deoglCapsFmtSupport::eutfStencil, "Stencil" },
	{ deoglCapsFmtSupport::eutfDepth16, "Depth16" },
	{ deoglCapsFmtSupport::eutfDepthF, "DepthF" },
	{ deoglCapsFmtSupport::eutfDepthF_Stencil, "DepthF_Stencil" }
};

struct sCompressionFormat{
	GLint id;
	const char *name;
};

#define COMPRESSION_FORMAT_COUNT 26
static sCompressionFormat ST_CompressionFormats[ COMPRESSION_FORMAT_COUNT ] = {
	{ GL_COMPRESSED_RGB, "Generic Compressed RGB" },
	{ GL_COMPRESSED_RGBA, "Generic Compressed RGBA" },
	{ GL_COMPRESSED_RGB_S3TC_DXT1, "S3TC DXT1 RGB" },
	{ GL_COMPRESSED_RGBA_S3TC_DXT1, "S3TC DXT1 RGBA" },
	{ GL_COMPRESSED_RGBA_S3TC_DXT3, "S3TC DXT3 RGBA" },
	{ GL_COMPRESSED_RGBA_S3TC_DXT5, "S3TC DXT5 RGBA" },
	{ GL_RGB_S3TC, "S3TC RGB" },
	{ GL_RGB4_S3TC, "S3TC RGB4" },
	{ GL_RGBA_S3TC, "S3TC RGBA" },
	{ GL_RGBA4_S3TC, "S3TC RGBA4" },
	{ GL_COMPRESSED_SRGB, "Compressed sRGB" },
	{ GL_COMPRESSED_SRGB_ALPHA, "Compressed sRGB + Alpha" },
	{ GL_COMPRESSED_SLUMINANCE, "Compressed sLuminance" },
	{ GL_COMPRESSED_SLUMINANCE_ALPHA, "Compressed sLuminance + Alpha" },
	{ GL_COMPRESSED_SRGB_S3TC_DXT1, "Compressed sRGB S3TC DXT1" },
	{ GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1, "Compressed sRGB + Alpha S3TC DXT1" },
	{ GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3, "Compressed sRGB + Alpha S3TC DXT3" },
	{ GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5, "Compressed sRGB + Alpha S3TC DXT5" },
	{ GL_COMPRESSED_LUMINANCE_LATC1, "Luminance LATC1" },
	{ GL_COMPRESSED_SIGNED_LUMINANCE_LATC1, "Signed Luminance LATC1" },
	{ GL_COMPRESSED_LUMINANCE_ALPHA_LATC2, "Luminance + Alpha LATC2" },
	{ GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2, "Signed Luminance + Alpha LATC2" },
	{ GL_COMPRESSED_RED_RGTC1, "Red RGTC1" },
	{ GL_COMPRESSED_SIGNED_RED_RGTC1, "Signed Red RGTC1" },
	{ GL_COMPRESSED_RED_GREEN_RGTC2, "Red + Green RGTC2" },
	{ GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2, "Signed Red + Green RGTC2" }
};

struct sCapability{
	GLint id;
	const char *name;
	int valueCount;
};



// Class deoglDeveloperMode
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDeveloperMode::deoglDeveloperMode( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pEnabled( false ),

pShowVisComponent( false ),
pShowVisLight( false ),
pShowComponentLODLevels( false ),

pShowHeightTerrain( false ),
pShowPropFieldBox( false ),
pShowPropFieldClusters( -1 ),

pShowLightFullBox( false ),
pShowLightBox( false ),
pShowLightVolume( false ),
pShowLightRooms( false ),
pShowLightVisualInfo( -1 ),

pShowTranspLevelCount( false ),
pHighlightTransparentObjects( false ),

pDebugRenderPlan( false ),
pShowMemoryInfo( false ),
pLogMemoryConsumption( false ),

pShowOccMapLevel( -1 ),

pShowEnvMaps( false ),

pShowEnvMapHull( false ),

pDebugEnableLightDepthStencil( false ),

pShowSSAO( false ),

pShowDebugInfo( false ),
pDebugInfoSync( false ),
pDebugInfoLog( false ),
pDebugInfoDetails( 0 ),

pGIShowProbes( false ),
pGIShowProbeOffsets( false ),
pGIShowProbeUpdate( false ),
pGIShowCascade( 0 ),

pTextureDebugImage( NULL ),
pFBODebugImage( NULL ),
pDebugImageUsed( false )
{
	#ifdef OS_ANDROID
	/*
	pEnabled = true;
	pShowDebugInfo = true;
	pDebugInfoDetails = edimModule;
	*/
	#endif
	/*
	pEnabled = true;
	pShowDebugInfo = true;
	pDebugInfoLog = true;
// 	pDebugInfoSync = true;
	pDebugInfoDetails = edimPlanPrepare | edimWorld | edimSolidGeometry | edimTransparency | edimLight | edimLightSky | edimLightPoint | edimLightSpot | edimGI;
// 	pDebugInfoDetails = edimPlanPrepare | edimWorld | edimLight | edimLightPoint;
	*/
}

deoglDeveloperMode::~deoglDeveloperMode(){
	if( pFBODebugImage ){
		delete pFBODebugImage;
	}
	if( pTextureDebugImage ){
		delete pTextureDebugImage;
	}
}



// Management
///////////////

deoglFramebuffer *deoglDeveloperMode::GetFBODebugImageWith( int width, int height ){
	if( pTextureDebugImage ){
		if( pTextureDebugImage->GetWidth() != width || pTextureDebugImage->GetHeight() != height ){
			if( pFBODebugImage ){
				delete pFBODebugImage;
				pFBODebugImage = NULL;
			}
			
			delete pTextureDebugImage;
			pTextureDebugImage = NULL;
		}
	}
	
	if( ! pTextureDebugImage ){
		pTextureDebugImage = new deoglTexture( pRenderThread );
		pTextureDebugImage->SetSize( width, height );
		pTextureDebugImage->SetMipMapped( false );
		pTextureDebugImage->SetFBOFormat( 3, false );
		
		pTextureDebugImage->CreateTexture();
		
		if( ! pFBODebugImage ){
			pFBODebugImage = new deoglFramebuffer( pRenderThread, false );
			
			pRenderThread.GetFramebuffer().Activate( pFBODebugImage );
			
			pFBODebugImage->AttachColorTexture( 0, pTextureDebugImage );
			const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
			OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			pFBODebugImage->Verify();
		}
	}
	
	pDebugImageUsed = true;
	
	return pFBODebugImage;
}

void deoglDeveloperMode::CheckDebugImageUse(){
	if( ! pDebugImageUsed ){
		if( pFBODebugImage ){
			delete pFBODebugImage;
			pFBODebugImage = NULL;
		}
		
		if( pTextureDebugImage ){
			delete pTextureDebugImage;
			pTextureDebugImage = NULL;
		}
	}
	
	pDebugImageUsed = false;
}



bool deoglDeveloperMode::ExecuteCommand( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	bool result = false;
	
	pRenderThread.Freeze();
	
	try{
		if( command.MatchesArgumentAt( 0, "dm_enable" ) ){
			pCmdEnable( command, answer );
			result = true;
			
		}else if( pEnabled ){
			if( command.MatchesArgumentAt( 0, "dm_help" ) ){
				pCmdHelp( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_vis_component" ) ){
				pCmdShowVisComponent( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_vis_light" ) ){
				pCmdShowVisLight( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_component_lod_levels" ) ){
				pCmdShowComponentLodLevels( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_height_terrain" ) ){
				pCmdShowHeightTerrain( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_propfield_box" ) ){
				pCmdShowPropFieldBox( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_propfield_clusters" ) ){
				pCmdShowPropFieldClusters( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_quick_test" ) ){
				pCmdQuickTest( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_opengl_caps" ) ){
				pCmdOpenGLCaps( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_stats" ) ){
				pCmdStats( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_capabilities" ) ){
				pCmdCapabilities( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_meminfo" ) ){
				pCmdMemoryInfo( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_snapshot" ) ){
				pCmdDebugSnapshot( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_light_full_box" ) ){
				pCmdShowLightFullBox( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_light_box" ) ){
				pCmdShowLightBox( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_light_volume" ) ){
				pCmdShowLightVolume( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_light_rooms" ) ){
				pCmdShowLightRooms( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_light_visual_info" ) ){
				pCmdShowLightVisualInfo( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_transp_layer_count" ) ){
				pCmdShowTranspLayerCount( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_highlight_transparent_objects" ) ){
				pCmdHighlightTransparentObjects( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_renderplan" ) ){
				pCmdDebugRenderPlan( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_memory_info" ) ){
				pCmdShowMemoryInfo( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_log_memory_consumption" ) ){
				pCmdLogMemoryConsumption( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_occmap_level" ) ){
				pCmdShowOccMapLevel( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_envmaps" ) ){
				pCmdShowEnvMaps( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_envmap_hull" ) ){
				pCmdShowEnvMapHull( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_ssao" ) ){
				pCmdShowSSAO( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_enable_light_depth_stencil" ) ){
				pCmdDebugEnableLightDepthStencil( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_tests" ) ){
				pCmdTests( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_show_debug_info" ) ){
				pCmdShowDebugInfo( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_info_sync" ) ){
				pCmdDebugInfoSync( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_info_log" ) ){
				pCmdDebugInfoLog( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_debug_info_details" ) ){
				pCmdDebugInfoDetails( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_gi_show_probes" ) ){
				pCmdGIShowProbes( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_gi_show_probe_offsets" ) ){
				pCmdGIShowProbeOffsets( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_gi_show_probe_update" ) ){
				pCmdGIShowProbeUpdate( command, answer );
				result = true;
				
			}else if( command.MatchesArgumentAt( 0, "dm_gi_show_cascade" ) ){
				pCmdGIShowCascade( command, answer );
				result = true;
			}
		}
		
		pRenderThread.Unfreeze();
		
	}catch( const deException &exception ){
		pRenderThread.Unfreeze();
		answer.AppendFromUTF8( exception.FormatOutput().Join( "\n" ) );
		return true;
	}
	
	return result;
}



// Private functions
//////////////////////

void deoglDeveloperMode::pCmdHelp( const decUnicodeArgumentList &, decUnicodeString &answer ){
	answer.SetFromUTF8( "dm_help => Displays this help screen.\n" );
	answer.AppendFromUTF8( "dm_capabilities => Displays hardware capabilities.\n" );
	answer.AppendFromUTF8( "dm_debug_enable_light_depth_stencil => Enable depth and stencil test using depth copy for lighting passes.\n" );
	answer.AppendFromUTF8( "dm_debug_renderplan [1|0] => Display render plan debug information.\n" );
	answer.AppendFromUTF8( "dm_debug_snapshot [num] => Creates a debug snapshot of something given by num.\n" );
	answer.AppendFromUTF8( "dm_enable_envmap_fresnel [1|0] => Enable environment map fresnel.\n" );
	answer.AppendFromUTF8( "dm_height_terrain => Show LOD levels and bounding boxes of the height terrain.\n" );
	answer.AppendFromUTF8( "dm_highlight_transparent_objects [1|0] => Highlight transparent objects.\n" );
	answer.AppendFromUTF8( "dm_meminfo => Displays memory information.\n" );
	answer.AppendFromUTF8( "dm_opengl_caps => Display OpenGL capabilities.\n" );
	answer.AppendFromUTF8( "dm_show_component_lod_levels [1|0] => Displays the component lod levels.\n" );
	answer.AppendFromUTF8( "dm_show_envmaps [1|0] => Show environment maps.\n" );
	answer.AppendFromUTF8( "dm_show_envmap_hull [1|0] => Show environment map hull.\n" );
	answer.AppendFromUTF8( "dm_show_light_box [1|0] => Show light box.\n" );
	answer.AppendFromUTF8( "dm_show_light_full_box [1|0] => Show light full box.\n" );
	answer.AppendFromUTF8( "dm_show_light_rooms [1|0] => Show light rooms.\n" );
	answer.AppendFromUTF8( "dm_show_light_visual_info [index] => Show visual info for a light or -1 to disable.\n" );
	answer.AppendFromUTF8( "dm_show_light_volume [1|0] => Show light volumes.\n" );
	answer.AppendFromUTF8( "dm_show_memory_info [1|0] => Display memory information.\n" );
	answer.AppendFromUTF8( "dm_show_occmap_level [-1|0..maxLevel] => Show occ map level or -1 to disable.\n" );
	answer.AppendFromUTF8( "dm_show_propfield_box [1|0] => Show boxes around prop fields.\n" );
	answer.AppendFromUTF8( "dm_show_propfield_clusters [-1|0..] => Show clusters for prop field type with the given index.\n" );
	answer.AppendFromUTF8( "dm_show_static_outside_faces [1|0] => Show static outside faces.\n" );
	answer.AppendFromUTF8( "dm_show_ssao [1|0] => Show SSAO texture.\n" );
	answer.AppendFromUTF8( "dm_show_transp_layer_count [1|0] => Show the number of transparency layers.\n" );
	answer.AppendFromUTF8( "dm_show_vis_component [1|0] => Displays the visibility of components.\n" );
	answer.AppendFromUTF8( "dm_show_vis_light [1|0] => Displays the visibility of lights.\n" );
	answer.AppendFromUTF8( "dm_stats => Displays various stats.\n" );
	answer.AppendFromUTF8( "dm_tests => Runs various tests.\n" );
	answer.AppendFromUTF8( "dm_show_debug_info [1|0] => Show debug information and enable timing measurements.\n" );
	answer.AppendFromUTF8( "dm_debug_info_sync [1|0] => Call glFinish before each debug timing measurement for true GPU time measuring.\n" );
	answer.AppendFromUTF8( "dm_debug_info_log [1|0] => Log debug timing measurement for each frame.\n" );
	answer.AppendFromUTF8( "dm_debug_info_details [list|+name...|-name...] => Debug info details to show.\n" );
	answer.AppendFromUTF8( "dm_gi_show_probes [1|0] => Display GI probes.\n" );
	answer.AppendFromUTF8( "dm_gi_show_probe_offsets [1|0] => Display GI probe offsets.\n" );
	answer.AppendFromUTF8( "dm_gi_show_probe_update [1|0] => Display GI probe update information.\n" );
	answer.AppendFromUTF8( "dm_gi_show_cascade [0..maxCascaded] => GI Cascade to show.\n" );
}

void deoglDeveloperMode::pCmdEnable( const decUnicodeArgumentList &, decUnicodeString &answer ){
	/*if( command.GetArgumentCount() == 2 ){
		if( command.MatchesArgumentAt( 1, "who is your daddy" ) ){
			answer.SetFromUTF8( "Developer mode is now enabled.\n" );
			pEnabled = true;
			
		}else{
			answer.SetFromUTF8( "Wrong magic word :P\n" );
		}
		
	}else{
		answer.SetFromUTF8( "Syntax error.\n" );
	}*/
	answer.SetFromUTF8( "Developer mode is now enabled.\n" );
	pEnabled = true;
}



void deoglDeveloperMode::pCmdShowVisComponent( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowVisComponent, "dm_show_vis_component" );
}

void deoglDeveloperMode::pCmdShowVisLight( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowVisLight, "dm_show_vis_light" );
}

void deoglDeveloperMode::pCmdShowComponentLodLevels( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowComponentLODLevels, "dm_show_component_lod_levels" );
}



void deoglDeveloperMode::pCmdShowHeightTerrain( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowHeightTerrain, "dm_height_terrain" );
}

void deoglDeveloperMode::pCmdShowPropFieldBox( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowPropFieldBox, "dm_show_propfield_box" );
}

void deoglDeveloperMode::pCmdShowPropFieldClusters( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowPropFieldClusters = command.GetArgumentAt( 1 )->ToInt();
	}
	
	decString text;
	text.Format( "dm_show_propfield_clusters = %i\n", pShowPropFieldClusters );
	answer.AppendFromUTF8( text.GetString() );
}



void deoglDeveloperMode::pCmdQuickTest( const decUnicodeArgumentList &, decUnicodeString &answer ){
	answer.AppendFromUTF8( "I'm not implemented anymore. Wanna have a beer instead?\n" );
}



void deoglDeveloperMode::pCmdOpenGLCaps( const decUnicodeArgumentList &, decUnicodeString &answer ){
	// this is not working since all opengl calls would have to be done in the render thread
	return;
	
	
	
	GLint i, j, f, formatCount, *formats = NULL;
	GLint resultsInt[ 2 ];
	decString text;
	
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &formatCount ) );
	answer.AppendFromUTF8( "- Texture Compression Formats:\n" );
	if( formatCount > 0 ){
		try{
			formats = new GLint[ formatCount ];
			if( ! formats ) DETHROW( deeOutOfMemory );
			
			OGL_CHECK( pRenderThread, glGetIntegerv( GL_COMPRESSED_TEXTURE_FORMATS, formats ) );
			for( f=0; f<formatCount; f++ ){
				for( i=0; i<COMPRESSION_FORMAT_COUNT; i++ ){
					if( formats[ f ] == ST_CompressionFormats[ i ].id ){
						text.Format( "   - %s\n", ST_CompressionFormats[ i ].name );
						break;
					}
				}
				
				if( i == COMPRESSION_FORMAT_COUNT ){
					text.Format( "   - Unknown ( ID %x )\n", formats[ f ] );
				}
				
				answer.AppendFromUTF8( text.GetString() );
			}
			
			delete [] formats;
			
		}catch( const deException & ){
			if( formats ) delete [] formats;
			throw;
		}
	}
	
	const int capabilityCount = 27;
	const sCapability capabilities[ capabilityCount ] = {
		{ GL_MAX_TEXTURE_SIZE, "GL: Maximum texture size", 1 },
		{ GL_MAX_VIEWPORT_DIMS, "GL: Maximum viewport size", 2 },
		{ GL_MAX_TEXTURE_UNITS, "GL: Maximum number of fixed function texture units", 1 },
		
		{ GL_MAX_TEXTURE_IMAGE_UNITS, "Shaders: Maximum number of texture image units", 1 },
		{ GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "Shaders: Maximum number of combined texture image units", 1 },
		{ GL_MAX_TEXTURE_COORDS, "Shaders: Maximum number of texture coordinates", 1 },
		
		{ GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "Vertex Shader: Maximum number of texture image units", 1 },
		{ GL_MAX_VERTEX_ATTRIBS, "Vertex Shader: Maximum number of vertex attributes", 1 },
		{ GL_MAX_VARYING_FLOATS, "Vertex Shader: Maximum number of varying floats", 1 },
		{ GL_MAX_VERTEX_UNIFORM_COMPONENTS, "Vertex Shader: Maximum number of vertex uniform components", 1 },
		
		{ GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, "Geometry Shader: Maximum number of geometry texture image units", 1 },
		{ GL_MAX_GEOMETRY_VARYING_COMPONENTS, "Geometry Shader: Maximum number of geometry varying components", 1 },
		{ GL_MAX_VERTEX_VARYING_COMPONENTS, "Geometry Shader: Maximum number of vertex varying components", 1 },
		{ GL_MAX_VARYING_COMPONENTS, "Geometry Shader: Maximum number of varying components", 1 },
		{ GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, "Geometry Shader: Maximum number of geometry uniform components", 1 },
		{ GL_MAX_GEOMETRY_OUTPUT_VERTICES, "Geometry Shader: Maximum number of geometry output vertices", 1 },
		{ GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, "Geometry Shader: Maximum number of total geometry output components", 1 },
		
		{ GL_MAX_ARRAY_TEXTURE_LAYERS, "Texture Array: Maximum number of texture layers", 1 },
		
		{ GL_MAX_VERTEX_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum vertex uniform blocks", 1 },
		{ GL_MAX_GEOMETRY_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum geometry uniform blocks", 1 },
		{ GL_MAX_FRAGMENT_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum fragment uniform blocks", 1 },
		{ GL_MAX_COMBINED_UNIFORM_BLOCKS, "Uniform Buffer Object: Maximum combined uniform blocks", 1 },
		{ GL_MAX_UNIFORM_BUFFER_BINDINGS, "Uniform Buffer Object: Maximum bindings", 1 },
		{ GL_MAX_UNIFORM_BLOCK_SIZE, "Uniform Buffer Object: Maximum block size", 1 },
		{ GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, "Uniform Buffer Object: Maximum vertex components", 1 },
		{ GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, "Uniform Buffer Object: Maximum geometry components", 1 },
		{ GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, "Uniform Buffer Object: Maximum fragment components", 1 }
	};
	
	for( i=0; i<capabilityCount; i++ ){
		OGL_CHECK( pRenderThread, glGetIntegerv( capabilities[ i ].id, &resultsInt[ 0 ] ) );
		
		text.Format( "- %s:", capabilities[ i ].name );
		for( j=0; j<capabilities[ i ].valueCount; j++ ){
			text.AppendFormat( " %i", resultsInt[ j ] );
		}
		text.AppendCharacter( '\n' );
		answer.AppendFromUTF8( text.GetString() );
	}
	
	OGL_CHECK( pRenderThread, glGetIntegerv( GL_TEXTURE_COMPRESSION_HINT, &resultsInt[ 0 ] ) );
	if( resultsInt[ 0 ] == GL_FASTEST ){
		answer.AppendFromUTF8( "- Texture Compression Hint: Fastest\n" );
		
	}else if( resultsInt[ 0 ] == GL_NICEST ){
		answer.AppendFromUTF8( "- Texture Compression Hint: Nicest\n" );
		
	}else if( resultsInt[ 0 ] == GL_DONT_CARE ){
		answer.AppendFromUTF8( "- Texture Compression Hint: Don't Care\n" );
		
	}else{
		text.Format( "- Texture Compression Hint: Unknown ( ID %x )\n", resultsInt[ 0 ] );
		answer.AppendFromUTF8( text.GetString() );
	}
}



void deoglDeveloperMode::pCmdStats( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglDeveloperModeStats stats( pRenderThread );
	stats.Stats( command, answer );
}



void deoglDeveloperMode::pCmdCapabilities( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglCapabilities &caps = pRenderThread.GetCapabilities();
	const deoglCapsTextureFormat *format;
	int f, formatCount;
	
	if( command.GetArgumentCount() == 2 ){
		if( command.MatchesArgumentAt( 1, "tex2D_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundTex2DFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found 2D-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "tex2D_use" ) ){
			answer.SetFromUTF8( "Used 2D-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseTex2DFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "texCube_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundTexCubeFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found Cube-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "texCube_use" ) ){
			answer.SetFromUTF8( "Used Cube-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseTexCubeFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "arrTex_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundArrayTexFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found Array-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "arrTex_use" ) ){
			answer.SetFromUTF8( "Used Array-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseArrayTexFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboTex2D_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundFBOTex2DFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found FBO 2D-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboTex2D_use" ) ){
			answer.SetFromUTF8( "Used FBO 2D-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseFBOTex2DFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboTexCube_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundFBOTexCubeFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found FBO Cube-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboTexCube_use" ) ){
			answer.SetFromUTF8( "Used FBO Cube-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseFBOTexCubeFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboArrTex_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundFBOArrayTexFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found FBO Array-Texture formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "fboArrTex_use" ) ){
			answer.SetFromUTF8( "Used FBO Array-Texture formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseFBOArrayTexFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "renBuf_found" ) ){
			const deoglCapsTextureFormatList &formatList = caps.GetFormats().GetFoundRenBufFormats();
			formatCount = formatList.GetFormatCount();
			
			answer.SetFromUTF8( "Found RenderBuffer formats:\n" );
			for( f=0; f<formatCount; f++ ){
				format = formatList.GetFormatAt( f );
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( format->GetName().GetString() );
				answer.AppendFromUTF8( "\n" );
			}
			return;
			
		}else if( command.MatchesArgumentAt( 1, "renBuf_use" ) ){
			answer.SetFromUTF8( "Used RenderBuffer formats:\n" );
			for( f=0; f<deoglCapsFmtSupport::UseTextureFormatCount; f++ ){
				format = caps.GetFormats().GetUseRenBufFormatFor( ST_UseTextureFormats[ f ].type );
				
				answer.AppendFromUTF8( "- " );
				answer.AppendFromUTF8( ST_UseTextureFormats[ f ].name );
				answer.AppendFromUTF8( " => " );
				if( format ){
					answer.AppendFromUTF8( format->GetName().GetString() );
					
				}else{
					answer.AppendFromUTF8( "< Unsupported >" );
				}
				answer.AppendFromUTF8( "\n" );
			}
			return;
		}
	}
	
	answer.SetFromUTF8( "dm_capabilities <mode>\n" );
	answer.AppendFromUTF8( "where <mode> can be:\n" );
	answer.AppendFromUTF8( "tex2D_found => Found working 2D-Texture formats.\n" );
	answer.AppendFromUTF8( "tex2D_use => Used 2D-Texture formats.\n" );
	answer.AppendFromUTF8( "texCube_found => Found working Cube-Texture formats.\n" );
	answer.AppendFromUTF8( "texCube_use => Used Cube-Texture formats.\n" );
	answer.AppendFromUTF8( "arrTex_found => Found working Array-Texture formats.\n" );
	answer.AppendFromUTF8( "arrTex_use => Used Array-Texture formats.\n" );
	answer.AppendFromUTF8( "fboTex2D_found => Found working FBO 2D-Texture formats.\n" );
	answer.AppendFromUTF8( "fboTex2D_use => Used FBO 2D-Texture formats.\n" );
	answer.AppendFromUTF8( "fboTexCube_found => Found working FBO Cube-Texture formats.\n" );
	answer.AppendFromUTF8( "fboTexCube_use => Used FBO Cube-Texture formats.\n" );
	answer.AppendFromUTF8( "fboArrTex_found => Found working FBO Array-Texture formats.\n" );
	answer.AppendFromUTF8( "fboArrTex_use => Used FBO Array-Texture formats.\n" );
	answer.AppendFromUTF8( "renBuf_found => Found working RenderBuffer formats.\n" );
	answer.AppendFromUTF8( "renBuf_use => Used RenderBuffer formats.\n" );
}

void deoglDeveloperMode::pCmdMemoryInfo( const decUnicodeArgumentList &, decUnicodeString &answer ){
	const deoglExtensions &extensions = pRenderThread.GetExtensions();
	
	if( extensions.GetHasExtension( deoglExtensions::ext_ATI_meminfo ) ){
		GLint values[ 4 ];
		decString text;
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_VBO_FREE_MEMORY_ATI, &values[ 0 ] ) );
		text.Format( "VBO: total=%ikb largestFree=%ikb totalAux=%ikb largestFreeAux=%ikb\n",
			values[ 0 ], values[ 1 ], values[ 2 ], values[ 3 ] );
		answer.SetFromUTF8( text.GetString() );
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_TEXTURE_FREE_MEMORY_ATI, &values[ 0 ] ) );
		text.Format( "Texture: total=%ikb largestFree=%ikb totalAux=%ikb largestFreeAux=%ikb\n",
			values[ 0 ], values[ 1 ], values[ 2 ], values[ 3 ] );
		answer.AppendFromUTF8( text.GetString() );
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_RENDERBUFFER_FREE_MEMORY_ATI, &values[ 0 ] ) );
		text.Format( "Renderbuffer: total=%ikb largestFree=%ikb totalAux=%ikb largestFreeAux=%ikb\n",
			values[ 0 ], values[ 1 ], values[ 2 ], values[ 3 ] );
		answer.AppendFromUTF8( text.GetString() );
		
	}else{
		answer.SetFromUTF8( "GL_ATI_meminfo not supported.\n" );
	}
}

void deoglDeveloperMode::pCmdDebugSnapshot( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() != 2 ){
		answer.SetFromUTF8( "dm_debug_snapshot <num>\n" );
		answer.AppendFromUTF8( "where <num> can be:\n" );
		answer.AppendFromUTF8( "1 => Render Task ( solid ).\n" );
		answer.AppendFromUTF8( "2 => Environment Map.\n" );
		answer.AppendFromUTF8( "10 => Transparency Counter Pass.\n" );
		answer.AppendFromUTF8( "20 => Transparency Passes.\n" );
		answer.AppendFromUTF8( "50 => Tone Mapping.\n" );
		answer.AppendFromUTF8( "60 => Reflection.\n" );
		answer.AppendFromUTF8( "90 => Light Sky.\n" );
		answer.AppendFromUTF8( "101 => Print World Render Tasks (depth, geometry).\n" );
		answer.AppendFromUTF8( "110 => Light Sky.\n" );
		answer.AppendFromUTF8( "8888 => Special.\n" );
		
	}else{
		deoglConfiguration &config = pRenderThread.GetConfiguration();
		decString text;
		
		config.SetDebugSnapshot( command.GetArgumentAt( 1 )->ToInt() );
		
		text.Format( "dm_debug_shapshot %i\n", config.GetDebugSnapshot() );
		answer.SetFromUTF8( text.GetString() );
	}
}

void deoglDeveloperMode::pCmdShowLightFullBox( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowLightFullBox, "dm_show_light_full_box" );
}

void deoglDeveloperMode::pCmdShowLightBox( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowLightBox, "dm_show_light_box" );
}

void deoglDeveloperMode::pCmdShowLightVolume( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowLightVolume, "dm_show_light_volume" );
}

void deoglDeveloperMode::pCmdShowLightRooms( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowLightRooms, "dm_show_light_rooms" );
}

void deoglDeveloperMode::pCmdShowLightVisualInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowLightVisualInfo = command.GetArgumentAt( 1 )->ToInt();
	}
	
	decString text;
	text.Format( "dm_show_light_visual_info = %i\n", pShowLightVisualInfo );
	answer.AppendFromUTF8( text.GetString() );
}

void deoglDeveloperMode::pCmdShowTranspLayerCount( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowTranspLevelCount, "dm_show_transp_layer_count" );
}

void deoglDeveloperMode::pCmdHighlightTransparentObjects( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pHighlightTransparentObjects, "dm_highlight_transparent_objects" );
}

void deoglDeveloperMode::pCmdDebugRenderPlan( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pDebugRenderPlan, "dm_debug_renderplan" );
}

void deoglDeveloperMode::pCmdShowMemoryInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowMemoryInfo, "dm_show_memory_info" );
}

void deoglDeveloperMode::pCmdLogMemoryConsumption( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pLogMemoryConsumption, "dm_log_memory_consumption" );
}



void deoglDeveloperMode::pCmdShowOccMapLevel( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( command.GetArgumentCount() == 2 ){
		pShowOccMapLevel = command.GetArgumentAt( 1 )->ToInt();
	}
	
	decString text;
	text.Format( "dm_show_occmap_level = %i\n", pShowOccMapLevel );
	answer.AppendFromUTF8( text.GetString() );
}



void deoglDeveloperMode::pCmdShowEnvMaps( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowEnvMaps, "dm_show_envmaps" );
}

void deoglDeveloperMode::pCmdShowEnvMapHull( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowEnvMapHull, "dm_show_envmap_hull" );
}



void deoglDeveloperMode::pCmdShowSSAO( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pShowSSAO, "dm_show_ssao" );
}



void deoglDeveloperMode::pCmdDebugEnableLightDepthStencil( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pDebugEnableLightDepthStencil, "dm_debug_enable_light_depth_stencil" );
}



void deoglDeveloperMode::pCmdTests( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	deoglDeveloperModeTests tests( pRenderThread );
	tests.Tests( command, answer );
}



void deoglDeveloperMode::pCmdShowDebugInfo( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pBaseCmdBool( command, answer, pShowDebugInfo, "dm_show_debug_info" ) ){
		pRenderThread.DevModeDebugInfoChanged();
	}
}

void deoglDeveloperMode::pCmdDebugInfoSync( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pBaseCmdBool( command, answer, pDebugInfoSync, "dm_debug_info_sync" ) ){
		pRenderThread.DevModeDebugInfoChanged();
	}
}

void deoglDeveloperMode::pCmdDebugInfoLog( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	if( pBaseCmdBool( command, answer, pDebugInfoLog, "dm_debug_info_log" ) ){
		pRenderThread.DevModeDebugInfoChanged();
	}
}

void deoglDeveloperMode::pCmdDebugInfoDetails( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	const int count = command.GetArgumentCount();
	int newDetails = pDebugInfoDetails;
	int i;
	
	if( count == 2 && command.GetArgumentAt( 1 )->ToUTF8() == "list" ){
		answer.AppendFromUTF8( "module|m planPrepare|pp canvas|c world|w solidGeometry|sg "
			"transparency|t light|l lightSky|lsky lightPoint|lp lightSpot|ls frameLimiter|fl\n" );
		
	}else{
		for( i=1; i<count; i++ ){
			const decUnicodeString &ucDetail = *command.GetArgumentAt( i );
			decString detail( ucDetail.ToUTF8() );
			bool add = true;
			int value = 0;
			
			if( detail.GetAt( 0 ) == '+' ){
				detail = detail.GetMiddle( 1 );
				
			}else if( detail.GetAt( 0 ) == '-' ){
				add = false;
				detail = detail.GetMiddle( 1 );
			}
			
			if( detail == "module" || detail == "m" ){
				value = edimModule;
				
			}else if( detail == "planPrepare" || detail == "pp" ){
				value = edimPlanPrepare;
				
			}else if( detail == "canvas" || detail == "c" ){
				value = edimCanvas;
				
			}else if( detail == "world" || detail == "w" ){
				value = edimWorld;
				
			}else if( detail == "solidGeometry" || detail == "sg" ){
				value = edimSolidGeometry;
				
			}else if( detail == "transparency" || detail == "t" ){
				value = edimTransparency;
				
			}else if( detail == "light" || detail == "l" ){
				value = edimLight;
				
			}else if( detail == "lightSky" || detail == "lsky" ){
				value = edimLightSky;
				
			}else if( detail == "lightPoint" || detail == "lp" ){
				value = edimLightPoint;
				
			}else if( detail == "lightSpot" || detail == "ls" ){
				value = edimLightSpot;
				
			}else if( detail == "frameLimiter" || detail == "fl" ){
				value = edimFrameLimiter;
				
			}else if( detail == "gi" ){
				value = edimGI;
				
			}else if( detail == "all" ){ // temporary
				value = edimModule | edimPlanPrepare | edimCanvas | edimWorld | edimSolidGeometry
					| edimTransparency | edimLight | edimLightSky | edimLightPoint | edimLightSpot
					| edimFrameLimiter | edimGI;
				
			}else{
				decString text;
				text.Format( "Invalid name '%s'\n", detail.GetString() );
				answer.AppendFromUTF8( text.GetString() );
				break;
			}
			
			if( add ){
				newDetails |= value;
				
			}else{
				newDetails &= ~value;
			}
		}
	}
	
	if( newDetails != pDebugInfoDetails ){
		pDebugInfoDetails = newDetails;
		pRenderThread.DevModeDebugInfoChanged();
	}
	
	answer.AppendFromUTF8( "dm_debug_info_details =" );
	if( ( pDebugInfoDetails & edimModule ) == edimModule ){
		answer.AppendFromUTF8( " module" );
	}
	if( ( pDebugInfoDetails & edimPlanPrepare ) == edimPlanPrepare ){
		answer.AppendFromUTF8( " planPrepare" );
	}
	if( ( pDebugInfoDetails & edimCanvas ) == edimCanvas ){
		answer.AppendFromUTF8( " canvas" );
	}
	if( ( pDebugInfoDetails & edimWorld ) == edimWorld ){
		answer.AppendFromUTF8( " world" );
	}
	if( ( pDebugInfoDetails & edimSolidGeometry ) == edimSolidGeometry ){
		answer.AppendFromUTF8( " solidGeometry" );
	}
	if( ( pDebugInfoDetails & edimTransparency ) == edimTransparency ){
		answer.AppendFromUTF8( " transparency" );
	}
	if( ( pDebugInfoDetails & edimLight ) == edimLight ){
		answer.AppendFromUTF8( " light" );
	}
	if( ( pDebugInfoDetails & edimLightSky ) == edimLightSky ){
		answer.AppendFromUTF8( " lightSky" );
	}
	if( ( pDebugInfoDetails & edimLightPoint ) == edimLightPoint ){
		answer.AppendFromUTF8( " lightPoint" );
	}
	if( ( pDebugInfoDetails & edimLightSpot ) == edimLightSpot ){
		answer.AppendFromUTF8( " lightSpot" );
	}
	if( ( pDebugInfoDetails & edimLightSpot ) == edimLightSpot ){
		answer.AppendFromUTF8( " lightSpot" );
	}
	if( ( pDebugInfoDetails & edimFrameLimiter ) == edimFrameLimiter ){
		answer.AppendFromUTF8( " frameLimiter" );
	}
	if( ( pDebugInfoDetails & edimGI ) == edimGI ){
		answer.AppendFromUTF8( " gi" );
	}
	answer.AppendCharacter( '\n' );
}

void deoglDeveloperMode::pCmdGIShowProbes( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pGIShowProbes, "dm_gi_show_probes" );
}

void deoglDeveloperMode::pCmdGIShowProbeOffsets( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pGIShowProbeOffsets, "dm_gi_show_probe_offsets" );
}

void deoglDeveloperMode::pCmdGIShowProbeUpdate( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdBool( command, answer, pGIShowProbeUpdate, "dm_gi_show_probe_update" );
}

void deoglDeveloperMode::pCmdGIShowCascade( const decUnicodeArgumentList &command, decUnicodeString &answer ){
	pBaseCmdInt( command, answer, pGIShowCascade, "dm_gi_show_cascade" );
}


bool deoglDeveloperMode::pBaseCmdBool( const decUnicodeArgumentList &command,
decUnicodeString &answer, bool &variable, const char *commandName ){
	const bool oldValue = variable;
	
	if( command.GetArgumentCount() == 2 ){
		const decString value( command.GetArgumentAt( 1 )->GetLower().ToUTF8() );
		variable = value == "1" || value == "yes" || value == "true" || value == "on";
	}
	
	decString text;
	text.Format( "%s = %s\n", commandName, variable ? "true" : "false" );
	answer.AppendFromUTF8( text );
	
	return variable != oldValue;
}

bool deoglDeveloperMode::pBaseCmdInt( const decUnicodeArgumentList &command,
decUnicodeString &answer, int &variable, const char *commandName ){
	const int oldValue = variable;
	
	if( command.GetArgumentCount() == 2 ){
		variable = command.GetArgumentAt( 1 )->ToInt();
	}
	
	decString text;
	text.Format( "%s = %d\n", commandName, variable );
	answer.AppendFromUTF8( text );
	
	return variable != oldValue;
}

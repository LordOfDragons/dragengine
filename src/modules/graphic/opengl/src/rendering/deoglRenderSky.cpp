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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderSky.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"

#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../light/deoglRLight.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../sky/deoglRSky.h"
#include "../sky/deoglRSkyLayer.h"
#include "../sky/deoglRSkyInstance.h"
#include "../sky/deoglRSkyInstanceLayer.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



/*

informations about sphere layer calculations
============================================

n = texture normal
transformation texture normal to screen pixel
---------------------------------------------

a = ( matLayer * matRotCam ) * n
b = matProj * a
c = ( b.x / b.w, b.y / b.w, b.z / b.w )
d = ( sx, sy, z )

matProj.a11 = 1 / tan( fov * 0.5 )
matProj.a22 = aspectRatio / tan( fov * fovRatio * 0.5 )
matProj.a33 = ( zf + zn ) / ( zf - zn )
matProj.a34 = -2 * zf * zn / ( zf - zn )
matProj.a43 = 1

b.x = matProj.a11 * a.x
b.y = matProj.a22 * a.y
b.z = matProj.a33 * a.z + matProj.a34
b.w = matProj.a43 * a.z = a.z



transformation back from screen pixel to texture normal
-------------------------------------------------------

b = ( cx * b.w, cy * b.w, c.z * b.w )

// assume point on near plane => z=zn, c.z=1, b.w=zn => a.z=zn
c = ( sx, sy, 1 )
b = ( sx * zn, sy * zn, zn )

// reverse the projection matrix
b.x = matProj.a11 * a.x
a.x = b.x / matProj.a11 = b.x * c1
c1 = 1 / matProj.a11
c1 = 1 / ( 1 / tan( fov * 0.5 ) )
c1 = tan( fov * 0.5 )

b.y = matProj.a22 * a.y
a.y = b.y / matProj.a22 = b.y * c2
c2 = 1 / matProj.a22
c2 = 1 / ( aspectRatio / tan( fov * fovRatio * 0.5 ) )
c2 = tan( fov * fovRatio * 0.5 ) / aspectRatio

b.z = matProj.a33 * a.z + matProj.a34
a.z = ( b.z - matProj.a34 ) / matProj.a33 = b.z * c3 + c4
c3 = 1 / matProj.a33
c3 = 1 / ( ( zf + zn ) / ( zf - zn ) )
c3 = ( zf - zn ) / ( zf + zn )
c4 = -matProj.a34 / matProj.a33
c4 = -( -2 * zf * zn / ( zf - zn ) ) / ( ( zf + zn ) / ( zf - zn ) )
c4 = ( 2 * zf * zn / ( zf - zn ) ) * ( ( zf - zn ) / ( zf + zn ) )
c4 = 2 * zf * zn / ( zf + zn )

c1 = tan( fov * 0.5 )
c2 = tan( fov * fovRatio * 0.5 ) / aspectRatio
c3 = ( zf - zn ) / ( zf + zn )
c4 = 2 * zf * zn / ( zf + zn )

// transform back through the projection matrix
a = matProj^-1 * b

a.x = b.x * c1
a.y = b.y * c2
a.z = b.z * c3 + c4

a.x = sx * zn * c1
a.y = sy * zn * c2
a.z = zn * c3 + c4

// modifying the constants
c1' = c1 * zn = tan( fov * 0.5 ) * zn
c2' = c2 * zn = tan( fov * fovRatio * 0.5 ) * zn / aspectRatio

c3' = zn * c3 + c4
c3' = zn * ( zf - zn ) / ( zf + zn ) + 2 * zf * zn / ( zf + zn )
c3' = ( zn * ( zf - zn ) + 2 * zf * zn ) / ( zf + zn )
c3' = ( zn * zf - zn * zn + 2 * zf * zn ) / ( zf + zn )
c3' = ( 3 * zf * zn - zn * zn ) / ( zf + zn )

c3' = -zn * c3 + c4
c3' = -zn * ( zf - zn ) / ( zf + zn ) + 2 * zf * zn / ( zf + zn )
c3' = ( -zn * ( zf - zn ) + 2 * zf * zn ) / ( zf + zn )
c3' = ( -zn * zf + zn * zn + 2 * zf * zn ) / ( zf + zn )
c3' = ( zf * zn + zn * zn ) / ( zf + zn )
c3' = ( zf + zn ) * zn / ( zf + zn )
c3' = zn

// which gives the full back transformation through the projection matrix
a.x = sx * c1'
a.y = sy * c2'
a.z = c3'

// and therefore the back transformation to the texture normal
n = ( matRotCam^-1 * matLayer^-1 ) * a

*/



// Definitions
////////////////

enum eSPSphere{
	spsphMatrixCamera,
	spsphMatrixLayer,
	spsphLayerPosition,
	spsphLayerColor,
	spsphParams,
	spsphMaterialGamma,
	spsphSkyBgColor,
	spsphPositionZ
};

enum eSPBox{
	spboxMatrixCamera,
	spboxMatrixLayer,
	spboxLayerPosition,
	spboxLayerColor,
	spboxParams,
	spboxMaterialGamma,
	spboxSkyBgColor,
	spboxPositionZ
};

enum eSPBody{
	spbodyMatrixMVP,
	spbodyScalePosition,
	spbodyColor,
	spbodyMaterialGamma
};



// Class deoglRenderSky
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderSky::deoglRenderSky( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	pShaderSkySphere = NULL;
	pShaderSkyBox = NULL;
	pShaderBody = NULL;
	
	try{
		sources = shaderManager.GetSourcesNamed( "Sky Sky-Sphere" );
		pShaderSkySphere = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "Sky Sky-Box" );
		pShaderSkyBox = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "Sky Body" );
		pShaderBody = shaderManager.GetProgramWith( sources, defines );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderSky::~deoglRenderSky(){
	pCleanUp();
}



// Rendering
//////////////

//#define DO_TIMING

#ifdef DO_TIMING
#include "dragengine/common/utils/decTimer.h"
static decTimer timerTotal;
static decTimer timer;

#define DEBUG_RESET_TIMERS				timer.Reset(); timerTotal.Reset()
#define DEBUG_PRINT_TIMER(what)			glFinish(); renderThread.GetLogger().LogInfoFormat( "RenderSky Timer: %s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0f ) )
#define DEBUG_PRINT_TIMER_TOTAL(what)	glFinish(); renderThread.GetLogger().LogInfoFormat( "RenderSky Timer-Total %s = %iys", what, ( int )( timerTotal.GetElapsedTime() * 1000000.0f ) )
#else
#define DEBUG_RESET_TIMERS
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif

void deoglRenderSky::RenderSky( deoglRenderPlan &plan ){
	// NOTE always switch FBO and clear the color attachment. if no sky is present use
	//      black as clear color
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	DEBUG_RESET_TIMERS;
	defren.ActivateFBOColor( true );
		// change the skin shader so the color texture is the first one.
		// would not require this switch here anymore
	
	// clear color buffer
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const int skyCount = plan.GetSkyInstanceCount();
	if( skyCount > 0 ){
		const decColor bgColor( LinearBgColor( *plan.GetSkyInstanceAt( 0 ), true ) );
		clearColor[ 0 ] = ( GLfloat )bgColor.r;
		clearColor[ 1 ] = ( GLfloat )bgColor.g;
		clearColor[ 2 ] = ( GLfloat )bgColor.b;
		clearColor[ 3 ] = ( GLfloat )bgColor.a;
	}
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	if( plan.GetClearColor() ){
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		plan.SetClearColor( false );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	// render sky if present
	if( skyCount == 0 ){
		return;
	}
	
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
		// alpha=false required to avoid blended alpha to be written
	
	// render sky
	bool first = true;
	int i, j;
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 2 && plan.GetWorld()->GetSkyEnvironmentMap() ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap(
			*plan.GetWorld()->GetSkyEnvironmentMap()->GetEnvironmentMap(), "environment_cubemap", false );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	DEBUG_PRINT_TIMER( "RenderSky: bind vao" );
	
	for( i=0; i<skyCount; i++ ){
		deoglRSkyInstance &instance = *plan.GetSkyInstanceAt( i );
		deoglRSky &sky = *instance.GetRSky();
		const int layerCount = sky.GetLayerCount();
		
		for( j=0; j<layerCount; j++ ){
			const deoglRSkyInstanceLayer &instanceLayer = instance.GetLayerAt( j );
			if( ! instanceLayer.GetVisible() ){
				continue;
			}
			
			const deoglRSkyLayer &layer = sky.GetLayerAt( j );
			
			switch( layer.GetLayerType() ){
			case deoglRSkyLayer::eltSkyBox:
				if( RenderSkyBox( plan, instance, j, first ) ){
					first = false;
				}
				break;
				
			case deoglRSkyLayer::eltSkySphere:
				if( RenderSkySphere( plan, instance, j, first ) ){
					first = false;
				}
				break;
				
			default:
				break;
			}
			
			if( layer.GetBodyCount() > 0 ){
				RenderSkyLayerBodies( plan, instance, j );
			}
		}
		DEBUG_PRINT_TIMER( "RenderSky: render sky layer" );
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	DEBUG_PRINT_TIMER( "RenderSky: unbind vao" );
}

#if 0
void deoglRenderSky::RenderSkyOld( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRSkyInstance *instance = plan.GetWorld()->GetSky();
	if( ! instance ){
		return;
	}
	deoglRSky *sky = instance->GetRSky();
	if( ! sky ){
		return;
	}
	
DEBUG_RESET_TIMERS;
	// attach color buffer, depth buffer and stencil buffer
	defren.ActivateFBOColor( true );
DEBUG_PRINT_TIMER( "RenderSky: attach color/depth/stencil" );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set stencil parameters
	if( plan.GetLevel() == 0 ){
		// if we have no mask clear the stencil buffer with 0
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0x00, 0x00 ) );
		OGL_CHECK( renderThread, glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE ) );
		OGL_CHECK( renderThread, glStencilMask( 0xff ) );
		OGL_CHECK( renderThread, glClearStencil( 0x00 ) );
		
	}else{
		// if we have a mask leave the stencil mask untouched but test against the mask bit
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 0x01, 0x01 ) );
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilMask( 0x00 ) );
	}
DEBUG_PRINT_TIMER( "RenderSky: set states" );
	
	// if there is no sky visible clear the stencil buffer
	if( ! plan.GetSkyVisible() ){
		// no need to clear if we have a mask as the content inside the mask stays untouched
		if( plan.GetLevel() == 0 ){
			OGL_CHECK( renderThread, glClear( /*GL_DEPTH_BUFFER_BIT | */ GL_STENCIL_BUFFER_BIT ) );
DEBUG_PRINT_TIMER( "RenderSky: clear stencil (sky invisible)" );
		}
		
		return;
	}
	
	// if there is no sky clear the stencil buffer and clear the color buffer with black
	if( ! sky ){
		// no need to clear if we have a mask as the content inside the mask stays untouched
		// and a single color looks the same no matter where you look at
		if( plan.GetLevel() == 0 ){
			OGL_CHECK( renderThread, glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ) );
			OGL_CHECK( renderThread, glClear( /*GL_DEPTH_BUFFER_BIT | */ GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
DEBUG_PRINT_TIMER( "RenderSky: clear color/stencil (sky visible but not set)" );
		}
		
		return;
	}
	
	// otherwise render using the sky object
	const int layerCount = sky->GetLayerCount();
	
	if( plan.GetLevel() == 0 && renderThread.GetConfiguration().GetDebugSnapshot() == 2 ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap(
			*instance->GetEnvironmentMap()->GetEnvironmentMap(),
			"environment_cubemap", false );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	// if there are no layers use the background color
	if( layerCount == 0 ){
		// no need to clear if we have a mask as the content inside the mask stays untouched
		// and a single color sky looks the same no matter where you look at
		if( plan.GetLevel() == 0 ){
			decColor engSkyColor = sky->GetBgColor();
			
			engSkyColor.r = powf( engSkyColor.r, OGL_RENDER_GAMMA );
			engSkyColor.g = powf( engSkyColor.g, OGL_RENDER_GAMMA );
			engSkyColor.b = powf( engSkyColor.b, OGL_RENDER_GAMMA );
			
			OGL_CHECK( renderThread, glClearColor( engSkyColor.r, engSkyColor.g, engSkyColor.b, engSkyColor.a ) );
			OGL_CHECK( renderThread, glClear( /*GL_DEPTH_BUFFER_BIT | */ GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
DEBUG_PRINT_TIMER( "RenderSky: clear color/stencil with background color" );
		}
		
	// otherwise render all layers
	}else{
		bool first = true;
		int l, layerType;
		
		// render all layers
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
DEBUG_PRINT_TIMER( "RenderSky: bind vao" );
		
		for( l=0; l<layerCount; l++ ){
			deoglRSkyLayer &layer = sky->GetLayerAt( l );
			
			if( layer.GetVisible() ){
				layerType = layer.GetLayerType();
				
				if( layerType == deoglRSkyLayer::eltSkyBox ){
					RenderSkyBox( plan, *sky, layer, false );
					
				}else if( layerType == deoglRSkyLayer::eltSkySphere ){
					RenderSkySphere( plan, *sky, layer, false );
					
				}else{
					// this one is a bit tricky. if the layer type is not set to a usable one there
					// is usually not a skin attached or the skin does not define a valid sky layer.
					// there are now two cases. in the first case the layer has no bodies and the
					// layer can be considered invisible. otherwise if there are bodies we have to
					// first clear the background
					if( first ){
						if( layer.GetBodyCount() == 0 ){
							continue;
						}
						
						decColor engSkyColor = sky->GetBgColor();
						engSkyColor.r = powf( engSkyColor.r, OGL_RENDER_GAMMA );
						engSkyColor.g = powf( engSkyColor.g, OGL_RENDER_GAMMA );
						engSkyColor.b = powf( engSkyColor.b, OGL_RENDER_GAMMA );
						
						OGL_CHECK( renderThread, glClearColor( engSkyColor.r, engSkyColor.g, engSkyColor.b, engSkyColor.a ) );
						OGL_CHECK( renderThread, glClear( /*GL_DEPTH_BUFFER_BIT | */ GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
					}
				}
				
				if( first ){
					// for everything beyond the first layer use blending and disable
					// writing to the stencil buffer. the first layer is supposed to
					// cover the entire screen so after this is done no stencil operation
					// is required anymore and transparency can occur
					OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
					OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
					OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
					OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
					
					first = false;
				}
				
				if( layer.GetBodyCount() > 0 ){
					RenderSkyLayerBodies( plan, *sky, layer );
				}
DEBUG_PRINT_TIMER( "RenderSky: render sky layer" );
			}
		}
		
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
DEBUG_PRINT_TIMER( "RenderSky: unbind vao" );
		
		// if there is no visible layer clear with the background color
		if( first ){
			decColor engSkyColor = sky->GetBgColor();
			
			engSkyColor.r = powf( engSkyColor.r, OGL_RENDER_GAMMA );
			engSkyColor.g = powf( engSkyColor.g, OGL_RENDER_GAMMA );
			engSkyColor.b = powf( engSkyColor.b, OGL_RENDER_GAMMA );
			
			OGL_CHECK( renderThread, glClearColor( engSkyColor.r, engSkyColor.g, engSkyColor.b, engSkyColor.a ) );
			OGL_CHECK( renderThread, glClear( /*GL_DEPTH_BUFFER_BIT | */ GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
		}
	}
}
#endif

bool deoglRenderSky::RenderSkyBox( deoglRenderPlan &plan, deoglRSkyInstance &instance,
int layerIndex, bool first ){
	return false;
}

bool deoglRenderSky::RenderSkySphere( deoglRenderPlan &plan, deoglRSkyInstance &instance,
int layerIndex, bool first ){
	if( ! instance.GetRSky() ){
		return false;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	const deoglRSkyInstanceLayer &instanceLayer = instance.GetLayerAt( layerIndex );
	const deoglRSkyLayer &layer = instance.GetRSky()->GetLayerAt( layerIndex );
	
	const int * const textures = layer.GetTextures();
	decColor layerColor( instanceLayer.GetColor() );
	float layerIntensity = instanceLayer.GetIntensity();
	deoglSkinTexture *oglSkinTexture;
	deoglRSkin *skin = layer.GetSkin();
	deoglShaderCompiled *shader;
	float constX, constY;
	
	if( ! skin ){
		return false;
	}
	if( skin->GetTextureCount() == 0 ){
		return false;
	}
	oglSkinTexture = &skin->GetTextureAt( 0 );
	
	// adjust gamma
	layerColor.r = powf( layerColor.r, OGL_RENDER_GAMMA );
	layerColor.g = powf( layerColor.g, OGL_RENDER_GAMMA );
	layerColor.b = powf( layerColor.b, OGL_RENDER_GAMMA );
	layerColor.a *= instanceLayer.GetTransparency();
	
	// if we render full bright ignore the intensity
	if( ! plan.GetDisableLights() ){
		layerColor.r *= layerIntensity;
		layerColor.g *= layerIntensity;
		layerColor.b *= layerIntensity;
	}
	
	// calculate the parameters
	float znear = plan.GetCameraImageDistance();
	constX = tanf( plan.GetCameraFov() * 0.5f ) * znear;
	constY = tanf( plan.GetCameraFov() * plan.GetCameraFovRatio() * 0.5f )
		* znear / plan.GetAspectRatio();
	
	// set the shaders
	float matGamma = oglSkinTexture->GetColorGamma();
	
	renderThread.GetShader().ActivateShader( pShaderSkySphere );
	shader = pShaderSkySphere->GetCompiled();
	
//	shader->SetParameterDMatrix3x3( spsphMatrixLayer,
//		decDMatrix::CreateFromQuaternion( layer.GetOrientation() )
//		* plan.GetInverseCameraMatrix().GetRotationMatrix() );
//	shader->SetParameterDMatrix3x3( spsphMatrixLayer,
//		plan.GetInverseCameraMatrix().GetRotationMatrix()
//		* decDMatrix::CreateFromQuaternion( layer.GetOrientation() ) );
	// normalMatrix = transpose( invert( invCamRot * invLayerMat ) )
	// normalMatrix = transpose( layerMat * camRot )
	shader->SetParameterDMatrix3x3( spsphMatrixCamera, plan.GetInverseCameraMatrix() );
	shader->SetParameterDMatrix3x3( spsphMatrixLayer, instanceLayer.GetMatrix() );
	shader->SetParameterVector3( spsphLayerPosition, -instanceLayer.GetMatrix().GetPosition() );
	shader->SetParameterColor4( spsphLayerColor, layerColor );
	shader->SetParameterFloat( spsphParams, constX, constY, znear );
	shader->SetParameterFloat( spsphMaterialGamma, matGamma, matGamma, matGamma, 1.0 );
	shader->SetParameterColor4( spsphSkyBgColor, LinearBgColor( instance, first ) );
	shader->SetParameterFloat( spsphPositionZ, renderThread.GetDeferredRendering().GetClearDepthValueRegular() );
	
	// set texture
	if( textures[ deoglRSkyLayer::eiSphere ] == -1 ){
		tsmgr.EnableTexture( 0, *renderThread.GetDefaultTextures().GetColor(),
			GetSamplerRepeatLinear() );
		
	}else{
		tsmgr.EnableSkin( 0, *skin, textures[ deoglRSkyLayer::eiSphere ],
			deoglSkinChannel::ectColor, renderThread.GetDefaultTextures().GetColor(),
			*renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinear ) );
	}
	
	// render layer
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	return true;
}

void deoglRenderSky::RenderSkyLayerBodies( deoglRenderPlan &plan,
deoglRSkyInstance &instance, int layerIndex ){
	if( ! instance.GetRSky() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	const deoglRSkyInstanceLayer &instanceLayer = instance.GetLayerAt( layerIndex );
	const deoglRSkyLayer &layer = instance.GetRSky()->GetLayerAt( layerIndex );
	
	const deoglRSkyLayer::sBody * const bodies = layer.GetBodies();
	int b, bodyCount = layer.GetBodyCount();
	decColor layerColor = instanceLayer.GetColor();
	float layerIntensity = instanceLayer.GetIntensity();
	deoglSkinTexture *oglSkinTexture;
	decDMatrix matrixLCP, matrixBody;
	deoglShaderCompiled *shader;
	decColor bodyColor;
	
	// adjust gamma
	layerColor.r = powf( layerColor.r, OGL_RENDER_GAMMA );
	layerColor.g = powf( layerColor.g, OGL_RENDER_GAMMA );
	layerColor.b = powf( layerColor.b, OGL_RENDER_GAMMA );
	layerColor.a *= instanceLayer.GetTransparency();
	
	// if we render full bright ignore the intensity
	if( ! plan.GetDisableLights() ){
		layerColor.r *= layerIntensity;
		layerColor.g *= layerIntensity;
		layerColor.b *= layerIntensity;
	}
	
	// set the shaders
	renderThread.GetShader().ActivateShader( pShaderBody );
	shader = pShaderBody->GetCompiled();
	
	// set matrix
	//matrixLCP = decDMatrix::CreateFromQuaternion( engLayer.GetOrientation() )
	matrixLCP = instanceLayer.GetMatrix()
		* plan.GetCameraMatrix().GetRotationMatrix()
		* decDMatrix( plan.GetProjectionMatrix() );
	
	// render bodies
	deoglTexture * const defTexColor = renderThread.GetDefaultTextures().GetColor();
	deoglTexture * const defTexTransparency = renderThread.GetDefaultTextures().GetTransparency();
	deoglTexSamplerConfig &tscClampLinear = *renderThread.GetShader()
		.GetTexSamplerConfig( deoglRTShader::etscClampLinear );
	
	for( b=0; b<bodyCount; b++ ){
		if( ! bodies[ b ].skin ){
			continue;
		}
		
		deoglRSkin &skin = *bodies[ b ].skin;
		
		if( skin.GetTextureCount() == 0 ) continue;
		oglSkinTexture = &skin.GetTextureAt( 0 );
		const decVector2 &size = bodies[ b ].size;
		float matGamma = oglSkinTexture->GetColorGamma();
		
		bodyColor.r = powf( bodies[ b ].color.r, OGL_RENDER_GAMMA ) * layerColor.r;
		bodyColor.g = powf( bodies[ b ].color.g, OGL_RENDER_GAMMA ) * layerColor.g;
		bodyColor.b = powf( bodies[ b ].color.b, OGL_RENDER_GAMMA ) * layerColor.b;
		bodyColor.a = bodies[ b ].color.a * layerColor.a;
		
		matrixBody.SetFromQuaternion( bodies[ b ].orientation );
		
		tsmgr.EnableSkin( 0, skin, 0, deoglSkinChannel::ectColor, defTexColor, tscClampLinear );
		tsmgr.EnableSkin( 1, skin, 0, deoglSkinChannel::ectTransparency,
			defTexTransparency, tscClampLinear );
		
		shader->SetParameterDMatrix4x4( spbodyMatrixMVP, matrixBody * matrixLCP );
		shader->SetParameterFloat( spbodyScalePosition, size.x, size.y );
		shader->SetParameterColor4( spbodyColor, bodyColor );
		shader->SetParameterFloat( spbodyMaterialGamma, matGamma, matGamma, matGamma, 1.0 );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
}

void deoglRenderSky::RenderSkyIntoEnvMap( deoglRWorld &world, const decLayerMask &layerMask,
deoglEnvironmentMap &envmap ){
	const bool useLayerMask = layerMask.IsNotEmpty();
	int i;
	
	pSkyInstances.RemoveAll();
	
	for( i=0; i<world.GetSkyCount(); i++ ){
		deoglRSkyInstance * const instance = world.GetSkyAt( i );
		if( ! instance->GetRSky() ){
			continue;
		}
		if( useLayerMask && instance->GetLayerMask().IsNotEmpty() && layerMask.MatchesNot( instance->GetLayerMask() ) ){
			continue;
		}
		pSkyInstances.Add( instance );
	}
	
	const int skyCount = pSkyInstances.GetCount();
	if( skyCount == 0 ){
		RenderEmptySkyIntoEnvMap( world, envmap );
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglFramebuffer *oldfbo = renderThread.GetFramebuffer().GetActive();
	deoglCubeMap *cubemap = envmap.GetEnvironmentMap();
	deoglRenderPlan plan( renderThread );
	deoglFramebuffer *fbo = NULL;
	int size = envmap.GetSize();
	decDMatrix matrixCamera;
	int cmf, l, s;
	
	const int cubeFaces[] = {
		deoglCubeMap::efPositiveX, deoglCubeMap::efNegativeX,
		deoglCubeMap::efPositiveY, deoglCubeMap::efNegativeY,
		deoglCubeMap::efPositiveZ, deoglCubeMap::efNegativeZ };
	
	decColor engSkyColor( ( ( deoglRSkyInstance* )pSkyInstances.GetAt( 0 ) )->GetRSky()->GetBgColor() );
	engSkyColor.r = powf( engSkyColor.r, OGL_RENDER_GAMMA );
	engSkyColor.g = powf( engSkyColor.g, OGL_RENDER_GAMMA );
	engSkyColor.b = powf( engSkyColor.b, OGL_RENDER_GAMMA );
	
	plan.SetCameraParameters( DEG2RAD * 90.0f, 1.0f, 0.01f, 500.0f );
	plan.SetCameraMatrix( decDMatrix() );
	plan.SetViewport( 0, 0, size, size );
	
	plan.SetWorld( NULL ); // little hack since we only want the projection matrix to be updated
	plan.PrepareRender();
	plan.SetWorld( &world );
	
	//defren.Resize( size, size );
	
	try{
		fbo = renderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
		renderThread.GetFramebuffer().Activate( fbo );
		
		fbo->DetachAllImages();
		
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
		
		const GLfloat clearColor[ 4 ] = { engSkyColor.r, engSkyColor.g, engSkyColor.b, 1.0f };
		
		for( cmf=0; cmf<6; cmf++ ){
			fbo->AttachColorCubeMapFace( 0, cubemap, cubeFaces[ cmf ] );
			fbo->Verify();
			
			OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			
			OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE ) );
				// alpha=false required to avoid blended alpha to be written
			
			if( cmf == 0 ){ // positive x
				matrixCamera.a11 = 0.0; matrixCamera.a12 =  0.0; matrixCamera.a13 = -1.0;
				matrixCamera.a21 = 0.0; matrixCamera.a22 = -1.0; matrixCamera.a23 =  0.0;
				matrixCamera.a31 = 1.0; matrixCamera.a32 =  0.0; matrixCamera.a33 =  0.0;
				
			}else if( cmf == 1 ){ // negative x
				matrixCamera.a11 =  0.0; matrixCamera.a12 =  0.0; matrixCamera.a13 = 1.0;
				matrixCamera.a21 =  0.0; matrixCamera.a22 = -1.0; matrixCamera.a23 = 0.0;
				matrixCamera.a31 = -1.0; matrixCamera.a32 =  0.0; matrixCamera.a33 = 0.0;
				
			}else if( cmf == 2 ){ // positive y
				matrixCamera.a11 = 1.0; matrixCamera.a12 = 0.0; matrixCamera.a13 = 0.0;
				matrixCamera.a21 = 0.0; matrixCamera.a22 = 0.0; matrixCamera.a23 = 1.0;
				matrixCamera.a31 = 0.0; matrixCamera.a32 = 1.0; matrixCamera.a33 = 0.0;
				
			}else if( cmf == 3 ){ // negative y
				matrixCamera.a11 = 1.0; matrixCamera.a12 =  0.0; matrixCamera.a13 =  0.0;
				matrixCamera.a21 = 0.0; matrixCamera.a22 =  0.0; matrixCamera.a23 = -1.0;
				matrixCamera.a31 = 0.0; matrixCamera.a32 = -1.0; matrixCamera.a33 =  0.0;
				
			}else if( cmf == 4 ){ // positive z
				matrixCamera.a11 = 1.0; matrixCamera.a12 =  0.0; matrixCamera.a13 = 0.0;
				matrixCamera.a21 = 0.0; matrixCamera.a22 = -1.0; matrixCamera.a23 = 0.0;
				matrixCamera.a31 = 0.0; matrixCamera.a32 =  0.0; matrixCamera.a33 = 1.0;
				
			}else if( cmf == 5 ){ // negative z
				matrixCamera.a11 = -1.0; matrixCamera.a12 =  0.0; matrixCamera.a13 =  0.0;
				matrixCamera.a21 =  0.0; matrixCamera.a22 = -1.0; matrixCamera.a23 =  0.0;
				matrixCamera.a31 =  0.0; matrixCamera.a32 =  0.0; matrixCamera.a33 = -1.0;
			}
			
			plan.SetCameraMatrix( matrixCamera );
			
			for( s=0; s<skyCount; s++ ){
				deoglRSkyInstance &instance = *( ( deoglRSkyInstance* )pSkyInstances.GetAt( s ) );
				deoglRSky &sky = *instance.GetRSky();
				const int layerCount = sky.GetLayerCount();
				
				for( l=0; l<layerCount; l++ ){
					const deoglRSkyLayer &layer = sky.GetLayerAt( l );
					if( ! instance.GetLayerAt( l ).GetVisible() ){
						continue;
					}
					
					switch( layer.GetLayerType() ){
					case deoglRSkyLayer::eltSkyBox:
						RenderSkyBox( plan, instance, l, false );
						break;
						
					case deoglRSkyLayer::eltSkySphere:
						RenderSkySphere( plan, instance, l, false );
						break;
						
					default:
						break;
					}
					
					if( layer.GetBodyCount() > 0 ){
						RenderSkyLayerBodies( plan, instance, l );
					}
				}
			}
		}
		
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		
		cubemap->CreateMipMaps();
		
	}catch( const deException & ){
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		throw;
	}
}

void deoglRenderSky::RenderEmptySkyIntoEnvMap( deoglRWorld&, deoglEnvironmentMap &envmap ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	deoglCubeMap * const cubemap = envmap.GetEnvironmentMap();
	deoglFramebuffer *fbo = NULL;
	const int size = envmap.GetSize();
	int cmf;
	
	const int cubeFaces[] = {
		deoglCubeMap::efPositiveX, deoglCubeMap::efNegativeX,
		deoglCubeMap::efPositiveY, deoglCubeMap::efNegativeY,
		deoglCubeMap::efPositiveZ, deoglCubeMap::efNegativeZ };
	
	const decColor skyColor( 0.0f, 0.0f, 0.0f );
	
	try{
		fbo = renderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
		renderThread.GetFramebuffer().Activate( fbo );
		
		fbo->DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		const GLfloat clearColor[ 4 ] = { skyColor.r, skyColor.g, skyColor.b, 1.0f };
		
		for( cmf=0; cmf<6; cmf++ ){
			fbo->AttachColorCubeMapFace( 0, cubemap, cubeFaces[ cmf ] );
			fbo->Verify();
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		}
		
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		
		cubemap->CreateMipMaps();
		
	}catch( const deException & ){
		renderThread.GetFramebuffer().Activate( oldfbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		throw;
	}
}

decColor deoglRenderSky::LinearBgColor( const deoglRSkyInstance &instance, bool first ) const{
	if( ! first ){
		return decColor( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	if( ! instance.GetRSky() ){
		return decColor( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	const decColor &skyColor = instance.GetRSky()->GetBgColor();
	
	return decColor(
		( GLfloat )powf( skyColor.r, OGL_RENDER_GAMMA ),
		( GLfloat )powf( skyColor.g, OGL_RENDER_GAMMA ),
		( GLfloat )powf( skyColor.b, OGL_RENDER_GAMMA ),
		skyColor.a );
}



// Private Functions
//////////////////////

void deoglRenderSky::pCleanUp(){
	if( pShaderSkySphere ){
		pShaderSkySphere->RemoveUsage();
	}
	if( pShaderSkyBox ){
		pShaderSkyBox->RemoveUsage();
	}
	if( pShaderBody ){
		pShaderBody->RemoveUsage();
	}
}

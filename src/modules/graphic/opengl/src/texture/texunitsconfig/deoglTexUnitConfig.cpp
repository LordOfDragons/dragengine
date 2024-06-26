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

#include "deoglTexUnitConfig.h"
#include "../deoglRImage.h"
#include "../deoglTextureStageManager.h"
#include "../arraytexture/deoglArrayTexture.h"
#include "../cubemap/deoglCubeMap.h"
#include "../texture2d/deoglTexture.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../rendering/deoglRenderReflection.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/combinedTexture/deoglCombinedTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../skin/state/deoglSkinStateConstructed.h"
#include "../../video/deoglRVideoPlayer.h"

#include <dragengine/common/exceptions.h>



// Class deoglTexUnitConfig
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexUnitConfig::deoglTexUnitConfig() :
pTexture( NULL ),
pCubeMap( NULL ),
pArrayTexture( NULL ),
pTBO( 0 ),
pSpecial( estNone ),
pSampler( NULL ){
}

deoglTexUnitConfig::~deoglTexUnitConfig(){
}



// Management
///////////////

void deoglTexUnitConfig::Apply( deoglRenderThread &renderThread, int stage ) const{
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	if( pTexture ){
		tsmgr.EnableTexture( stage, *pTexture, *pSampler );
		
	}else if( pCubeMap ){
		tsmgr.EnableCubeMap( stage, *pCubeMap, *pSampler );
		
	}else if( pArrayTexture ){
		tsmgr.EnableArrayTexture( stage, *pArrayTexture, *pSampler );
		
	}else if( pTBO ){
		tsmgr.EnableTBO( stage, pTBO, *renderThread.GetShader().GetTexSamplerConfig(
			deoglRTShader::etscClampNearest ) );
		
	}else{
		switch( pSpecial ){
		case deoglTexUnitConfig::estPrevDepth:
			tsmgr.EnableArrayTexture( stage, *renderThread.GetDeferredRendering().GetDepthTexture2(), *pSampler );
			break;
			
		case deoglTexUnitConfig::estPrevColor:
			tsmgr.EnableArrayTexture( stage, *renderThread.GetDeferredRendering().GetTextureTemporary1(), *pSampler );
			break;
			
		case estXRayDepth:
			tsmgr.EnableArrayTexture( stage, *renderThread.GetDeferredRendering().GetDepthTextureXRay(), *pSampler );
			break;
			
		case deoglTexUnitConfig::estDirectEnvMapActive:{
			const deoglRenderReflection &renderReflection = renderThread.GetRenderers().GetReflection();
			const bool useEqui = renderReflection.GetUseEquiEnvMap();
			deoglEnvironmentMap * const envmap = renderReflection.GetDirectEnvMapActive();
			
			if( envmap ){
				if( useEqui ){
					tsmgr.EnableTexture( stage, *envmap->GetEquiEnvMap(), *pSampler );
					
				}else{
					// DEBUGGING
					if( ! envmap->GetEnvironmentMap() ){
						printf( "BUG! envmap->GetEnvironmentMap() is NULL\n" );
						printf( "  world: %p\n", envmap->GetWorld() );
						printf( "  position: %g %g %g\n", envmap->GetPosition().x, envmap->GetPosition().y, envmap->GetPosition().z );
						printf( "  skyonly: %d\n", envmap->GetSkyOnly() );
						printf( "  dirty: %d\n", envmap->GetDirty() );
						printf( "  ready: %d\n", envmap->GetReady() );
						printf( "  plan usage count: %d\n", envmap->GetPlanUsageCount() );
						printf( "  destroy if unused: %d\n", envmap->GetDestroyIfUnused() );
						printf( "  component list count: %d\n", envmap->GetComponentList().GetCount() );
						printf( "  particle emitter instance list count: %d\n", envmap->GetParticleEmitterInstanceList().GetCount() );
						printf( "  render plan list count: %d\n", envmap->GetRenderPlanList().GetCount() );
						printf( "  marked: %d\n", envmap->GetMarked() );
						printf( "  slot index: %d\n", envmap->GetSlotIndex() );
						printf( "  refcount: %d\n", envmap->GetRefCount() );
						tsmgr.EnableCubeMap( stage, *renderThread.GetDefaultTextures().GetEnvMap(), *pSampler );
						return;
					}
					// DEBUGGING
					tsmgr.EnableCubeMap( stage, *envmap->GetEnvironmentMap(), *pSampler );
				}
				
			}else{
				if( useEqui ){
					tsmgr.EnableTexture( stage, *renderThread.GetDefaultTextures().GetColor(),
						*pSampler );
					
				}else{
					tsmgr.EnableCubeMap( stage, *renderThread.GetDefaultTextures().GetEnvMap(),
						*pSampler );
				}
			}
			}break;
			
		case deoglTexUnitConfig::estDirectEnvMapFading:{
			const deoglRenderReflection &renderReflection = renderThread.GetRenderers().GetReflection();
			const bool useEqui = renderReflection.GetUseEquiEnvMap();
			deoglEnvironmentMap *envmap = renderReflection.GetDirectEnvMapFading();
			
			if( ! envmap ){
				envmap = renderReflection.GetDirectEnvMapActive();
			}
			
			if( envmap ){
				if( useEqui ){
					tsmgr.EnableTexture( stage, *envmap->GetEquiEnvMap(), *pSampler );
					
				}else{
					tsmgr.EnableCubeMap( stage, *envmap->GetEnvironmentMap(), *pSampler );
				}
				
			}else{
				if( useEqui ){
					tsmgr.EnableTexture( stage, *renderThread.GetDefaultTextures().GetColor(),
						*pSampler );
					
				}else{
					tsmgr.EnableCubeMap( stage, *renderThread.GetDefaultTextures().GetEnvMap(),
						*pSampler );
				}
			}
			}break;
			
		default:
			break;
		}
	}
}



void deoglTexUnitConfig::SetSampler( deoglTexSamplerConfig *sampler ){
	pSampler = sampler;
}



bool deoglTexUnitConfig::IsEnabled() const{
	return pTexture || pCubeMap || pArrayTexture || pTBO || pSpecial != estNone;
}

bool deoglTexUnitConfig::IsDisabled() const{
	return ! pTexture && ! pCubeMap && ! pArrayTexture && ! pTBO && pSpecial == estNone;
}



void deoglTexUnitConfig::Disable(){
	pTexture = NULL;
	pCubeMap = NULL;
	pArrayTexture = NULL;
	pTBO = 0;
	pSpecial = estNone;
	pSampler = NULL;
}

void deoglTexUnitConfig::EnableTexture( deoglTexture *texture, deoglTexSamplerConfig *sampler ){
	pTexture = texture;
	pCubeMap = NULL;
	pArrayTexture = NULL;
	pTBO = 0;
	pSpecial = estNone;
	pSampler = sampler;
}

void deoglTexUnitConfig::EnableTextureFromChannel( deoglRenderThread &renderThread,
const deoglSkinTexture &skinTexture, deoglSkinChannel::eChannelTypes skinChannel,
const deoglSkinState *skinState, const deoglRDynamicSkin *dynamicSkin, deoglTexture *defaultTexture ){
	deoglTexture *useTexture = defaultTexture;
	deoglTexSamplerConfig *useSampler = nullptr;
	bool found = false;
	
	// determine texture to use
	const deoglSkinChannel * const schan = skinTexture.GetChannelAt( skinChannel );
	
	if( schan ){
		// try renderable
		const int skinRenderable = schan->GetRenderable();
		deoglTexture *renderableTexture = nullptr;
		
		if( dynamicSkin && skinState ){
			if( skinRenderable != -1 && skinRenderable < skinState->GetRenderableCount() ){
				const deoglSkinStateRenderable &skinStateRenderable = *skinState->GetRenderableAt( skinRenderable );
				const int hostIndex = skinStateRenderable.GetHostRenderable();
				
				if( hostIndex != -1 && dynamicSkin ){
					renderableTexture = dynamicSkin->GetRenderableAt( hostIndex )->GetRenderTexture();
				}
			}
		}
		
		if( renderableTexture ){
			useTexture = renderableTexture;
			found = true;
		}
		
		// try video player
		if( ! found ){
			const int skinVideoPlayer = schan->GetVideoPlayer();
			deoglRVideoPlayer *videoPlayer = nullptr;
			
			if( skinVideoPlayer != -1 && skinState && skinVideoPlayer < skinState->GetVideoPlayerCount() ){
				videoPlayer = skinState->GetVideoPlayerAt( skinVideoPlayer );
			}
			
			if( videoPlayer ){
				useTexture = videoPlayer->GetTexture();
				found = true;
			}
		}
		
		// try dynamic constructed
		if( ! found ){
			const int dynamicConstructed = schan->GetDynamicConstructed();
			
			if( dynamicConstructed != -1 && skinState
			&& dynamicConstructed < skinState->GetConstructedPropertyCount() ){
				const deoglRenderTarget::Ref &renderTarget =
					skinState->GetConstructedPropertyAt( dynamicConstructed ).GetRenderTarget();
				if( renderTarget ){
					useTexture = renderTarget->GetTexture();
				}
				found = true;
			}
		}
		
		// otherwise use channel texture
		if( ! found ){
			deoglTexture *texture = schan->GetTexture();
			
			if( schan->GetImage() ){
				texture = schan->GetImage()->GetTexture();
				
			}else if( schan->GetCombinedTexture() ){
				texture = schan->GetCombinedTexture()->GetTexture();
			}
			
			if( texture ){
				useTexture = texture;
			}
		}
	}
	
	// determine if texture coordinates have to be clamped
	bool clampTexCoord = skinTexture.GetTexCoordClamp();
	
	if( skinChannel == deoglSkinChannel::ectColorOmnidirEquirect ){
		// this is important here. if not clamped opengl adds thin artifact lines across the
		// 0-degrees angle from top to bottom as well as causing poles to be distorted and
		// flipped upside down (top pole is down). no idea what driver problem causes this
		// but forcing clamping solves the problem
		clampTexCoord = true;
	}
	
	// determine sampler to use
	if( useTexture && useTexture->GetMipMapped() ){
		if( clampTexCoord ){
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap );
			
		}else{
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap );
		}
		
	}else{
		if( clampTexCoord ){
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear );
			
		}else{
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinear );
		}
	}
	
	// enable texture using the found parameters if existing otherwise disable unit
	if( useTexture ){
		EnableTexture( useTexture, useSampler );
		
	}else{
		Disable();
	}
}

void deoglTexUnitConfig::EnableCubeMap( deoglCubeMap *cubemap, deoglTexSamplerConfig *sampler ){
	pTexture = NULL;
	pCubeMap = cubemap;
	pArrayTexture = NULL;
	pTBO = 0;
	pSpecial = estNone;
	pSampler = sampler;
}

void deoglTexUnitConfig::EnableCubeMapFromChannel( deoglRenderThread &renderThread,
const deoglSkinTexture &skinTexture, deoglSkinChannel::eChannelTypes skinChannel,
const deoglSkinState *skinState, const deoglRDynamicSkin *dynamicSkin, deoglCubeMap *defaultCubemap ){
	deoglCubeMap *useCubemap = defaultCubemap;
	deoglTexSamplerConfig *useSampler = NULL;
	bool found = false;
	
	// determine texture to use
	const deoglSkinChannel * const schan = skinTexture.GetChannelAt( skinChannel );
	
	if( schan ){
		// try renderable
		const int skinRenderable = schan->GetRenderable();
		deoglCubeMap *renderableCubemap = NULL;
		
		if( dynamicSkin && skinState ){
			if( skinRenderable != -1 && skinRenderable < skinState->GetRenderableCount() ){
				const deoglSkinStateRenderable &skinStateRenderable = *skinState->GetRenderableAt( skinRenderable );
				const int hostIndex = skinStateRenderable.GetHostRenderable();
				
				if( hostIndex != -1 && dynamicSkin ){
					renderableCubemap = dynamicSkin->GetRenderableAt( hostIndex )->GetRenderCubeMap();
				}
			}
		}
		
		if( renderableCubemap ){
			useCubemap = renderableCubemap;
			found = true;
		}
		
		// otherwise us channel texture
		if( ! found ){
			deoglCubeMap *cubemap = schan->GetCubeMap();
			
			if( schan->GetImage() ){
				cubemap = schan->GetImage()->GetCubeMap();
			}
			
			if( cubemap ){
				useCubemap = cubemap;
			}
		}
	}
	
	// determine filtering to use
	if( useCubemap && useCubemap->GetMipMapped() ){
		useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap );
		
	}else{
		useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear );
	}
	
	// enable cube map using the found parameters if existing otherwise disable unit
	if( useCubemap ){
		EnableCubeMap( useCubemap, useSampler );
		
	}else{
		Disable();
	}
}

void deoglTexUnitConfig::EnableArrayTexture( deoglArrayTexture *texture, deoglTexSamplerConfig *sampler ){
	pTexture = NULL;
	pCubeMap = NULL;
	pArrayTexture = texture;
	pTBO = 0;
	pSpecial = estNone;
	pSampler = sampler;
}

void deoglTexUnitConfig::EnableArrayTextureFromChannel( deoglRenderThread &renderThread,
const deoglSkinTexture &skinTexture, deoglSkinChannel::eChannelTypes skinChannel,
const deoglSkinState *skinState, const deoglRDynamicSkin *dynamicSkin, deoglArrayTexture *defaultTexture ){
	deoglArrayTexture *useTexture = defaultTexture;
	deoglTexSamplerConfig *useSampler = NULL;
	
	// determine texture to use
	const deoglSkinChannel * const schan = skinTexture.GetChannelAt( skinChannel );
	
	if( schan ){
		deoglArrayTexture *texture = schan->GetArrayTexture();
		
		if( schan->GetImage() ){
			texture = schan->GetImage()->GetArrayTexture();
			
		}else if( schan->GetCombinedTexture() ){
			//texture = schan->GetCombinedTexture()->GetTexture();
			// not supported for the time being
		}
		
		if( texture ){
			useTexture = texture;
		}
	}
	
	// determine sampler to use
	if( useTexture && useTexture->GetMipMapped() ){
		if( skinTexture.GetTexCoordClamp() ){
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap );
			
		}else{
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap );
		}
		
	}else{
		if( skinTexture.GetTexCoordClamp() ){
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear );
			
		}else{
			useSampler = renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinear );
		}
	}
	
	// enable texture using the found parameters if existing otherwise disable unit
	if( useTexture ){
		EnableArrayTexture( useTexture, useSampler );
		
	}else{
		Disable();
	}
}

void deoglTexUnitConfig::EnableTBO( GLuint tbo ){
	pTexture = NULL;
	pCubeMap = NULL;
	pArrayTexture = NULL;
	pTBO = tbo;
	pSpecial = estNone;
	pSampler = NULL;
}

void deoglTexUnitConfig::EnableSpecial( int special, deoglTexSamplerConfig *sampler ){
	if( special < estNone || special >= EST_BASE_GLNAME ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	pCubeMap = NULL;
	pArrayTexture = NULL;
	pTBO = 0;
	pSpecial = special;
	pSampler = sampler;
}



bool deoglTexUnitConfig::Equals( const deoglTexUnitConfig &tuc ) const{
	return pTexture == tuc.pTexture
		&& pCubeMap == tuc.pCubeMap
		&& pArrayTexture == tuc.pArrayTexture
		&& pTBO == tuc.pTBO
		&& pSpecial == tuc.pSpecial
		&& pSampler == tuc.pSampler;
}

void deoglTexUnitConfig::SetFrom( const deoglTexUnitConfig &tuc ){
	pTexture = tuc.pTexture;
	pCubeMap = tuc.pCubeMap;
	pArrayTexture = tuc.pArrayTexture;
	pTBO = tuc.pTBO;
	pSpecial = tuc.pSpecial;
	pSampler = tuc.pSampler;
}

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

void deoglTexUnitConfig::Apply( deoglRenderThread &renderThread, int stage ){
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
		
	}else if( pSpecial == deoglTexUnitConfig::estPrevDepth ){
		tsmgr.EnableTexture( stage, *renderThread.GetDeferredRendering().GetDepthTexture2(),
			*pSampler );
		
	}else if( pSpecial == deoglTexUnitConfig::estPrevColor ){
		tsmgr.EnableTexture( stage, *renderThread.GetDeferredRendering().GetTextureTemporary1(),
			*pSampler );
		
	}else if( pSpecial == deoglTexUnitConfig::estDirectEnvMapActive ){
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
		
	}else if( pSpecial == deoglTexUnitConfig::estDirectEnvMapFading ){
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
	deoglTexSamplerConfig *useSampler = NULL;
	
	// determine texture to use
	const deoglSkinChannel * const schan = skinTexture.GetChannelAt( skinChannel );
	
	if( schan ){
		const int skinRenderable = schan->GetRenderable();
		deoglTexture *renderableTexture = NULL;
		
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
			
		}else{
			const int skinVideoPlayer = schan->GetVideoPlayer();
			deoglRVideoPlayer *videoPlayer = NULL;
			
			if( skinVideoPlayer != -1 && skinState && skinVideoPlayer < skinState->GetVideoPlayerCount() ){
				videoPlayer = skinState->GetVideoPlayerAt( skinVideoPlayer );
			}
			
			if( videoPlayer ){
				useTexture = videoPlayer->GetTexture();
				
			}else{
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
	
	// determine texture to use
	const deoglSkinChannel * const schan = skinTexture.GetChannelAt( skinChannel );
	
	if( schan ){
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
			
		}else{
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

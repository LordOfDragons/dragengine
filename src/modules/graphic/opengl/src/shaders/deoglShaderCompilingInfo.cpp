/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglShaderCompilingInfo.h"
#include "deoglShaderLanguage.h"
#include "deoglShaderManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoglShaderCompilingInfo
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderCompilingInfo::deoglShaderCompilingInfo( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pState( esInvisible ),
pDelayFadeIn( 0.25f ),
pTimeFadeIn( 0.25f ),
pDelayFadeOut( 1.0f ),
pTimeFadeOut( 0.5f ),
pElapsed( 0.0f ),
pTransparency( 0.0f ),
pHasLoadingShader( false ),
pHasCompilingShader( false ){
}

deoglShaderCompilingInfo::~deoglShaderCompilingInfo(){
}



// Management
///////////////

void deoglShaderCompilingInfo::Update( float elapsed ){
	pUpdateChecks();
	pUpdateState( elapsed );
	
	// pRenderThread.GetLogger().LogInfoFormat("ShaderCompilingInfo: hls=%d hcs=%d s=%d e=%f",
		// pHasLoadingShader, pHasCompilingShader, pState, pElapsed);
}



// Private Functions
//////////////////////

void deoglShaderCompilingInfo::pUpdateChecks(){
	deoglShaderLanguage &shaderLanguage = *pRenderThread.GetShader().GetShaderManager().GetLanguage();
	pHasLoadingShader = shaderLanguage.GetHasLoadingShader();
	pHasCompilingShader = shaderLanguage.GetHasCompilingShader();
}

bool deoglShaderCompilingInfo::pShouldBeVisible() const{
	return pHasLoadingShader || pHasCompilingShader;
}

void deoglShaderCompilingInfo::pUpdateState( float elapsed ){
	if( pState == esInvisible ){
		if( ! pShouldBeVisible() ){
			return;
		}
		
		pState = esDelayFadeIn;
		pElapsed = 0.0f;
		
	}else if( pState == esVisible ){
		if( pShouldBeVisible() ){
			return;
		}
		
		pState = esDelayFadeOut;
		pElapsed = 0.0f;
	}
	
	if( pState == esDelayFadeIn ){
		if( ! pShouldBeVisible() ){
			pState = esInvisible;
			pElapsed = 0.0f;
			return;
		}
		
		pElapsed += elapsed;
		if( pElapsed < pDelayFadeIn ){
			return;
		}
		
		pState = esFadeIn;
		pElapsed -= pDelayFadeIn;
		
	}else if( pState == esDelayFadeOut ){
		if( pShouldBeVisible() ){
			pState = esVisible;
			pElapsed = 0.0f;
			return;
		}
		
		pElapsed += elapsed;
		if( pElapsed < pDelayFadeOut ){
			return;
		}
		
		pState = esFadeOut;
		pElapsed -= pDelayFadeOut;
	}
	
	if( pState == esFadeIn ){
		pElapsed += elapsed;
		if( pElapsed >= pTimeFadeIn ){
			pState = esVisible;
			pElapsed = 0.0f;
		}
		pUpdateTransparency();
		
	}else if( pState == esFadeOut ){
		if( pShouldBeVisible() ){
			pState = esFadeIn;
			pElapsed = decMath::linearStep( pElapsed, 0.0f, pTimeFadeOut, pTimeFadeIn, 0.0f );
			
		}else{
			pElapsed += elapsed;
			if( pElapsed >= pTimeFadeOut ){
				pState = esInvisible;
				pElapsed = 0.0f;
			}
		}
		pUpdateTransparency();
	}
}

void deoglShaderCompilingInfo::pUpdateTransparency(){
	switch( pState ){
	case esInvisible:
	case esDelayFadeIn:
		pTransparency = 0.0f;
		break;
		
	case esFadeIn:
		pTransparency = decMath::linearStep( pElapsed, 0.0f, pTimeFadeIn, 0.0f, 1.0f );
		break;
		
	case esVisible:
	case esDelayFadeOut:
		pTransparency = 1.0f;
		break;
		
	case esFadeOut:
		pTransparency = decMath::linearStep( pElapsed, 0.0f, pTimeFadeOut, 1.0f, 0.0f );
		break;
	}
}

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

#include "deoglEnvMapFader.h"
#include "../envmap/deoglEnvironmentMap.h"

#include <dragengine/common/exceptions.h>



// Class deoglEnvMapFader
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglEnvMapFader::deoglEnvMapFader() :
pEnvMapActive( NULL ),
pEnvMapFading( NULL ),
pEnvMapDelayed( NULL ),
pFadePerTime( 1.0f ),
pBlendFactor( 1.0f ){
}

deoglEnvMapFader::~deoglEnvMapFader(){
}



// Management
///////////////

void deoglEnvMapFader::SetFadePerTime( float fadePerTime ){
	if( fadePerTime < 0.1f ){
		pFadePerTime = 0.1f;
		
	}else{
		pFadePerTime = fadePerTime;
	}
}

void deoglEnvMapFader::FadeTo( deoglEnvironmentMap *envmap ){
	if( envmap == pEnvMapActive ){
		return;
	}
	
	if( ! pEnvMapActive ){
		pEnvMapActive = envmap;
		if( envmap ){
			envmap->AddReference();
		}
		return;
	}
	
	// has pEnvMapActive and it is not envmap
	if( ! pEnvMapFading ){
		pEnvMapFading = envmap;
		if( envmap ){
			envmap->AddReference();
		}
		return;
	}
	
	// has pEnvMapActive and it is not envmap. has pEnvMapFading which could be envmap.
	// drop pEnvMapDelayed. this is correct no matter if pEnvMapFading is envmpa or not
	if( pEnvMapDelayed ){
		pEnvMapDelayed->FreeReference();
		pEnvMapDelayed = NULL;
	}
	
	// here pEnvMapActive is not NULL, pEnvMapFading is not NULL and pEnvMapDelayed is NULL.
	// add envmap as delayed env map in case it is not the fading one
	if( envmap == pEnvMapFading ){
		return;
	}
	
	pEnvMapDelayed = envmap;
	if( envmap ){
		envmap->AddReference();
	}
	//printf( "fadeto %p: active=%p fading=%p delayed=%p\n", envmap, pEnvMapActive, pEnvMapFading, pEnvMapDelayed );
}

void deoglEnvMapFader::Drop( deoglEnvironmentMap *envmap ){
	if( ! envmap ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEnvMapDelayed == envmap ){
		pEnvMapDelayed->FreeReference();
		pEnvMapDelayed = NULL;
	}
	
	if( pEnvMapFading == envmap ){
		if( pEnvMapFading ){
			pEnvMapFading->FreeReference();
		}
		pEnvMapFading = pEnvMapDelayed;
		pEnvMapDelayed = NULL;
		pBlendFactor = 1.0f;
	}
	
	if( pEnvMapActive == envmap ){
		if( pEnvMapActive ){
			pEnvMapActive->FreeReference();
		}
		pEnvMapActive = pEnvMapFading;
		pEnvMapFading = pEnvMapDelayed;
		pEnvMapDelayed = NULL;
		pBlendFactor = 1.0f;
	}
	//printf( "drop %p: active=%p fading=%p delayed=%p\n", envmap, pEnvMapActive, pEnvMapFading, pEnvMapDelayed );
}

void deoglEnvMapFader::DropAll(){
	if( pEnvMapActive ){
		pEnvMapActive->FreeReference();
		pEnvMapActive = NULL;
	}
	
	if( pEnvMapFading ){
		pEnvMapFading->FreeReference();
		pEnvMapFading = NULL;
	}
	
	if( pEnvMapDelayed ){
		pEnvMapDelayed->FreeReference();
		pEnvMapDelayed = NULL;
	}
	
	pBlendFactor = 1.0f;
}

bool deoglEnvMapFader::IsFading() const{
	return pEnvMapFading != NULL;
}

void deoglEnvMapFader::Update( float elapsed ){
	if( ! pEnvMapFading ){
		return;
	}
	
	pBlendFactor -= pFadePerTime * elapsed;
	//printf( "update: blend=%f\n", pBlendFactor );
	
	while( pBlendFactor <= 0.0f ){
		if( pEnvMapActive ){
			pEnvMapActive->FreeReference();
		}
		pEnvMapActive = pEnvMapFading;
		pEnvMapFading = pEnvMapDelayed;
		pEnvMapDelayed = NULL;
		
		if( pEnvMapFading ){
			pBlendFactor += 1.0f;
			
		}else{
			pBlendFactor = 1.0f;
		}
		//printf( "done: active=%p fading=%p delayed=%p\n", pEnvMapActive, pEnvMapFading, pEnvMapDelayed );
	}
}

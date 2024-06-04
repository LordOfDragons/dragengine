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

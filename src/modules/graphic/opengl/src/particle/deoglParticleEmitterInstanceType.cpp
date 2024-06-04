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

#include "deoglParticleEmitter.h"
#include "deoglParticleEmitterInstance.h"
#include "deoglParticleEmitterInstanceType.h"
#include "deoglParticleEmitterType.h"
#include "deoglRParticleEmitterInstanceType.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglParticleEmitterInstanceType
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglParticleEmitterInstanceType::deoglParticleEmitterInstanceType(
deoglParticleEmitterInstance &instance, int index ) :
pEmitterInstance( instance ),
pIndex( index ),
pRType( NULL ),
pDirtyType( true ),
pDirtyParamBlocks( true )
{
	try{
		pRType = new deoglRParticleEmitterInstanceType( *instance.GetRInstance(), index );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglParticleEmitterInstanceType::~deoglParticleEmitterInstanceType(){
	pCleanUp();
}



// Management
///////////////

void deoglParticleEmitterInstanceType::SyncToRender(){
	if( pDirtyType ){
		const deParticleEmitterInstanceType &itype = pEmitterInstance.GetInstance().GetTypeAt( pIndex );
		if( itype.GetDynamicSkin() ){
			pRType->SetDynamicSkin( ( ( deoglDynamicSkin* )itype.GetDynamicSkin()->GetPeerGraphic() )->GetRDynamicSkin() );
			
		}else{
			pRType->SetDynamicSkin( NULL );
		}
		
		deoglParticleEmitter * const emitter = pEmitterInstance.GetEmitter();
		if( emitter ){
			const deParticleEmitterType &etype = emitter->GetParticleEmitter().GetTypeAt( pIndex );
			
			if( etype.GetSkin() ){
				pRType->SetUseSkin( ( ( deoglSkin* )etype.GetSkin()->GetPeerGraphic() )->GetRSkin() );
				
			}else{
				pRType->SetUseSkin( NULL );
			}
			
		}else{
			pRType->SetUseSkin( NULL );
		}
		
		pDirtyType = false;
	}
	
	if( pDirtyParamBlocks ){
		pRType->InvalidateParamBlocks();
		pDirtyParamBlocks = false;
	}
}



void deoglParticleEmitterInstanceType::InvalidateParamBlocks(){
	pDirtyParamBlocks = true;
}



void deoglParticleEmitterInstanceType::TypeChanged(){
	pDirtyType = true;
}



// Private Functions
//////////////////////

void deoglParticleEmitterInstanceType::pCleanUp(){
	if( pRType ){
		pRType->FreeReference();
	}
}

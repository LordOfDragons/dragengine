/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglRSkyInstance.h"
#include "deoglRSkyInstanceLayer.h"
#include "deoglSkyLayerGIState.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../gi/deoglGIState.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shadow/deoglShadowCaster.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkyLayerGIState
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyLayerGIState::deoglSkyLayerGIState( deoglRSkyInstanceLayer &layer, const deoglGIState *giState ) :
pLayer( layer ),
pGIState( giState ),
pPositionThreshold( 5.0 ),
pShadowCaster( NULL ),
pDirtyStaticShadow( true )
{
	try{
		pTracker.SetThresholdOrientation( deoglSkyLayerTracker::THRESHOLD_ONE_DEGREE_ORIENTATION * 5.0f );
		
		pShadowCaster = new deoglShadowCaster( layer.GetInstance().GetRenderThread() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglSkyLayerGIState::~deoglSkyLayerGIState(){
	pCleanUp();
}



// Management
///////////////

void deoglSkyLayerGIState::Update(){
	if( ( pGIState->GetPosition() - pPosition ).Length() >= pPositionThreshold ){
		pPosition = pGIState->GetPosition();
		pDirtyStaticShadow = true;
			pLayer.GetInstance().GetRenderThread().GetLogger().LogInfoFormat("Update: Position Changed: %p", &pLayer);
	}
	
	pDirtyStaticShadow |= pTracker.UpdateOrientation( pLayer.GetMatrix().ToQuaternion() );
	pDirtyStaticShadow |= pTracker.UpdateLightOrientation( pLayer.GetLightOrientation() );
}

void deoglSkyLayerGIState::ClearDirtyStaticShadow(){
	pDirtyStaticShadow = false;
}

void deoglSkyLayerGIState::NotifyUpdateStaticComponent( deoglRComponent* ){
	// TODO if the component is too small do mark dirty
	
	// we could do things complicated here to dirty static shadow maps only in the most
	// necessary case. instead we dirty in all cases. this does dirty in some situations
	// where we could avoid it but these situations are few and far in between
		if( ! pDirtyStaticShadow ){
			pLayer.GetInstance().GetRenderThread().GetLogger().LogInfoFormat("NotifyUpdateStaticComponent: %p", &pLayer);
		}
	pDirtyStaticShadow = true;
}



// Private Functions
//////////////////////

class deoglSkyLayerGIStateDeletion : public deoglDelayedDeletion{
public:
	deoglShadowCaster *shadowCaster;
	
	deoglSkyLayerGIStateDeletion() : shadowCaster( NULL ){
	}
	
	virtual ~deoglSkyLayerGIStateDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( shadowCaster ){
			delete shadowCaster;
		}
	}
};

void deoglSkyLayerGIState::pCleanUp(){
	// delayed deletion of opengl containing objects
	deoglSkyLayerGIStateDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglSkyLayerGIStateDeletion;
		delayedDeletion->shadowCaster = pShadowCaster;
		pLayer.GetInstance().GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pLayer.GetInstance().GetRenderThread().GetLogger().LogException( e );
		//throw; -> otherwise terminate
	}
}

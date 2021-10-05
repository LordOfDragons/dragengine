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
#include "deoglSkyLayerGICascade.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../gi/deoglGICascade.h"
#include "../gi/deoglGICascade.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shadow/deoglShadowCaster.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkyLayerGICascade
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyLayerGICascade::deoglSkyLayerGICascade( deoglRSkyInstanceLayer &layer, const deoglGICascade &cascade ) :
pLayer( layer ),
pGICascade( cascade ),
pPosition( cascade.GetPosition() ),
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

deoglSkyLayerGICascade::~deoglSkyLayerGICascade(){
	pCleanUp();
}



// Management
///////////////

void deoglSkyLayerGICascade::Update(){
	if( ( pGICascade.GetPosition() - pPosition ).Length() >= pPositionThreshold ){
		pPosition = pGICascade.GetPosition();
		pDirtyStaticShadow = true;
	}
	
	pDirtyStaticShadow |= pTracker.UpdateOrientation( pLayer.GetMatrix().ToQuaternion() );
	pDirtyStaticShadow |= pTracker.UpdateLightOrientation( pLayer.GetLightOrientation() );
}

void deoglSkyLayerGICascade::ClearDirtyStaticShadow(){
	pDirtyStaticShadow = false;
}

void deoglSkyLayerGICascade::NotifyUpdateStaticComponent( deoglRComponent* ){
	// TODO if the component is too small do not mark dirty
	
	// we could do things complicated here to dirty static shadow maps only in the most
	// necessary case. instead we dirty in all cases. this does dirty in some situations
	// where we could avoid it but these situations are few and far in between
	pDirtyStaticShadow = true;
}



// Private Functions
//////////////////////

void deoglSkyLayerGICascade::pCleanUp(){
	if( pShadowCaster ){
		delete pShadowCaster;
	}
}

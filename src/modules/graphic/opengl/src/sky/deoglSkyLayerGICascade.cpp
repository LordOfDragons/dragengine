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
#include <string.h>

#include "deoglRSkyInstance.h"
#include "deoglRSkyInstanceLayer.h"
#include "deoglSkyLayerGICascade.h"
#include "../delayedoperation/deoglDelayedOperations.h"
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

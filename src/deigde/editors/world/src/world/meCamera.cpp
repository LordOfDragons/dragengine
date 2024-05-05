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
#include "meWorld.h"
#include "meCamera.h"
#include "meWorld.h"
#include "object/meObject.h"

#include "dragengine/deEngine.h"
#include "dragengine/resources/world/deWorld.h"
#include "dragengine/resources/camera/deCamera.h"
#include "dragengine/resources/camera/deCameraManager.h"
#include "dragengine/resources/debug/deDebugDrawer.h"
#include "dragengine/resources/debug/deDebugDrawerShape.h"
#include "dragengine/common/exceptions.h"



// Class meCamera
///////////////////

// Constructor, destructor
////////////////////////////

meCamera::meCamera( deEngine *engine ) :
igdeCamera( engine ),

pDDVolume( NULL ),
pWorld( NULL ),
pHostObject( NULL )
{
	SetLowestIntensity( 1.0f );
	SetHighestIntensity( 20.0f );
	
	GetEngineCamera()->GetLayerMask().SetBit( meWorld::elmCamera );
	GetEngineCamera()->GetLayerMask().SetBit( meWorld::elmEnvMapProbes );
	GetEngineCamera()->NotifyLayerMaskChanged();
}

meCamera::~meCamera(){
	SetWorld( NULL );
}



// Management
///////////////

void meCamera::Dispose(){
	SetHostObject( NULL );
}

void meCamera::SetWorld( meWorld *world ){
	if( world == pWorld ) return;
	
	if( pWorld ){
		if( pDDVolume ){
			//pWorld->GetDDCameras()->RemoveVolume( pDDVolume );
			pDDVolume = NULL;
		}
	}
	
	pWorld = world;
	
	if( world ){
		SetEngineWorld( world->GetEngineWorld() );
		
	}else{
		SetEngineWorld( NULL );
	}
	
	pUpdateDDVGeometry();
	pUpdateDDVolume();
}



bool meCamera::HasHostObject() const{
	return pHostObject != NULL;
}

void meCamera::SetHostObject( meObject *object ){
	pHostObject = object;
}

void meCamera::Reset(){
	igdeCamera::Reset();
	SetPosition( decVector() );
}



void meCamera::GeometryChanged(){
	pUpdateDDVGeometry();
}

void meCamera::AdaptionChanged(){
}



// Private Functions
//////////////////////

void meCamera::pUpdateDDVolume(){
	/*
	// check if we need a volume and create or destroy it if required
	if( pWorld && pVisible && ( pCurrent || pSelected ) ){
		if( ! pDDVolume ){
			try{
				pDDVolume = new deDebugDrawerShape;
				if( ! pDDVolume ) DETHROW( deeOutOfMemory );
				pWorld->GetDDObjects()->AddVolume( pDDVolume );
				
			}catch( const deException & ){
				if( pDDVolume ){
					delete pDDVolume;
					pDDVolume = NULL;
				}
				throw;
			}
			pUpdateDDVGeometry();
		}
		
	}else if( pDDVolume ){
		pWorld->GetDDObjects()->RemoveVolume( pDDVolume );
		pDDVolume = NULL;
	}
	
	// update color if volume exists
	if( pDDVolume ){
		if( pCurrent ){
			pDDVolume->SetEdgeColor( decColor( 1.0f, 0.5f, 0.0f, 1.0 ) );
		}else if( pSelected ){
			pDDVolume->SetEdgeColor( decColor( 1.0f, 0.0f, 0.0f, 1.0 ) );
		}else{
			pDDVolume->SetEdgeColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		}
		pDDVolume->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
	}
	*/
}

void meCamera::pUpdateDDVGeometry(){
	/*
	if( pDDVolume ){
		decCollisionBox *colBox = NULL;
		
		try{
			colBox = new decCollisionBox( pPosition,
				pSize * 0.5f + decVector( 0.01f, 0.01f, 0.01f ),
				decMatrix::CreateRotation( pRotation ).ToQuaternion() );
			pDDVolume->SetVolume( colBox );
		}catch( const deException & ){
			if( colBox ) delete colBox;
		}
	}
	*/
}

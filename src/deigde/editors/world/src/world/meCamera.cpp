/* 
 * Drag[en]gine IGDE World Editor
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

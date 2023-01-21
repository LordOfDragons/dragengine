/* 
 * Drag[en]gine IGDE Sky Editor
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
#include <string.h>
#include <stdlib.h>

#include "seUControllerRemove.h"
#include "../../sky/seSky.h"
#include "../../sky/controller/seController.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seUControllerRemove
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUControllerRemove::seUControllerRemove( seController *controller ) :
pSky( nullptr ),
pController( nullptr ),
pIndex( 0 ),
pLinks( nullptr ),
pLinkCount( 0 )
{
	DEASSERT_NOTNULL( controller )
	
	seSky * const sky = controller->GetSky();
	DEASSERT_NOTNULL( sky )
	
	SetShortInfo( "Remove Controller" );
	
	pIndex = sky->GetControllers().IndexOf( controller );
	DEASSERT_TRUE( pIndex != -1 )
	
	const int usageCount = sky->CountControllerUsage( controller );
	if( usageCount > 0 ){
		const seLayerList &layers = sky->GetLayers();
		const int layerCount = layers.GetCount();
		int i, j, k;
		
		try{
			pLinks = new seLink*[ usageCount ];
			
			for( i=0; i<layerCount; i++ ){
				seLayer * const layer = layers.GetAt( i );
				
				for( j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++ ){
					const deSkyLayer::eTargets target = ( deSkyLayer::eTargets )j;
					const seLinkList &links = layer->GetTarget( target ).GetLinks();
					
					const int linkCount = links.GetCount();
					for( k=0; k<linkCount; k++ ){
						seLink * const link = links.GetAt( k );
						if( link->GetController() != controller ){
							continue;
						}
						
						pLinks[ pLinkCount ] = link;
						link->AddReference();
						pLinkCount++;
					}
				}
			}
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	pSky = sky;
	sky->AddReference();
	
	pController = controller;
	controller->AddReference();
}

seUControllerRemove::~seUControllerRemove(){
	pCleanUp();
}



// Management
///////////////

void seUControllerRemove::Undo(){
	pSky->InsertControllerAt( pController, pIndex );
	
	int i;
	for( i=0; i<pLinkCount; i++ ){
		pLinks[ i ]->SetController( pController );
	}
}

void seUControllerRemove::Redo(){
	pSky->RemoveController( pController );
	
	int i;
	for( i=0; i<pLinkCount; i++ ){
		pLinks[ i ]->SetController( nullptr );
	}
}



// Private Functions
//////////////////////

void seUControllerRemove::pCleanUp(){
	int i;
	for( i=0; i<pLinkCount; i++ ){
		pLinks[ i ]->FreeReference();
	}
	
	if( pController ){
		pController->FreeReference();
	}
	if( pSky ){
		pSky->FreeReference();
	}
}

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

#include "seULinkRemove.h"
#include "../../sky/seSky.h"
#include "../../sky/link/seLink.h"
#include "../../sky/layer/seLayer.h"

#include <dragengine/common/exceptions.h>



// Class seULinkRemove
////////////////////////

// Constructor, destructor
////////////////////////////

seULinkRemove::seULinkRemove( seLink *link ) :
pSky( NULL ),
pLink( NULL ),
pTargets( NULL ),
pTargetCount( 0 )
{
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	
	seSky * const sky = link->GetSky();
	if( ! sky ){
		DETHROW( deeInvalidParam );
	}
	
	if( sky->GetLinks().IndexOf( link ) == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove Link" );
	
	const int targetCount = sky->CountLinkUsage( link );
	if( targetCount > 0 ){
		const seLayerList &layers = sky->GetLayers();
		const int layerCount = layers.GetCount();
		int i, j;
		
		try{
			pTargets = new sTarget[ targetCount ];
			
			for( i=0; i<layerCount; i++ ){
				seLayer * const layer = layers.GetAt( i );
				
				for( j=deSkyLayer::etOffsetX; j<=deSkyLayer::etAmbientIntensity; j++ ){
					const deSkyLayer::eTargets target = ( deSkyLayer::eTargets )j;
					
					if( ! layer->GetTarget( target ).GetLinks().Has( link ) ){
						continue;
					}
					
					pTargets[ pTargetCount ].layer = layer;
					pTargets[ pTargetCount ].target = target;
					layer->AddReference();
					pTargetCount++;
				}
			}
			
		}catch( const deException & ){
			pCleanUp();
			throw;
		}
	}
	
	pSky = sky;
	sky->AddReference();
	
	pLink = link;
	link->AddReference();
}

seULinkRemove::~seULinkRemove(){
	pCleanUp();
}



// Management
///////////////

void seULinkRemove::Undo(){
	pSky->AddLink( pLink );
	
	int i;
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].layer->GetTarget( pTargets[ i ].target ).AddLink( pLink );
		pTargets[ i ].layer->NotifyTargetChanged( pTargets[ i ].target );
	}
}

void seULinkRemove::Redo(){
	int i;
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].layer->GetTarget( pTargets[ i ].target ).RemoveLink( pLink );
		pTargets[ i ].layer->NotifyTargetChanged( pTargets[ i ].target );
	}
	
	pSky->RemoveLink( pLink );
}



// Private Functions
//////////////////////

void seULinkRemove::pCleanUp(){
	int i;
	for( i=0; i<pTargetCount; i++ ){
		pTargets[ i ].layer->FreeReference();
	}
	
	if( pLink ){
		pLink->FreeReference();
	}
	if( pSky ){
		pSky->FreeReference();
	}
}

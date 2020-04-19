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

#include "seUTargetRemoveLink.h"
#include "../../sky/controller/seControllerTarget.h"
#include "../../sky/layer/seLayer.h"
#include "../../sky/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seUTargetRemoveLink
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUTargetRemoveLink::seUTargetRemoveLink( seLayer *layer,
deSkyLayer::eTargets target, seLink *link ) :
pLayer( NULL ),
pTarget( target ),
pLink( NULL )
{
	if( ! layer || ! link ){
		DETHROW( deeInvalidParam );
	}
	if( ! layer->GetTarget( target ).GetLinks().Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Target Remove Link" );
	
	pLayer = layer;
	layer->AddReference();
	
	pLink = link;
	link->AddReference();
}

seUTargetRemoveLink::~seUTargetRemoveLink(){
	if( pLink ){
		pLink->FreeReference();
	}
	if( pLayer ){
		pLayer->FreeReference();
	}
}



// Management
///////////////

void seUTargetRemoveLink::Undo(){
	pLayer->GetTarget( pTarget ).AddLink( pLink );
	pLayer->NotifyTargetChanged( pTarget );
}

void seUTargetRemoveLink::Redo(){
	pLayer->GetTarget( pTarget ).RemoveLink( pLink );
	pLayer->NotifyTargetChanged( pTarget );
}

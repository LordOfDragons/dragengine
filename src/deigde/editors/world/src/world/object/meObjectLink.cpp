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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "meObject.h"
#include "meObjectLink.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWConnection.h>



// Class meObjectLink
///////////////////////

// Constructor, destructor
////////////////////////////

meObjectLink::meObjectLink( igdeEnvironment *environment, meObject *anchor, meObject *target ){
	if( ! environment || ! anchor || ! target  ){
		DETHROW( deeInvalidParam );
	}
	
	pEnvironment = environment;
	pWorld = NULL;
	
	pDebugDrawer = NULL;
	pDDSConnection = NULL;
	
	pAnchor = NULL;
	pTarget = NULL;
	
	try{
		pAnchor = anchor;
		anchor->AddReference();
		
		pTarget = target;
		target->AddReference();
		
		// create debug drawer and shapes
		pDebugDrawer = environment->GetEngineController()->GetEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSConnection = new igdeWConnection;
		pDDSConnection->SetArrowSize( 0.05f );
		pDDSConnection->SetArrowOnAnchor( false );
		pDDSConnection->SetArrowOnTarget( true );
		pDDSConnection->SetColor( decColor( 1.0f, 0.0f, 1.0f ) );
		pDDSConnection->SetParentDebugDrawer( pDebugDrawer );
		
		// update the rest
		pUpdateConnection();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meObjectLink::~meObjectLink(){
	pCleanUp();
}



// Management
///////////////

void meObjectLink::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
	}
	
	pWorld = world;
	
	if( world ){
		world->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
	}
}

void meObjectLink::SetAnchorProperty( const char *property ){
	pAnchorProperty = property;
}

void meObjectLink::SetTargetProperty( const char *property ){
	pTargetProperty = property;
}

void meObjectLink::ObjectsMoved(){
	pUpdateConnection();
}



// Private Functions
//////////////////////

void meObjectLink::pCleanUp(){
	SetWorld( NULL );
	
	if( pTarget ){
		pTarget->FreeReference();
	}
	if( pAnchor ){
		pAnchor->FreeReference();
	}
	
	if( pDDSConnection ){
		delete pDDSConnection;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}

void meObjectLink::pUpdateConnection(){
	const decDVector &anchorPosition = pAnchor->GetPosition();
	const decDVector &targetPosition = pTarget->GetPosition();
	
	pDebugDrawer->SetPosition( anchorPosition );
	pDDSConnection->SetTargetPosition( ( targetPosition - anchorPosition ).ToVector() );
}

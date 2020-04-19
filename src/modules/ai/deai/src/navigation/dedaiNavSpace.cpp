/* 
 * Drag[en]gine AI Module
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

#include "dedaiNavSpace.h"
#include "spaces/dedaiSpace.h"
#include "../deDEAIModule.h"
#include "../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>



// Class dedaiNavSpace
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavSpace::dedaiNavSpace( deDEAIModule &deai, const deNavigationSpace &navigationSpace ) :
pDEAI( deai ),
pNavigationSpace( navigationSpace ),
pParentWorld( NULL ),
pSpace( NULL )
{
	try{
		pSpace = new dedaiSpace( deai );
		pSpace->SetOwnerNavSpace( this );
		pSpace->SetPosition( navigationSpace.GetPosition() );
		pSpace->SetOrientation( navigationSpace.GetOrientation() );
		pSpace->SetLayerNumber( navigationSpace.GetLayer() );
		pSpace->SetType( navigationSpace.GetType() );
		pSpace->SetSnapDistance( navigationSpace.GetSnapDistance() );
		pSpace->SetSnapAngle( navigationSpace.GetSnapAngle() );
		pSpace->SetBlockerShape( navigationSpace.GetBlockerShapeList() );
		pSpace->SetBlockingPriority( navigationSpace.GetBlockingPriority() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedaiNavSpace::~dedaiNavSpace(){
	pCleanUp();
}



// Management
///////////////

void dedaiNavSpace::SetParentWorld( dedaiWorld *world ){
	pParentWorld = world;
	pSpace->SetParentWorld( world );
}



// Notifications
//////////////////

void dedaiNavSpace::PositionChanged(){
	pSpace->SetPosition( pNavigationSpace.GetPosition() );
}

void dedaiNavSpace::OrientationChanged(){
	pSpace->SetOrientation( pNavigationSpace.GetOrientation() );
}

void dedaiNavSpace::LayerChanged(){
	pSpace->SetLayerNumber( pNavigationSpace.GetLayer() );
}

void dedaiNavSpace::TypeChanged(){
	pSpace->SetType( pNavigationSpace.GetType() );
}

void dedaiNavSpace::SnappingChanged(){
	pSpace->SetSnapDistance( pNavigationSpace.GetSnapDistance() );
	pSpace->SetSnapAngle( pNavigationSpace.GetSnapAngle() );
}

void dedaiNavSpace::BlockerShapeChanged(){
	pSpace->SetBlockerShape( pNavigationSpace.GetBlockerShapeList() );
}

void dedaiNavSpace::BlockingPriorityChanged(){
	pSpace->SetBlockingPriority( pNavigationSpace.GetBlockingPriority() );
}

void dedaiNavSpace::LayoutChanged(){
	pSpace->OwnerLayoutChanged();
}



// Private Functions
//////////////////////

void dedaiNavSpace::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pSpace ){
		delete pSpace;
	}
}

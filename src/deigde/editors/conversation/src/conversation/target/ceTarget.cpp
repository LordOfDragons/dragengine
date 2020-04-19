/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceTarget.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"
#include "../playback/cePlaybackCamera.h"
#include "../actor/ceConversationActor.h"
#include "../coordsystem/ceCoordSystem.h"

#include <deigde/gui/igdeCamera.h>

#include <dragengine/common/exceptions.h>



// Class ceTarget
///////////////////

// Constructor, destructor
////////////////////////////

ceTarget::ceTarget( const char *name ) :
pConversation( NULL ),
pName( name ){
}

ceTarget::ceTarget( const ceTarget &target ){
	pConversation = NULL;
	pName = target.GetName();
	   pActor = target.GetActor();
	   pCoordSystem = target.GetCoordSystem();
	pBone = target.GetBone();
	pPosition = target.GetPosition();
	pOrientation = target.GetOrientation();
}

ceTarget::~ceTarget(){
}



// Management
///////////////

void ceTarget::SetConversation( ceConversation *conversation ){
	pConversation = conversation;
}

void ceTarget::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pName.Equals( name ) ){
		if( pConversation && pConversation->GetTargetList().HasNamed( name ) ){
			DETHROW( deeInvalidParam );
		}
		
		pName = name;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}

void ceTarget::SetActor( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pActor.Equals( id ) ){
		      pActor = id;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}

void ceTarget::SetCoordSystem( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pCoordSystem.Equals( id ) ){
		      pCoordSystem = id;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}

void ceTarget::SetBone( const char *bone ){
	if( ! bone ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pBone.Equals( bone ) ){
		pBone = bone;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}

void ceTarget::SetPosition( const decVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}

void ceTarget::SetOrientation( const decVector &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		
		if( pConversation ){
			pConversation->NotifyTargetChanged( this );
		}
	}
}



void ceTarget::GetCoordinateSystem( cePlayback &playback, decMatrix &coordinateSystem ){
	coordinateSystem.SetRT( pOrientation * DEG2RAD, pPosition );
	//coordinateSystem = decMatrix::CreateTranslation( pPosition )
	//	* decMatrix::CreateRotation( pOrientation * DEG2RAD );
	
	if( ! pConversation ){
		return;
	}
	
	const ceConversationActorList &actorList = pConversation->GetActorList();
	const ceConversationActor *actor = NULL;
	
	// coordinate system
	if( ! pCoordSystem.IsEmpty() ){
		const ceCoordSystem * const coordSystem = pConversation->GetCoordSystemList().GetWithIDOrAliasID( pCoordSystem );
		if( coordSystem ){
			coordinateSystem *= decMatrix::CreateRT( coordSystem->GetOrientation() * DEG2RAD, coordSystem->GetPosition() );
		}
		
	// actor
	}else if( ! pActor.IsEmpty() ){
		actor = actorList.GetWithIDOrAliasID( pActor );
		if( actor ){
			if( ! pBone.IsEmpty() ){
				coordinateSystem *= actor->GetBoneMatrix( pBone );
			}
			coordinateSystem *= decMatrix::CreateRT( actor->GetOrientation() * DEG2RAD, actor->GetPosition() );
		}
		
	// otherwise use current camera location
	}else{
		coordinateSystem *= playback.GetConversation().GetCamera()->GetViewMatrix();
	}
}


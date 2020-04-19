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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceProp.h"
#include "../ceConversation.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceProp
/////////////////

// Constructor, destructor
////////////////////////////

ceProp::ceProp(){
	pConversation = NULL;
	
	pObjectWrapper = NULL;
	
	pName = "Prop";
	pVisible = true;
	
	pObjectClass = "GenericProp";
}

ceProp::~ceProp(){
	pCleanUp();
}



// Management
///////////////

void ceProp::SetConversation( ceConversation *conversation ){
	if( conversation != pConversation ){
		if( pConversation ){
			delete pObjectWrapper;
			pObjectWrapper = NULL;
		}
		
		pConversation = conversation;
		
		if( conversation ){
			const decQuaternion &orientation = decMatrix::CreateRotation( pOrientation * DEG2RAD ).ToQuaternion();
			
			pObjectWrapper = new igdeWObject( *conversation->GetEnvironment() );
			pObjectWrapper->SetGDClassName( pObjectClass.GetString() );
			pObjectWrapper->SetPosition( pPosition );
			pObjectWrapper->SetOrientation( orientation );
			pObjectWrapper->SetVisible( pVisible );
			pObjectWrapper->SetDynamicCollider( false );
			pObjectWrapper->SetWorld( conversation->GetEngineWorld() );
		}
	}
}



void ceProp::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pName.Equals( name ) ){
		pName = name;
		
		if( pConversation ){
			pConversation->NotifyPropChanged( this );
		}
	}
}

void ceProp::SetObjectClass( const char *objectClass ){
	if( ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pObjectClass.Equals( objectClass ) ){
		pObjectClass = objectClass;
		if( pObjectWrapper ){
			pObjectWrapper->SetGDClassName( pObjectClass.GetString() );
		}
		
		if( pConversation ){
			pConversation->NotifyPropChanged( this );
		}
	}
}

void ceProp::SetPosition( const decVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		if( pObjectWrapper ){
			pObjectWrapper->SetPosition( decDVector( position ) );
		}
		
		if( pConversation ){
			pConversation->NotifyPropChanged( this );
		}
	}
}

void ceProp::SetOrientation( const decVector &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		if( pObjectWrapper ){
			pObjectWrapper->SetOrientation( decMatrix::CreateRotation( orientation * DEG2RAD ).ToQuaternion() );
		}
		
		if( pConversation ){
			pConversation->NotifyPropChanged( this );
		}
	}
}

void ceProp::SetVisible( bool visible ){
	if( visible != pVisible ){
		pVisible = visible;
		if( pObjectWrapper ){
			pObjectWrapper->SetVisible( visible );
		}
		
		if( pConversation ){
			pConversation->NotifyPropChanged( this );
		}
	}
}



// Private Functions
//////////////////////

void ceProp::pCleanUp(){
	SetConversation( NULL );
}

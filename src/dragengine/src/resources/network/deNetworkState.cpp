/* 
 * Drag[en]gine Game Engine
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "deNetworkState.h"
#include "deNetworkStateManager.h"
#include "value/deNetworkValue.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/network/deBaseNetworkState.h"
#include "../../systems/modules/scripting/deBaseScriptingNetworkState.h"



// Class deNetworkState
/////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkState::deNetworkState( deNetworkStateManager *manager, bool readOnly ) :
deResource( manager ),

pReadOnly( readOnly ),

pPeerNetwork( NULL ),
pPeerScripting( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ){
}

deNetworkState::~deNetworkState(){
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
}



// Management
///////////////




// Properties
///////////////

int deNetworkState::GetValueCount() const{
	return pValues.GetCount();
}

int deNetworkState::IndexOfValue( deNetworkValue *value ) const{
	return pValues.IndexOf( value );
}

deNetworkValue *deNetworkState::GetValueAt( int index ) const{
	return ( deNetworkValue* )pValues.GetAt( index );
}

void deNetworkState::AddValue( deNetworkValue *value ){
	if( ! value ){
		DETHROW( deeInvalidParam );
	}
	pValues.Add( value );
	
	if( pPeerNetwork ){
		pPeerNetwork->ValueAdded( pValues.GetCount() - 1, value );
	}
}

void deNetworkState::NotifyValueChanged( int index ) const{
	if( pPeerNetwork ){
		pPeerNetwork->ValueChanged( index, GetValueAt( index ) );
	}
}

void deNetworkState::NotifyPrecisionChanged( int index ) const{
	if( pPeerNetwork ){
		pPeerNetwork->PrecisionChanged( index, GetValueAt( index ) );
	}
}



// System Peers
/////////////////

void deNetworkState::SetPeerNetwork ( deBaseNetworkState *peer ){
	if( peer == pPeerNetwork ){
		return;
	}
	
	if( pPeerNetwork ){
		delete pPeerNetwork;
	}
	pPeerNetwork = peer;
}

void deNetworkState::SetPeerScripting( deBaseScriptingNetworkState *peer ){
	if( peer == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}



// Linked List
////////////////

void deNetworkState::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deNetworkState::SetLLWorldPrev( deNetworkState *networkState ){
	pLLWorldPrev = networkState;
}

void deNetworkState::SetLLWorldNext( deNetworkState *networkState ){
	pLLWorldNext = networkState;
}

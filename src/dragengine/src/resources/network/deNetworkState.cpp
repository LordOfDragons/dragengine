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

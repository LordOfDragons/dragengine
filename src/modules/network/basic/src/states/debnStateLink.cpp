/* 
 * Drag[en]gine Basic Network Module
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

#include "debnState.h"
#include "debnStateLink.h"
#include "debnStateLinkList.h"
#include "../debnConnection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/network/deNetworkState.h>



// Class debnStateLink
////////////////////////

// Constructor, destructor
////////////////////////////

debnStateLink::debnStateLink( debnState *bnState, debnConnection &connection ) :
pState( bnState ),
pIdentifier( -1 ),
pLinkState( elsDown ),
pConnection( connection ),
pChanged( false ),
pValueChanged( NULL ),
pValueCount( 0 ),
pPreviousLink( NULL ),
pNextLink( NULL )
{
	try{
		const int valueCount = bnState->GetValueCount();
		if( valueCount > 0 ){
			pValueChanged = new bool[ valueCount ];
			for( pValueCount=0; pValueCount<valueCount; pValueCount++ ){
				pValueChanged[ pValueCount ] = false;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debnStateLink::~debnStateLink(){
	pCleanUp();
}



// Management
///////////////

void debnStateLink::DropState(){
	pState = NULL;
}

void debnStateLink::SetIdentifier( int identifier ){
	if( identifier < -1 ){
		DETHROW( deeInvalidParam );
	}
	pIdentifier = identifier;
}

void debnStateLink::SetLinkState( int linkState ){
	pLinkState = linkState;
}

void debnStateLink::SetChanged( bool changed ){
	if( changed == pChanged ){
		return;
	}
	
	pChanged = changed;
	
	if( changed ){
		pConnection.GetModifiedStateLinks()->AddLink( this );
	}
}



bool debnStateLink::GetValueChangedAt( int index ) const{
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	return pValueChanged[ index ];
}

void debnStateLink::SetValueChangedAt( int index, bool changed ){
	if( index < 0 || index >= pValueCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( changed == pValueChanged[ index ] ){
		return;
	}
	
	pValueChanged[ index ] = changed;
	
	if( changed ){
		SetChanged( true );
	}
}

bool debnStateLink::HasChangedValues() const{
	int i;
	for( i=0; i<pValueCount; i++ ){
		if( pValueChanged[ i ] ){
			return true;
		}
	}
	return false;
}

void debnStateLink::ResetChanged(){
	pChanged = false;
	
	int i;
	for( i=0; i<pValueCount; i++ ){
		pValueChanged[ i ] = false;
	}
}



// Linked List
////////////////

void debnStateLink::SetPreviousLink( debnStateLink *link ){
	pPreviousLink = link;
}

void debnStateLink::SetNextLink( debnStateLink *link ){
	pNextLink = link;
}



// Private Functions
//////////////////////

void debnStateLink::pCleanUp(){
	if( pValueChanged ){
		delete [] pValueChanged;
	}
}

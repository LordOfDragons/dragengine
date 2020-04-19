/* 
 * Drag[en]gine IGDE Rig Editor
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
#include "reSelectionPushes.h"
#include "reRigPush.h"
#include "reRigPushList.h"
#include "../reRig.h"
#include "dragengine/common/exceptions.h"



// Class reSelectionPushes
////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionPushes::reSelectionPushes( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	pRig = rig;
	
	pPushes = NULL;
	pPushCount = 0;
	pPushSize = 0;
	pActivePush = NULL;
}

reSelectionPushes::~reSelectionPushes(){
	Reset();
	if( pPushes ) delete [] pPushes;
}



// Management
///////////////

reRigPush *reSelectionPushes::GetPushAt( int index ) const{
	if( index < 0 || index >= pPushCount ) DETHROW( deeOutOfBoundary );
	
	return pPushes[ index ];
}

bool reSelectionPushes::HasPush( reRigPush *push ) const{
	if( ! push ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPushCount; i++ ){
		if( push == pPushes[ i ] ){
			return true;
		}
	}
	
	return false;
}
	
int reSelectionPushes::IndexOfPush( reRigPush *push ) const{
	if( ! push ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPushCount; i++ ){
		if( push == pPushes[ i ] ){
			return i;
		}
	}
	
	return -1;
}

int reSelectionPushes::IndexOfPushWith( deColliderVolume *collider ) const{
	if( ! collider ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pPushCount; i++ ){
		if( collider == pPushes[ i ]->GetCollider() ){
			return i;
		}
	}
	
	return -1;
}

void reSelectionPushes::AddPush( reRigPush *push ){
	if( HasPush( push ) ) DETHROW( deeInvalidParam );
	
	if( pPushCount == pPushSize ){
		int newSize = pPushSize * 3 / 2 + 1;
		reRigPush **newArray = new reRigPush*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pPushes ){
			memcpy( newArray, pPushes, sizeof( reRigPush* ) * pPushSize );
			delete [] pPushes;
		}
		pPushes = newArray;
		pPushSize = newSize;
	}
	
	pPushes[ pPushCount ] = push;
	pPushCount++;
	
	push->AddReference();
	
	push->SetSelected( true );
	
	pRig->NotifyPushSelectedChanged( push );
	
	if( pActivePush == NULL ){
		SetActivePush( push );
	}
}

void reSelectionPushes::RemovePush( reRigPush *push ){
	int i, index = IndexOfPush( push );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pPushCount; i++ ){
		pPushes[ i - 1 ] = pPushes[ i ];
	}
	pPushes[ pPushCount - 1 ] = NULL;
	pPushCount--;
	
	push->SetSelected( false );
	
	if( push == pActivePush ){
		if( pPushCount > 0 ){
			SetActivePush( pPushes[ 0 ] );
			
		}else{
			SetActivePush( NULL );
		}
	}
	
	pRig->NotifyPushSelectedChanged( push );
	
	push->FreeReference();
}

void reSelectionPushes::RemoveAllPushes(){
	SetActivePush( NULL );
	
	pRig->NotifyAllPushesDeselected();
	
	while( pPushCount > 0 ){
		pPushCount--;
		
		pPushes[ pPushCount ]->SetSelected( false );
		pPushes[ pPushCount ]->FreeReference();
	}
}



bool reSelectionPushes::HasActivePush() const{
	return pActivePush != NULL;
}

void reSelectionPushes::SetActivePush( reRigPush *push ){
	if( push != pActivePush ){
		if( push && ! HasPush( push ) ) DETHROW( deeInvalidParam );
		
		if( pActivePush ){
			pActivePush->SetActive( false );
		}
		
		pActivePush = push;
		
		if( push ){
			push->SetActive( true );
		}
		
		pRig->NotifyActivePushChanged();
	}
}

void reSelectionPushes::Reset(){
	RemoveAllPushes();
}

void reSelectionPushes::AddVisiblePushesTo( reRigPushList &list ) const{
	int c;
	
	for( c=0; c<pPushCount; c++ ){
		if( pPushes[ c ]->IsVisible() ){
			list.AddPush( pPushes[ c ] );
		}
	}
}

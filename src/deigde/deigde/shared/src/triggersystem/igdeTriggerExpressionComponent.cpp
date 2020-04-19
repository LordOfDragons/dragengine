/* 
 * Drag[en]gine IGDE
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

#include "igdeTriggerTarget.h"
#include "igdeTriggerTargetList.h"
#include "igdeTriggerExpressionComponent.h"
#include "igdeTriggerListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionComponent
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionComponent::igdeTriggerExpressionComponent() :
pNegate( false ),
pCurState( false ),
pType( ectTarget ),
pTarget( NULL ),
pTargetListener( NULL ){
}

igdeTriggerExpressionComponent::~igdeTriggerExpressionComponent(){
	if( pTargetListener ){
		pTarget->RemoveListener( pTargetListener );
		pTargetListener->FreeReference();
	}
	if( pTarget ){
		pTarget->FreeReference();
	}
}



// Management
///////////////

void igdeTriggerExpressionComponent::SetNegate( bool negate ){
	pNegate = negate;
}

void igdeTriggerExpressionComponent::SetCurState( bool curState ){
	pCurState = curState;
}

void igdeTriggerExpressionComponent::SetType ( eComponentTypes type ){
	if( type < ectTarget || type > ectOr ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void igdeTriggerExpressionComponent::SetTargetName( const char *name ){
	pTargetName = name;
}

void igdeTriggerExpressionComponent::SetTarget( igdeTriggerTarget *target ){
	if( target == pTarget ){
		return;
	}
	
	if( pTarget ){
		pTarget->FreeReference();
	}
	
	pTarget = target;
	
	if( target ){
		target->AddReference();
	}
}

void igdeTriggerExpressionComponent::SetTargetListener( igdeTriggerListener *listener ){
	if( listener == pTargetListener ){
		return;
	}
	
	if( pTargetListener ){
		pTargetListener->FreeReference();
	}
	
	pTargetListener = listener;
	
	if( listener ){
		listener->AddReference();
	}
}

void igdeTriggerExpressionComponent::LinkTargets( igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener ){
	if( pTargetListener ){
		pTarget->RemoveListener( pTargetListener );
	}
	
	SetTarget( NULL );
	SetTargetListener( NULL );
	
	if( pType == ectTarget && ! pTargetName.IsEmpty() ){
		SetTarget( triggerTable.GetNamedAddIfMissing( pTargetName.GetString() ) );
		
		if( listener ){
			pTarget->AddListener( listener );
			SetTargetListener( listener );
		}
	}
	
	const int count = pChildred.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeTriggerExpressionComponent* )pChildred.GetAt( i ) )->LinkTargets( triggerTable, listener );
	}
}

void igdeTriggerExpressionComponent::UnlinkTargets(){
	if( pTargetListener ){
		pTarget->RemoveListener( pTargetListener );
	}
	
	SetTarget( NULL );
	SetTargetListener( NULL );
	
	const int count = pChildred.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeTriggerExpressionComponent* )pChildred.GetAt( i ) )->UnlinkTargets();
	}
}

bool igdeTriggerExpressionComponent::Evaluate(){
	bool result = false;
	
	switch( pType ){
	case ectTarget:
		if( pTarget ){
			if( pCurState ){
				result = pTarget->GetFired();
				
			}else{
				result = pTarget->GetHasFired();
			}
			
		}else{
			return false; // in the case the trigger is not correctly configurated
		}
		break;
		
	case ectAnd:{
		const int count = pChildred.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			if( ! ( ( igdeTriggerExpressionComponent* )pChildred.GetAt( i ) )->Evaluate() ){
				break;
			}
		}
		result = count > 0 && i == count;
		}break;
		
	case ectOr:{
		const int count = pChildred.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			if( ( ( igdeTriggerExpressionComponent* )pChildred.GetAt( i ) )->Evaluate() ){
				break;
			}
		}
		result = i < count;
		}break;
		
	default:
		break;
	}
	
	return result ^ pNegate;
}



// Children
/////////////

int igdeTriggerExpressionComponent::GetChildCount() const{
	return pChildred.GetCount();
}

igdeTriggerExpressionComponent *igdeTriggerExpressionComponent::GetChildAt( int index ) const{
	return ( igdeTriggerExpressionComponent* )pChildred.GetAt( index );
}

int igdeTriggerExpressionComponent::IndexOfChild( igdeTriggerExpressionComponent *child ) const{
	return pChildred.IndexOf( child );
}

void igdeTriggerExpressionComponent::AddChild( igdeTriggerExpressionComponent *child ){
	if( ! child ){
		DETHROW( deeInvalidParam );
	}
	pChildred.Add( child );
}

void igdeTriggerExpressionComponent::InsertChild( igdeTriggerExpressionComponent *child, int index ){
	if( ! child ){
		DETHROW( deeInvalidParam );
	}
	pChildred.Insert( child, index );
}

void igdeTriggerExpressionComponent::RemoveChild( igdeTriggerExpressionComponent *child ){
	pChildred.Remove( child );
}

void igdeTriggerExpressionComponent::RemoveAllChildren(){
	pChildred.RemoveAll();
}

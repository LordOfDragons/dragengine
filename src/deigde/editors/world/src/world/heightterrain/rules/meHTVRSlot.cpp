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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHTVRule.h"
#include "meHTVRLink.h"
#include "meHTVRSlot.h"

#include <dragengine/common/exceptions.h>



// Class meHTVRSlot
//////////////////////////

// Constructor, destructor
////////////////////////////

meHTVRSlot::meHTVRSlot() :
pIsInput( true ){
}

meHTVRSlot::~meHTVRSlot(){
}



// Management
///////////////

void meHTVRSlot::SetIsInput( bool isInput ){
	pIsInput = isInput;
}



int meHTVRSlot::GetLinkCount() const{
	return pLinks.GetCount();
}

meHTVRLink *meHTVRSlot::GetLinkAt( int index ) const{
	return ( meHTVRLink* )pLinks.GetAt( index );
}

int meHTVRSlot::IndexOfLink( meHTVRLink *link ) const{
	return pLinks.IndexOf( link );
}

bool meHTVRSlot::HasLink( meHTVRLink *link ) const{
	return pLinks.Has( link );
}

void meHTVRSlot::AddLink( meHTVRLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void meHTVRSlot::RemoveLink( meHTVRLink *link ){
	pLinks.Remove( link );
}

void meHTVRSlot::RemoveAllLinks(){
	pLinks.RemoveAll();
}



bool meHTVRSlot::HasLinkWithSource( meHTVRule *rule, int slot ) const{
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meHTVRLink &link = *( meHTVRLink* )pLinks.GetAt( i );
		if( link.GetSourceRule() == rule && link.GetSourceSlot() == slot ){
			return true;
		}
	}
	
	return false;
}

meHTVRLink *meHTVRSlot::GetLinkWithSource( meHTVRule *rule, int slot ) const{
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		meHTVRLink * const link = ( meHTVRLink* )pLinks.GetAt( i );
		if( link->GetSourceRule() == rule && link->GetSourceSlot() == slot ){
			return link;
		}
	}
	
	return NULL;
}

bool meHTVRSlot::HasLinkWithDestination( meHTVRule *rule, int slot ) const{
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meHTVRLink &link = *( meHTVRLink* )pLinks.GetAt( i );
		if( link.GetDestinationRule() == rule && link.GetDestinationSlot() == slot ){
			return true;
		}
	}
	
	return false;
}

meHTVRLink *meHTVRSlot::GetLinkWithDestination( meHTVRule *rule, int slot ) const{
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		meHTVRLink * const link = ( meHTVRLink* )pLinks.GetAt( i );
		if( link->GetDestinationRule() == rule && link->GetDestinationSlot() == slot ){
			return link;
		}
	}
	
	return NULL;
}

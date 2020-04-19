/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeControllerTarget.h"
#include "../aeAnimator.h"
#include "../link/aeLink.h"

#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorControllerTarget.h>
#include <dragengine/common/exceptions.h>



// Class aeControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

aeControllerTarget::aeControllerTarget(){
}

aeControllerTarget::aeControllerTarget( const aeControllerTarget &copy ) :
pLinks( copy.pLinks ){
}

aeControllerTarget::~aeControllerTarget(){
	RemoveAllLinks();
}



// Management
///////////////

int aeControllerTarget::GetLinkCount() const{
	return pLinks.GetCount();
}

aeLink *aeControllerTarget::GetLinkAt( int index ) const{
	return ( aeLink* )pLinks.GetAt( index );
}

int aeControllerTarget::IndexOfLink( aeLink *link ) const{
	return pLinks.IndexOf( link );
}

bool aeControllerTarget::HasLink( aeLink *link ) const{
	return pLinks.Has( link );
}

void aeControllerTarget::AddLink( aeLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void aeControllerTarget::RemoveLink( aeLink *link ){
	pLinks.Remove( link );
}

void aeControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}



void aeControllerTarget::UpdateEngineTarget( aeAnimator *animator, deAnimatorControllerTarget &target ) const{
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	target.RemoveAllLinks();
	
	deAnimator * const engAnimator = animator->GetEngineAnimator();
	if( engAnimator ){
		const int linkCount = pLinks.GetCount();
		int i;
		
		for( i=0; i<linkCount; i++ ){
			deAnimatorLink * const engLink = ( ( aeLink* )pLinks.GetAt( i ) )->GetEngineLink();
			
			if( engLink ){
				const int indexLink = engAnimator->IndexOfLink( engLink );
				
				if( indexLink != -1 ){
					target.AddLink( indexLink );
				}
			}
		}
	}
}



void aeControllerTarget::AddLinksToList( aeLinkList &list ){
	list += pLinks;
}



// Operators
//////////////

aeControllerTarget &aeControllerTarget::operator=( const aeControllerTarget &copy ){
	pLinks = copy.pLinks;
	return *this;
}

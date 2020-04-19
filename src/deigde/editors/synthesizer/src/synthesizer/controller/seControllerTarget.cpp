/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seControllerTarget.h"
#include "../seSynthesizer.h"
#include "../link/seLink.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>



// Class seControllerTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

seControllerTarget::seControllerTarget(){
}

seControllerTarget::seControllerTarget( const seControllerTarget &copy ) :
pLinks( copy.pLinks ){
}

seControllerTarget::~seControllerTarget(){
	RemoveAllLinks();
}



// Management
///////////////

int seControllerTarget::GetLinkCount() const{
	return pLinks.GetCount();
}

seLink *seControllerTarget::GetLinkAt( int index ) const{
	return ( seLink* )pLinks.GetAt( index );
}

int seControllerTarget::IndexOfLink( seLink *link ) const{
	return pLinks.IndexOf( link );
}

bool seControllerTarget::HasLink( seLink *link ) const{
	return pLinks.Has( link );
}

void seControllerTarget::AddLink( seLink *link ){
	if( ! link ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void seControllerTarget::RemoveLink( seLink *link ){
	pLinks.Remove( link );
}

void seControllerTarget::RemoveAllLinks(){
	pLinks.RemoveAll();
}



void seControllerTarget::UpdateEngineTarget( seSynthesizer *synthesizer, deSynthesizerControllerTarget &target ) const{
	if( ! synthesizer ){
		DETHROW( deeInvalidParam );
	}
	
	target.RemoveAllLinks();
	
	deSynthesizer * const engSynthesizer = synthesizer->GetEngineSynthesizer();
	if( ! engSynthesizer ){
		return;
	}
	
	const int linkCount = pLinks.GetCount();
	int i;
	
	for( i=0; i<linkCount; i++ ){
		deSynthesizerLink * const engLink = ( ( seLink* )pLinks.GetAt( i ) )->GetEngineLink();
		if( ! engLink ){
			continue;
		}
		
		const int indexLink = engSynthesizer->IndexOfLink( engLink );
		if( indexLink == -1 ){
			continue;
		}
		
		target.AddLink( indexLink );
	}
}



void seControllerTarget::AddLinksToList( seLinkList &list ){
	list += pLinks;
}



// Operators
//////////////

seControllerTarget &seControllerTarget::operator=( const seControllerTarget &copy ){
	pLinks = copy.pLinks;
	return *this;
}

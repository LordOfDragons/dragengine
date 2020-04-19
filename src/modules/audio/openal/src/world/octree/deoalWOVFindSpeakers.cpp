/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalWorldOctree.h"
#include "deoalWOVFindSpeakers.h"
#include "../deoalAWorld.h"
#include "../../speaker/deoalASpeaker.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVFindSpeakers
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVFindSpeakers::deoalWOVFindSpeakers( const decDVector &position,
	const decLayerMask &layerMask, deoalSpeakerList &speakerList ) :
pPosition( position ),
pLayerMask( layerMask ),
pSpeakerList( speakerList )
{
	SetVisitAll( false );
	SetVisitSpeakers( true );
}

deoalWOVFindSpeakers::~deoalWOVFindSpeakers(){
}



// Visiting
/////////////

void deoalWOVFindSpeakers::FindSpeakers( deoalAWorld &world, const decDVector &position,
const decLayerMask &layerMask, deoalSpeakerList &speakerList ){
	deoalWOVFindSpeakers visitor( position, layerMask, speakerList );
	
	const double visitRange = 0.01;
	const decDVector visitRangeVector( visitRange, visitRange, visitRange );
	const decDVector visitBoxMin( position - visitRangeVector );
	const decDVector visitBoxMax( position + visitRangeVector );
	
	world.GetOctree()->VisitNodesColliding( &visitor, visitBoxMin, visitBoxMax );
}



void deoalWOVFindSpeakers::VisitSpeaker( deoalASpeaker *speaker ){
	if( speaker->GetLayerMask().MatchesNot( pLayerMask ) ){
		return;
	}
	if( ( speaker->GetPosition() - pPosition ).LengthSquared() > speaker->GetRangeSquared() ){
		return;
	}
	
	pSpeakerList.Add( speaker );
}

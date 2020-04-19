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

#include "deoalWOVSLMFindSpeakers.h"
#include "deoalASoundLevelMeter.h"
#include "../world/octree/deoalWorldOctree.h"
#include "../speaker/deoalASpeaker.h"
#include "../speaker/deoalSpeakerList.h"
#include "../world/deoalAWorld.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVSLMFindSpeakers
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVSLMFindSpeakers::deoalWOVSLMFindSpeakers(
const deoalASoundLevelMeter &soundLevelMeter, deoalSpeakerList &list ) :
pSoundLevelMeter( soundLevelMeter ),
pRangeSquared( soundLevelMeter.GetAudibleDistance() * soundLevelMeter.GetAudibleDistance() ),
pList( list )
{
	SetVisitAll( false );
	SetVisitSpeakers( true );
}

deoalWOVSLMFindSpeakers::~deoalWOVSLMFindSpeakers(){
}



// Visiting
/////////////

void deoalWOVSLMFindSpeakers::Visit( deoalAWorld &world ){
	const double audibleDistance = pSoundLevelMeter.GetAudibleDistance();
	const decDVector &position = pSoundLevelMeter.GetPosition();
	const decDVector radius( audibleDistance, audibleDistance, audibleDistance );
	world.GetOctree()->VisitNodesColliding( this, position - radius, position + radius );
}



void deoalWOVSLMFindSpeakers::VisitNode( deoalDOctree *node, int ){
	const deoalWorldOctree &sonode = *( ( deoalWorldOctree* )node );
	const decLayerMask &layerMask = pSoundLevelMeter.GetLayerMask();
	const decDVector &position = pSoundLevelMeter.GetPosition();
	const int count = sonode.GetSpeakerCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalASpeaker * const speaker = sonode.GetSpeakerAt( i );
		if( speaker->GetLayerMask().MatchesNot( layerMask ) ){
			continue;
		}
		if( ( speaker->GetPosition() - position ).LengthSquared() > pRangeSquared ){
			continue;
		}
		
		pList.Add( speaker );
	}
}

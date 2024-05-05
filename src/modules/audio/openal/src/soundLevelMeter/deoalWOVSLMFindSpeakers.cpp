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

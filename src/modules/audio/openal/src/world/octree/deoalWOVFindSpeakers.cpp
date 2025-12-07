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

#include "deoalWorldOctree.h"
#include "deoalWOVFindSpeakers.h"
#include "../deoalAWorld.h"
#include "../../speaker/deoalASpeaker.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVFindSpeakers
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVFindSpeakers::deoalWOVFindSpeakers(const decDVector &position,
	const decLayerMask &layerMask, deoalSpeakerList &speakerList) :
pPosition(position),
pLayerMask(layerMask),
pSpeakerList(speakerList)
{
	SetVisitAll(false);
	SetVisitSpeakers(true);
}

deoalWOVFindSpeakers::~deoalWOVFindSpeakers(){
}



// Visiting
/////////////

void deoalWOVFindSpeakers::FindSpeakers(deoalAWorld &world, const decDVector &position,
const decLayerMask &layerMask, deoalSpeakerList &speakerList){
	deoalWOVFindSpeakers visitor(position, layerMask, speakerList);
	
	const double visitRange = 0.01;
	const decDVector visitRangeVector(visitRange, visitRange, visitRange);
	const decDVector visitBoxMin(position - visitRangeVector);
	const decDVector visitBoxMax(position + visitRangeVector);
	
	world.GetOctree()->VisitNodesColliding(&visitor, visitBoxMin, visitBoxMax);
}



void deoalWOVFindSpeakers::VisitSpeaker(deoalASpeaker *speaker){
	if(speaker->GetLayerMask().MatchesNot(pLayerMask)){
		return;
	}
	if((speaker->GetPosition() - pPosition).LengthSquared() > speaker->GetRangeSquared()){
		return;
	}
	
	pSpeakerList.Add(speaker);
}

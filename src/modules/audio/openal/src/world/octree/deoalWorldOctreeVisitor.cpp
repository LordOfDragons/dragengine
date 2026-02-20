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
#include "deoalWorldOctreeVisitor.h"

#include "../../component/deoalAComponent.h"
#include "../../environment/deoalEnvProbe.h"
#include "../../microphone/deoalAMicrophone.h"
#include "../../speaker/deoalASpeaker.h"
#include "../../soundLevelMeter/deoalASoundLevelMeter.h"
#include "../../utils/collision/deoalDCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoalWorldOctreeVisitor
//////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWorldOctreeVisitor::deoalWorldOctreeVisitor() :
pVisitMicrophones(true),
pVisitComponents(true),
pVisitSpeakers(true),
pVisitEnvProbes(true),
pVisitSoundLevelMeters(true){
}

deoalWorldOctreeVisitor::~deoalWorldOctreeVisitor(){
}



// Management
///////////////

void deoalWorldOctreeVisitor::SetVisitComponents(bool visitComponents){
	pVisitComponents = visitComponents;
}

void deoalWorldOctreeVisitor::SetVisitMicrophones(bool visitMicrophones){
	pVisitMicrophones = visitMicrophones;
}

void deoalWorldOctreeVisitor::SetVisitSpeakers(bool visitSpeakers){
	pVisitSpeakers = visitSpeakers;
}

void deoalWorldOctreeVisitor::SetVisitEnvProbes(bool visitEnvProbes){
	pVisitEnvProbes = visitEnvProbes;
}

void deoalWorldOctreeVisitor::SetVisitSoundLevelMeters(bool visitSoundLevelMeters){
	pVisitSoundLevelMeters = visitSoundLevelMeters;
}

void deoalWorldOctreeVisitor::SetVisitAll(bool visitAll){
	pVisitComponents = visitAll;
	pVisitMicrophones = visitAll;
	pVisitSpeakers = visitAll;
	pVisitEnvProbes = visitAll;
	pVisitSoundLevelMeters = visitAll;
}



// Visiting
/////////////

void deoalWorldOctreeVisitor::VisitNode(deoalWorldOctree &node){
	VisitNode(&node, deoalDCollisionDetection::eirPartial);
}

void deoalWorldOctreeVisitor::VisitNode(deoalDOctree *node, int){
	const deoalWorldOctree &sonode = *((deoalWorldOctree*)node);
	int i;
	
	if(pVisitComponents){
		const int count = sonode.GetComponentCount();
		
		for(i=0; i<count; i++){
			VisitComponent(sonode.GetComponentAt(i));
		}
	}
	
	if(pVisitMicrophones){
		const int count = sonode.GetMicrophoneCount();
		
		for(i=0; i<count; i++){
			VisitMicrophone(sonode.GetMicrophoneAt(i));
		}
	}
	
	if(pVisitSpeakers){
		const int count = sonode.GetSpeakerCount();
		
		for(i=0; i<count; i++){
			VisitSpeaker(sonode.GetSpeakerAt(i));
		}
	}
	
	if(pVisitEnvProbes){
		const int count = sonode.GetEnvProbeCount();
		
		for(i=0; i<count; i++){
			VisitEnvProbe(sonode.GetEnvProbeAt(i));
		}
	}
	
	if(pVisitSoundLevelMeters){
		const int count = sonode.GetSoundLevelMeterCount();
		
		for(i=0; i<count; i++){
			VisitSoundLevelMeter(sonode.GetSoundLevelMeterAt(i));
		}
	}
}

void deoalWorldOctreeVisitor::VisitComponent(deoalAComponent*){
}

void deoalWorldOctreeVisitor::VisitMicrophone(deoalAMicrophone*){
}

void deoalWorldOctreeVisitor::VisitSpeaker(deoalASpeaker*){
}

void deoalWorldOctreeVisitor::VisitEnvProbe(deoalEnvProbe*){
}

void deoalWorldOctreeVisitor::VisitSoundLevelMeter(deoalASoundLevelMeter*){
}

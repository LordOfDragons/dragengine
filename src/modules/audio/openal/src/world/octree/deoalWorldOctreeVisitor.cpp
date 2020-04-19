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
pVisitMicrophones( true ),
pVisitComponents( true ),
pVisitSpeakers( true ),
pVisitEnvProbes( true ),
pVisitSoundLevelMeters( true ){
}

deoalWorldOctreeVisitor::~deoalWorldOctreeVisitor(){
}



// Management
///////////////

void deoalWorldOctreeVisitor::SetVisitComponents( bool visitComponents ){
	pVisitComponents = visitComponents;
}

void deoalWorldOctreeVisitor::SetVisitMicrophones( bool visitMicrophones ){
	pVisitMicrophones = visitMicrophones;
}

void deoalWorldOctreeVisitor::SetVisitSpeakers( bool visitSpeakers ){
	pVisitSpeakers = visitSpeakers;
}

void deoalWorldOctreeVisitor::SetVisitEnvProbes( bool visitEnvProbes ){
	pVisitEnvProbes = visitEnvProbes;
}

void deoalWorldOctreeVisitor::SetVisitSoundLevelMeters( bool visitSoundLevelMeters ){
	pVisitSoundLevelMeters = visitSoundLevelMeters;
}

void deoalWorldOctreeVisitor::SetVisitAll( bool visitAll ){
	pVisitComponents = visitAll;
	pVisitMicrophones = visitAll;
	pVisitSpeakers = visitAll;
	pVisitEnvProbes = visitAll;
	pVisitSoundLevelMeters = visitAll;
}



// Visiting
/////////////

void deoalWorldOctreeVisitor::VisitNode( deoalWorldOctree &node ){
	VisitNode( &node, deoalDCollisionDetection::eirPartial );
}

void deoalWorldOctreeVisitor::VisitNode( deoalDOctree *node, int ){
	const deoalWorldOctree &sonode = *( ( deoalWorldOctree* )node );
	int i;
	
	if( pVisitComponents ){
		const int count = sonode.GetComponentCount();
		
		for( i=0; i<count; i++ ){
			VisitComponent( sonode.GetComponentAt( i ) );
		}
	}
	
	if( pVisitMicrophones ){
		const int count = sonode.GetMicrophoneCount();
		
		for( i=0; i<count; i++ ){
			VisitMicrophone( sonode.GetMicrophoneAt( i ) );
		}
	}
	
	if( pVisitSpeakers ){
		const int count = sonode.GetSpeakerCount();
		
		for( i=0; i<count; i++ ){
			VisitSpeaker( sonode.GetSpeakerAt( i ) );
		}
	}
	
	if( pVisitEnvProbes ){
		const int count = sonode.GetEnvProbeCount();
		
		for( i=0; i<count; i++ ){
			VisitEnvProbe( sonode.GetEnvProbeAt( i ) );
		}
	}
	
	if( pVisitSoundLevelMeters ){
		const int count = sonode.GetSoundLevelMeterCount();
		
		for( i=0; i<count; i++ ){
			VisitSoundLevelMeter( sonode.GetSoundLevelMeterAt( i ) );
		}
	}
}

void deoalWorldOctreeVisitor::VisitComponent( deoalAComponent* ){
}

void deoalWorldOctreeVisitor::VisitMicrophone( deoalAMicrophone* ){
}

void deoalWorldOctreeVisitor::VisitSpeaker( deoalASpeaker* ){
}

void deoalWorldOctreeVisitor::VisitEnvProbe( deoalEnvProbe* ){
}

void deoalWorldOctreeVisitor::VisitSoundLevelMeter( deoalASoundLevelMeter* ){
}

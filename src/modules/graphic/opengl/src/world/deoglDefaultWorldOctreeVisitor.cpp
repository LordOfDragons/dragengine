/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglWorldOctree.h"
#include "deoglDefaultWorldOctreeVisitor.h"

#include <dragengine/common/exceptions.h>



// Class deoglDefaultWorldOctreeVisitor
/////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglDefaultWorldOctreeVisitor::deoglDefaultWorldOctreeVisitor() :
pVisitBillboards( false ),
pVisitComponents( false ),
pVisitEnvMaps( false ),
pVisitLights( false ),
pVisitParticleEmitters( false ){
}

deoglDefaultWorldOctreeVisitor::~deoglDefaultWorldOctreeVisitor(){
}



// Management
///////////////

void deoglDefaultWorldOctreeVisitor::SetVisitBillboards( bool visitBillboards ){
	pVisitBillboards = visitBillboards;
}

void deoglDefaultWorldOctreeVisitor::SetVisitComponents( bool visitComponents ){
	pVisitComponents = visitComponents;
}

void deoglDefaultWorldOctreeVisitor::SetVisitEnvMaps( bool visitEnvMaps ){
	pVisitEnvMaps = visitEnvMaps;
}

void deoglDefaultWorldOctreeVisitor::SetVisitLights( bool visitLights ){
	pVisitLights = visitLights;
}

void deoglDefaultWorldOctreeVisitor::SetVisitParticleEmitters( bool visitEmitters ){
	pVisitParticleEmitters = visitEmitters;
}

void deoglDefaultWorldOctreeVisitor::SetVisitAll( bool visitAll ){
	pVisitBillboards = visitAll;
	pVisitComponents = visitAll;
	pVisitEnvMaps = visitAll;
	pVisitLights = visitAll;
	pVisitParticleEmitters = visitAll;
}



// Visiting
/////////////

void deoglDefaultWorldOctreeVisitor::VisitNode( deoglDOctree *node, int intersection ){
	const deoglWorldOctree &sonode = *( ( deoglWorldOctree* )node );
	int i;
	
	if( pVisitBillboards ){
		const deoglBillboardList &list = sonode.GetBillboardList();
		const int count = list.GetCount();
		
		for( i=0; i<count; i++ ){
			VisitBillboard( list.GetAt( i ) );
		}
	}
	
	if( pVisitComponents ){
		const int count = sonode.GetComponentCount();
		
		for( i=0; i<count; i++ ){
			VisitComponent( sonode.GetComponentAt( i ) );
		}
	}
	
	if( pVisitEnvMaps ){
		const deoglEnvironmentMapList &list = sonode.GetEnvMapList();
		const int count = list.GetCount();
		
		for( i=0; i<count; i++ ){
			VisitEnvMaps( list.GetAt( i ) );
		}
	}
	
	if( pVisitLights ){
		const int count = sonode.GetLightCount();
		
		for( i=0; i<count; i++ ){
			VisitLight( sonode.GetLightAt( i ) );
		}
	}
	
	if( pVisitParticleEmitters ){
		const deoglParticleEmitterInstanceList &list = sonode.GetParticleEmittersList();
		const int count = list.GetCount();
		
		for( i=0; i<count; i++ ){
			VisitParticleEmitter( list.GetAt( i ) );
		}
	}
}

void deoglDefaultWorldOctreeVisitor::VisitBillboard( deoglRBillboard *billboard ){
}

void deoglDefaultWorldOctreeVisitor::VisitComponent( deoglRComponent *component ){
}

void deoglDefaultWorldOctreeVisitor::VisitEnvMaps( deoglEnvironmentMap *envmap ){
}

void deoglDefaultWorldOctreeVisitor::VisitLight( deoglRLight *light ){
}

void deoglDefaultWorldOctreeVisitor::VisitParticleEmitter( deoglRParticleEmitterInstance *emitter ){
}

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

#include "deoglWorldOctree.h"
#include "deoglDefaultWorldOctreeVisitor.h"

#include <dragengine/common/exceptions.h>



// Class deoglDefaultWorldOctreeVisitor
/////////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglDefaultWorldOctreeVisitor::deoglDefaultWorldOctreeVisitor() :
pVisitBillboards(false),
pVisitComponents(false),
pVisitEnvMaps(false),
pVisitLights(false),
pVisitParticleEmitters(false){
}

deoglDefaultWorldOctreeVisitor::~deoglDefaultWorldOctreeVisitor(){
}



// Management
///////////////

void deoglDefaultWorldOctreeVisitor::SetVisitBillboards(bool visitBillboards){
	pVisitBillboards = visitBillboards;
}

void deoglDefaultWorldOctreeVisitor::SetVisitComponents(bool visitComponents){
	pVisitComponents = visitComponents;
}

void deoglDefaultWorldOctreeVisitor::SetVisitEnvMaps(bool visitEnvMaps){
	pVisitEnvMaps = visitEnvMaps;
}

void deoglDefaultWorldOctreeVisitor::SetVisitLights(bool visitLights){
	pVisitLights = visitLights;
}

void deoglDefaultWorldOctreeVisitor::SetVisitParticleEmitters(bool visitEmitters){
	pVisitParticleEmitters = visitEmitters;
}

void deoglDefaultWorldOctreeVisitor::SetVisitAll(bool visitAll){
	pVisitBillboards = visitAll;
	pVisitComponents = visitAll;
	pVisitEnvMaps = visitAll;
	pVisitLights = visitAll;
	pVisitParticleEmitters = visitAll;
}



// Visiting
/////////////

void deoglDefaultWorldOctreeVisitor::VisitNode(deoglDOctree *node, int intersection){
	const deoglWorldOctree &sonode = *((deoglWorldOctree*)node);
	int i;
	
	if(pVisitBillboards){
		
		sonode.GetBillboardList().Visit([&](deoglRBillboard *billboard){
			VisitBillboard(billboard);
		});
	}
	
	if(pVisitComponents){
		const int count = sonode.GetComponentCount();
		
		for(i=0; i<count; i++){
			VisitComponent(sonode.GetComponentAt(i));
		}
	}
	
	if(pVisitEnvMaps){
		sonode.GetEnvMapList().Visit([&](deoglEnvironmentMap *envmap){
			VisitEnvMaps(envmap);
		});
	}
	
	if(pVisitLights){
		const int count = sonode.GetLightCount();
		
		for(i=0; i<count; i++){
			VisitLight(sonode.GetLightAt(i));
		}
	}
	
	if(pVisitParticleEmitters){
		sonode.GetParticleEmittersList().Visit([&](deoglRParticleEmitterInstance *emitter){
			VisitParticleEmitter(emitter);
		});
	}
}

void deoglDefaultWorldOctreeVisitor::VisitBillboard(deoglRBillboard *billboard){
}

void deoglDefaultWorldOctreeVisitor::VisitComponent(deoglRComponent *component){
}

void deoglDefaultWorldOctreeVisitor::VisitEnvMaps(deoglEnvironmentMap *envmap){
}

void deoglDefaultWorldOctreeVisitor::VisitLight(deoglRLight *light){
}

void deoglDefaultWorldOctreeVisitor::VisitParticleEmitter(deoglRParticleEmitterInstance *emitter){
}

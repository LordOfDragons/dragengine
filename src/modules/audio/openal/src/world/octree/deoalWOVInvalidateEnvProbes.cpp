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
#include "deoalWOVInvalidateEnvProbes.h"
#include "../../environment/deoalEnvProbe.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVInvalidateEnvProbes
//////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVInvalidateEnvProbes::deoalWOVInvalidateEnvProbes()
{
	SetVisitAll(false);
	SetVisitEnvProbes(true);
}

deoalWOVInvalidateEnvProbes::deoalWOVInvalidateEnvProbes(const decDVector &minExtend,
	const decDVector &maxExtend, const decLayerMask &layerMask) :
pMinExtend(minExtend),
pMaxExtend(maxExtend),
pLayerMask(layerMask)
{
	SetVisitAll(false);
	SetVisitEnvProbes(true);
}

deoalWOVInvalidateEnvProbes::~deoalWOVInvalidateEnvProbes(){
}



// Visiting
/////////////

void deoalWOVInvalidateEnvProbes::SetExtends(const decDVector &minExtend, const decDVector &maxExtend){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoalWOVInvalidateEnvProbes::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}



void deoalWOVInvalidateEnvProbes::VisitNode(deoalDOctree *node, int){
	deoalWorldOctree &sonode = *((deoalWorldOctree*)node);
	int i, count = sonode.GetEnvProbeCount();
	
	// collect environment probes to remove
	pRemoveEnvProbes.RemoveAll();
	
	for(i=0; i<count; i++){
		deoalEnvProbe &envProbe = *sonode.GetEnvProbeAt(i);
		if(envProbe.GetLayerMask().MatchesNot(pLayerMask)){
			continue;
		}
		if(envProbe.GetMaxExtend() < pMinExtend || envProbe.GetMinExtend() > pMaxExtend){
			continue;
		}
		pRemoveEnvProbes.Add(&envProbe);
	}
	
	// remove collected environment probes
	count = pRemoveEnvProbes.GetCount();
	for(i=0; i<count; i++){
		deoalEnvProbe * const envProbe = (deoalEnvProbe*)pRemoveEnvProbes.GetAt(i);
		sonode.RemoveEnvProbe(envProbe);
		envProbe->SetOctreeNode(NULL);
		envProbe->Invalidate();
	}
}

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

#include "deoglCollideListHTSector.h"
#include "deoglCollideListHTSCluster.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglHTViewSectorCluster.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglRHeightTerrain.h"
#include "../terrain/heightmap/deoglRHTSector.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListHTSCluster
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListHTSCluster::deoglCollideListHTSCluster() :
pCluster( nullptr ),
pIndex( 0 ),
pCulled( false ),
pCascadeMask( 0 ){
}

deoglCollideListHTSCluster::~deoglCollideListHTSCluster(){
}



// Management
///////////////

void deoglCollideListHTSCluster::SetCluster( deoglHTViewSectorCluster *cluster ){
	DEASSERT_NOTNULL( cluster )
	pCluster = cluster;
	pCoordinates = cluster->GetCoordinate();
	pIndex = pCoordinates.y * cluster->GetSector().GetSector().GetClusterCount() + pCoordinates.x;
}

void deoglCollideListHTSCluster::Clear(){
	pCoordinates.SetZero();
	pIndex = 0;
	pCulled = false;
	pCascadeMask = 0;
	pCluster = nullptr;
}

void deoglCollideListHTSCluster::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListHTSCluster::SetCascadeMask( int mask ){
	pCascadeMask = mask;
}

void deoglCollideListHTSCluster::StartOcclusionTest(
deoglOcclusionTest &occlusionTest, const decVector &offset ){
	const deoglHTSCluster &cluster = pCluster->GetSector().GetSector().GetClusterAt( pCoordinates.x, pCoordinates.y );
	const decVector realOffset( cluster.GetCenter() + offset );
	
	pCulled = false;
	occlusionTest.AddInputData( realOffset - cluster.GetHalfExtends(),
		realOffset + cluster.GetHalfExtends(), this );
}

void deoglCollideListHTSCluster::OcclusionTestInvisible(){
	pCulled = true;
}

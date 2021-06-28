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

#include "deoglCollideListHTSector.h"
#include "deoglCollideListHTSCluster.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglRHeightTerrain.h"
#include "../terrain/heightmap/deoglRHTSector.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListHTSCluster
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListHTSCluster::deoglCollideListHTSCluster( deoglCollideListHTSector &sector ) :
pSector( sector ),
pIndex( 0 ),
pCulled( false ),
pCascadeMask( 0 ){
}

deoglCollideListHTSCluster::~deoglCollideListHTSCluster(){
}



// Management
///////////////

void deoglCollideListHTSCluster::Clear(){
	pCoordinates.SetZero();
	pIndex = 0;
	pCulled = false;
	pCascadeMask = 0;
}

void deoglCollideListHTSCluster::SetCoordinates( const decPoint &coordinates ){
	pCoordinates = coordinates;
	pIndex = coordinates.y * pSector.GetSector()->GetSector().GetClusterCount() + coordinates.x;
}

void deoglCollideListHTSCluster::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListHTSCluster::SetCascadeMask( int mask ){
	pCascadeMask = mask;
}

void deoglCollideListHTSCluster::StartOcclusionTest(
deoglOcclusionTest &occlusionTest, const decVector &offset ){
	const deoglHTSCluster &cluster = pSector.GetSector()->GetSector().
		GetClusterAt( pCoordinates.x, pCoordinates.y );
	const decVector realOffset( cluster.GetCenter() + offset );
	
	pCulled = false;
	occlusionTest.AddInputData( realOffset - cluster.GetHalfExtends(),
		realOffset + cluster.GetHalfExtends(), this );
}

void deoglCollideListHTSCluster::OcclusionTestInvisible(){
	pCulled = true;
}

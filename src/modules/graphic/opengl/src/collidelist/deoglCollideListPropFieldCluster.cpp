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

#include "deoglCollideListPropFieldCluster.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropFieldCluster
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropFieldCluster::deoglCollideListPropFieldCluster() :
pCluster( NULL ),
pCulled( false ),
pCascadeMask( 0 ){
}

deoglCollideListPropFieldCluster::~deoglCollideListPropFieldCluster(){
}



// Management
///////////////

void deoglCollideListPropFieldCluster::Clear(){
	pCluster = nullptr;
	pCulled = false;
	pCascadeMask = 0;
}

void deoglCollideListPropFieldCluster::SetCluster( deoglPropFieldCluster *cluster ){
	pCluster = cluster;
}

void deoglCollideListPropFieldCluster::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListPropFieldCluster::SetCascadeMask( int mask ){
	pCascadeMask = mask;
}

void deoglCollideListPropFieldCluster::StartOcclusionTest( deoglOcclusionTest &occlusionTest, const decVector &offset ){
	DEASSERT_NOTNULL( pCluster )
	
	pCulled = false;
	occlusionTest.AddInputData( offset - pCluster->GetMinimumExtend(), offset + pCluster->GetMaximumExtend(), this );
	pCulled = false;
}

void deoglCollideListPropFieldCluster::OcclusionTestInvisible(){
	pCulled = true;
}

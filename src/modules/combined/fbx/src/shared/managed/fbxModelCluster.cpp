/* 
 * FBX Model Module
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
#include <inttypes.h>

#include "fbxModel.h"
#include "fbxModelCluster.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxModelCluster
// ///////////////////////

// Constructor, destructor
////////////////////////////

fbxModelCluster::fbxModelCluster( fbxModel &model, fbxNode &nodeCluster ) :
pModel( model ),
pNodeCluster( nodeCluster ),
pName( nodeCluster.GetPropertyAt( 1 )->CastString().GetValue() ),
pRigBone( NULL )
{
	// no idea where this is coming from but some test FBX files seem to append "Cluster"
	// to the end of the bone name. without removing it matching does not work
	if( pName.GetRight( 7 ) == "Cluster" ){
		pName = pName.GetMiddle( 0, -7 );
	}
}

fbxModelCluster::~fbxModelCluster(){
}



// Management
///////////////

void fbxModelCluster::SetName( const char *name ){
	pName = name;
}

void fbxModelCluster::SetRigBone( fbxRigBone *rigBone ){
	pRigBone = rigBone;
}



void fbxModelCluster::Prepare(){
}



void fbxModelCluster::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose) const{
	module.LogInfoFormat( "%sCluster '%s'", prefix.GetString(), pName.GetString() );
}
